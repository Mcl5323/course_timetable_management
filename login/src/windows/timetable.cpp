#include "timetable.h"
#include "ui_timetable.h"
#include "managecoursespage.h"
#include "uistyles.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QTableWidgetItem>
#include <QColor>

TIMETABLE::TIMETABLE(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TIMETABLE)
    , currentCombinationIndex(0)
{
    ui->setupUi(this);

    // Initialize window properties and maximize
    this->setWindowTitle("View Timetable");
    this->setWindowFlags(Qt::Window);  // Set as normal window
    this->setWindowState(Qt::WindowMaximized);
    this->showMaximized();

    // Connect button signals to slots
    connect(ui->saveAsBtn, &QPushButton::clicked, this, &TIMETABLE::onSaveAs);
    connect(ui->backBtn, &QPushButton::clicked, this, &TIMETABLE::onBack);
    connect(ui->prevPageBtn, &QPushButton::clicked, this, &TIMETABLE::onPrevPage);
    connect(ui->nextPageBtn, &QPushButton::clicked, this, &TIMETABLE::onNextPage);
    connect(ui->deleteBtn, &QPushButton::clicked, this, &TIMETABLE::onDelete);

    // Configure timetable table properties
    if (ui->timetableTable) {
        ui->timetableTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // Disable editing
        ui->timetableTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  // Stretch columns
        ui->timetableTable->horizontalHeader()->setStretchLastSection(true);
        ui->timetableTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->timetableTable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);  // Enable scrolling for all 7 days

        for (int row = 0; row < ui->timetableTable->rowCount(); ++row) {
            ui->timetableTable->setRowHeight(row, 80);  // Set row height for better display
        }

        ui->timetableTable->setWordWrap(true);  // Enable word wrapping
        ui->timetableTable->setTextElideMode(Qt::ElideNone);
    }
}

TIMETABLE::~TIMETABLE()
{
    delete ui;
}

// Set course data and generate timetable combinations
void TIMETABLE::setCoursesData(const LinkedList<Course> &courses)
{
    coursesData = courses;  // Store courses locally
    allCombinations.clear();  // Clear previous combinations
    currentCombinationIndex = 0;  // Reset to first page

    generateAllCombinations();  // Generate all possible timetable combinations

    if (!allCombinations.isEmpty()) {
        displayCurrentCombination();  // Show first combination
        updatePageLabel();
    } else {
        populateTimetable();  // Show all courses if no valid combinations
        updateStatistics();
        updatePageLabel();
    }
}

// Populate timetable with course data
void TIMETABLE::populateTimetable()
{
    if (!ui->timetableTable) return;

    ui->timetableTable->clearContents();  // Clear existing content

    // Process each course and create table cells
    for (int i = 0; i < coursesData.size(); ++i) {
        const Course &course = coursesData[i];
        int row = dayToRow(course.day);  // Convert day to row index
        int startCol = timeToColumn(course.startTime);  // Convert time to column index
        int endCol = timeToColumn(course.endTime);

        if (row < 0 || startCol < 0 || endCol < 0 || startCol >= endCol) continue;  // Skip invalid data

        int colSpan = endCol - startCol;  // Calculate cell span

        // Create table item with course info
        QTableWidgetItem *mainItem = new QTableWidgetItem(
            QString("%1\n%2\n%3-%4")
                .arg(course.name)
                .arg(course.classroom)
                .arg(course.startTime)
                .arg(course.endTime)
        );

        // Set cell styling
        mainItem->setBackground(QBrush(QColor(UIColors::BLUE_DEEP)));
        mainItem->setForeground(QBrush(QColor(UIColors::TEXT_WHITE)));
        mainItem->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        mainItem->setData(Qt::TextWordWrap, true);

        QFont font = mainItem->font();
        font.setBold(true);
        font.setPointSize(8);  // Smaller font for compact display
        mainItem->setFont(font);

        ui->timetableTable->setItem(row, startCol, mainItem);

        if (colSpan > 1) {
            ui->timetableTable->setSpan(row, startCol, 1, colSpan);  // Merge cells for multi-hour courses
        }
    }
}

// Update statistics labels
void TIMETABLE::updateStatistics()
{
    if (!ui->totalCourseLabel || !ui->totalHoursLabel || !ui->conflictsLabel) return;

    int totalCourses = coursesData.size();
    int totalHours = calculateTotalHours();
    int conflicts = detectConflicts();

    ui->totalCourseLabel->setText(QString("Total Course: %1").arg(totalCourses));
    ui->totalHoursLabel->setText(QString("Total Hours: %1").arg(totalHours));
    ui->conflictsLabel->setText(QString("Conflicts: %1").arg(conflicts));
}

// Calculate total hours of all courses
int TIMETABLE::calculateTotalHours()
{
    int total = 0;
    for (int i = 0; i < coursesData.size(); ++i) {
        const Course &course = coursesData[i];
        int start = timeToColumn(course.startTime);
        int end = timeToColumn(course.endTime);
        if (start >= 0 && end >= 0) {
            total += (end - start);  // Add duration
        }
    }
    return total;
}

// Detect time conflicts between courses
int TIMETABLE::detectConflicts()
{
    int conflicts = 0;

    // Check every pair of courses
    for (int i = 0; i < coursesData.size(); ++i) {
        for (int j = i + 1; j < coursesData.size(); ++j) {
            const Course &course1 = coursesData[i];
            const Course &course2 = coursesData[j];

            if (course1.day != course2.day) continue;  // Skip if different days

            int start1 = timeToColumn(course1.startTime);
            int end1 = timeToColumn(course1.endTime);
            int start2 = timeToColumn(course2.startTime);
            int end2 = timeToColumn(course2.endTime);

            if (start1 < 0 || end1 < 0 || start2 < 0 || end2 < 0) continue;  // Skip invalid times

            if (start1 < end2 && start2 < end1) {  // Check time overlap
                conflicts++;
            }
        }
    }

    return conflicts;
}

// Convert time string to column index (e.g., "8am" -> 0, "2pm" -> 6)
int TIMETABLE::timeToColumn(const QString &time)
{
    QString t = time.toLower().trimmed();
    bool isPM = t.contains("pm");

    QString numStr = t;
    numStr.remove("am").remove("pm").remove(".00");  // Clean up string
    int hour = numStr.toInt();

    // Convert 12-hour format to 24-hour format
    if (isPM && hour != 12) {
        hour += 12;  // e.g., 2pm -> 14
    } else if (!isPM && hour == 12) {
        hour = 0;  // 12am -> 0
    }

    if (hour >= 8 && hour <= 21) {
        return hour - 8;  // Timetable starts at 8am (column 0)
    }

    return -1;  // Invalid time
}

// Convert day name to row index
int TIMETABLE::dayToRow(const QString &day)
{
    HashTable<QString, int> dayMap;
    dayMap.insert("Monday", 0);
    dayMap.insert("Tuesday", 1);
    dayMap.insert("Wednesday", 2);
    dayMap.insert("Thursday", 3);
    dayMap.insert("Friday", 4);
    dayMap.insert("Saturday", 5);
    dayMap.insert("Sunday", 6);

    return dayMap.value(day, -1);  // Return -1 if day not found
}

// Save timetable as image file
void TIMETABLE::onSaveAs()
{
    if (!ui->timetableTable) {
        QMessageBox::warning(this, "Error", "Timetable table not found!");
        return;
    }

    // Open file dialog to select save location
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save Timetable As",
        QDir::homePath() + "/timetable.png",
        "PNG Image (*.png);;JPEG Image (*.jpg);;All Files (*.*)"
    );

    if (fileName.isEmpty()) return;  // User cancelled

    // Save original properties
    QSize originalSize = ui->timetableTable->size();
    Qt::ScrollBarPolicy oldHPolicy = ui->timetableTable->horizontalScrollBarPolicy();
    Qt::ScrollBarPolicy oldVPolicy = ui->timetableTable->verticalScrollBarPolicy();

    // Temporarily disable scrollbars
    ui->timetableTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->timetableTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Calculate total size needed for all 7 days
    int totalWidth = ui->timetableTable->verticalHeader()->width();
    for (int col = 0; col < ui->timetableTable->columnCount(); ++col) {
        totalWidth += ui->timetableTable->columnWidth(col);
    }

    int totalHeight = ui->timetableTable->horizontalHeader()->height();
    for (int row = 0; row < ui->timetableTable->rowCount(); ++row) {
        totalHeight += ui->timetableTable->rowHeight(row);
    }

    ui->timetableTable->resize(totalWidth, totalHeight);  // Resize to fit all content

    // Create pixmap and render table
    QPixmap pixmap(totalWidth, totalHeight);
    pixmap.fill(QColor(UIColors::BACKGROUND_VERY_DARK));

    QPainter painter(&pixmap);
    ui->timetableTable->render(&painter, QPoint(), QRegion(), QWidget::DrawChildren);
    painter.end();

    // Restore original properties
    ui->timetableTable->resize(originalSize);
    ui->timetableTable->setHorizontalScrollBarPolicy(oldHPolicy);
    ui->timetableTable->setVerticalScrollBarPolicy(oldVPolicy);

    // Save to file
    if (pixmap.save(fileName)) {
        UIDialogs::showInfo(this, "Success", "Full timetable (Monday to Sunday) saved successfully!");
    } else {
        UIDialogs::showWarning(this, "Error", "Failed to save timetable!");
    }
}

// Close timetable window
void TIMETABLE::onBack()
{
    this->close();
}

// Navigate to previous timetable combination
void TIMETABLE::onPrevPage()
{
    if (allCombinations.isEmpty()) return;

    currentCombinationIndex--;
    if (currentCombinationIndex < 0) {
        currentCombinationIndex = allCombinations.size() - 1;  // Wrap to last
    }

    displayCurrentCombination();
    updatePageLabel();
}

// Navigate to next timetable combination
void TIMETABLE::onNextPage()
{
    if (allCombinations.isEmpty()) return;

    currentCombinationIndex++;
    if (currentCombinationIndex >= allCombinations.size()) {
        currentCombinationIndex = 0;  // Wrap to first
    }

    displayCurrentCombination();
    updatePageLabel();
}

// Toggle between timetable pages
void TIMETABLE::onTogglePage()
{
    if (allCombinations.isEmpty() || allCombinations.size() <= 1) return;

    currentCombinationIndex++;
    if (currentCombinationIndex >= allCombinations.size()) {
        currentCombinationIndex = 0;  // Wrap to first
    }

    displayCurrentCombination();
    updatePageLabel();
}

// Close timetable window
void TIMETABLE::onDelete()
{
    this->close();
}

// Generate all possible timetable combinations from courses with multiple time options
void TIMETABLE::generateAllCombinations()
{
    HashTable<QString, LinkedList<Course>> courseGroups;  // Group courses by name

    // Group courses and remove duplicates
    for (int i = 0; i < coursesData.size(); ++i) {
        const Course &course = coursesData[i];

        bool found = false;
        if (courseGroups.contains(course.name)) {
            // Get reference to existing group using operator[] (avoids copy)
            LinkedList<Course> &existingGroup = courseGroups[course.name];
            for (int j = 0; j < existingGroup.size(); ++j) {
                const Course &existing = existingGroup[j];
                // Check if exact course already exists
                if (existing.day == course.day &&
                    existing.startTime == course.startTime &&
                    existing.endTime == course.endTime &&
                    existing.classroom == course.classroom) {
                    found = true;
                    break;
                }
            }

            // Append to existing group if not duplicate (modifies original via reference)
            if (!found) {
                existingGroup.append(course);
            }
        } else {
            // Create new group for new course name
            LinkedList<Course> newGroup;
            newGroup.append(course);
            courseGroups.insert(course.name, newGroup);
        }
    }

    // Check if any course has multiple time options
    bool hasMultipleOptions = false;
    LinkedList<QString> keys = courseGroups.keys();
    for (int i = 0; i < keys.size(); ++i) {
        if (courseGroups.value(keys[i]).size() > 1) {
            hasMultipleOptions = true;
            break;
        }
    }

    // If all courses are unique, create single combination
    if (!hasMultipleOptions) {
        LinkedList<Course> uniqueCourses;
        for (int i = 0; i < keys.size(); ++i) {
            LinkedList<Course> group = courseGroups.value(keys[i]);
            for (int j = 0; j < group.size(); ++j) {
                uniqueCourses.append(group[j]);
            }
        }
        allCombinations.append(uniqueCourses);
        return;
    }

    // Generate all combinations using recursion
    LinkedList<LinkedList<Course>> groups;
    for (int i = 0; i < keys.size(); ++i) {
        groups.append(courseGroups.value(keys[i]));
    }

    if (groups.isEmpty()) return;

    LinkedList<Course> currentCombination;
    generateCombinationsRecursive(groups, 0, currentCombination);
}

// Recursive function to generate all combinations (backtracking algorithm)
void TIMETABLE::generateCombinationsRecursive(const LinkedList<LinkedList<Course>> &groups,
                                               int groupIndex,
                                               LinkedList<Course> &currentCombination)
{
    if (groupIndex >= groups.size()) {  // Base case: all courses selected
        allCombinations.append(currentCombination);
        return;
    }

    // Try each time option for current course
    const LinkedList<Course> &currentGroup = groups[groupIndex];
    for (int i = 0; i < currentGroup.size(); ++i) {
        const Course &course = currentGroup[i];
        currentCombination.append(course);  // Choose
        generateCombinationsRecursive(groups, groupIndex + 1, currentCombination);  // Explore
        currentCombination.removeLast();  // Unchoose (backtrack)
    }
}

// Check if a combination has time conflicts
bool TIMETABLE::hasConflict(const LinkedList<Course> &combination)
{
    for (int i = 0; i < combination.size(); ++i) {
        for (int j = i + 1; j < combination.size(); ++j) {
            const Course &c1 = combination[i];
            const Course &c2 = combination[j];

            if (c1.day != c2.day) continue;  // Different days, no conflict

            int start1 = timeToColumn(c1.startTime);
            int end1 = timeToColumn(c1.endTime);
            int start2 = timeToColumn(c2.startTime);
            int end2 = timeToColumn(c2.endTime);

            if (!(end1 <= start2 || end2 <= start1)) {  // Time overlap detected
                return true;
            }
        }
    }
    return false;  // No conflicts
}

// Display the currently selected combination
void TIMETABLE::displayCurrentCombination()
{
    if (allCombinations.isEmpty() || currentCombinationIndex < 0 ||
        currentCombinationIndex >= allCombinations.size()) {
        return;
    }

    LinkedList<Course> originalData = coursesData;  // Save original
    coursesData = allCombinations[currentCombinationIndex];  // Switch to current combination

    populateTimetable();
    updateStatistics();

    coursesData = originalData;  // Restore original
}

// Update page label and navigation buttons
void TIMETABLE::updatePageLabel()
{
    if (!allCombinations.isEmpty()) {
        // Update page number display
        if (ui->pageNumberLabel) {
            ui->pageNumberLabel->setText(QString("%1/%2")
                                         .arg(currentCombinationIndex + 1)
                                         .arg(allCombinations.size()));
        }

        // Show/hide navigation buttons based on number of combinations
        if (allCombinations.size() > 1) {
            if (ui->prevPageBtn) ui->prevPageBtn->show();
            if (ui->nextPageBtn) ui->nextPageBtn->show();
            if (ui->pageNumberLabel) ui->pageNumberLabel->show();
        } else {
            if (ui->prevPageBtn) ui->prevPageBtn->hide();
            if (ui->nextPageBtn) ui->nextPageBtn->hide();
            if (ui->pageNumberLabel) ui->pageNumberLabel->hide();
        }

        // Update window title
        this->setWindowTitle(QString("View Timetable - Page %1 of %2")
                             .arg(currentCombinationIndex + 1)
                             .arg(allCombinations.size()));
    } else {
        if (ui->pageNumberLabel) {
            ui->pageNumberLabel->setText("1/1");
        }
        if (ui->prevPageBtn) ui->prevPageBtn->hide();
        if (ui->nextPageBtn) ui->nextPageBtn->hide();
        if (ui->pageNumberLabel) ui->pageNumberLabel->hide();
        this->setWindowTitle("View Timetable - No valid combinations");
    }
}
