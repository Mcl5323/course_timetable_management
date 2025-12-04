#include "timetable.h"
#include "ui_timetable.h"
#include "managecoursespage.h"
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

    // Set window properties - force full screen
    this->setWindowTitle("View Timetable");
    this->setWindowFlags(Qt::Window);  // Set as normal window (not dialog)
    this->setWindowState(Qt::WindowMaximized);  // Force maximize window
    this->showMaximized();  // Maximize window

    // Setup button connections
    connect(ui->saveAsBtn, &QPushButton::clicked, this, &TIMETABLE::onSaveAs);
    connect(ui->backBtn, &QPushButton::clicked, this, &TIMETABLE::onBack);
    connect(ui->nextPageBtn, &QPushButton::clicked, this, &TIMETABLE::onTogglePage);
    connect(ui->deleteBtn, &QPushButton::clicked, this, &TIMETABLE::onDelete);

    // Hide the previous page button (we'll use only next button to toggle)
    if (ui->prevPageBtn) {
        ui->prevPageBtn->hide();
    }

    // Initialize timetable table
    if (ui->timetableTable) {
        ui->timetableTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

        // Configure column sizing - stretch to fill window
        ui->timetableTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->timetableTable->horizontalHeader()->setStretchLastSection(true);
        ui->timetableTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        // Enable vertical scrolling to see all 7 days
        ui->timetableTable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        // Set row height - increased for better text display
        for (int row = 0; row < ui->timetableTable->rowCount(); ++row) {
            ui->timetableTable->setRowHeight(row, 80);
        }

        // Enable word wrapping to show all text
        ui->timetableTable->setWordWrap(true);
        ui->timetableTable->setTextElideMode(Qt::ElideNone);
    }
}

TIMETABLE::~TIMETABLE()
{
    delete ui;
}

// This gets called when user clicks "Generate Timetable" button
// Main job: take the courses and figure out all possible non-conflicting schedules
void TIMETABLE::setCoursesData(const QVector<Course> &courses)
{
    coursesData = courses;  // store the courses locally
    allCombinations.clear();  // clear any old combinations
    currentCombinationIndex = 0;  // start from first page

    // Generate all possible non-conflicting combinations
    generateAllCombinations();

    // Display the first combination if any exist
    if (!allCombinations.isEmpty()) {
        displayCurrentCombination();
        updatePageLabel();
    } else {
        // if no combinations found, just show all courses (might have conflicts)
        populateTimetable();
        updateStatistics();
        updatePageLabel();
    }
}

void TIMETABLE::populateTimetable()
{
    if (!ui->timetableTable) return;

    // Clear existing content
    ui->timetableTable->clearContents();

    // Use single default deep blue color for all courses
    QColor defaultColor("#2d5a8c");

    // Process each course and create spanning cells
    for (const Course &course : coursesData) {
        int row = dayToRow(course.day);
        int startCol = timeToColumn(course.startTime);
        int endCol = timeToColumn(course.endTime);

        if (row < 0 || startCol < 0 || endCol < 0) continue;
        if (startCol >= endCol) continue; // Invalid time range

        // Calculate span duration
        int colSpan = endCol - startCol;

        // Create the main cell with full course information - compact format
        QTableWidgetItem *mainItem = new QTableWidgetItem(
            QString("%1\n%2\n%3-%4")
                .arg(course.name)
                .arg(course.classroom)
                .arg(course.startTime)
                .arg(course.endTime)
        );

        mainItem->setBackground(QBrush(defaultColor));
        mainItem->setForeground(QBrush(QColor("#FFFFFF")));
        mainItem->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);

        // Enable word wrapping for this specific item
        mainItem->setData(Qt::TextWordWrap, true);

        // Smaller font size to fit more content
        QFont font = mainItem->font();
        font.setBold(true);
        font.setPointSize(8);  // Reduced from 10 to 8
        mainItem->setFont(font);

        // Set the item at the starting position
        ui->timetableTable->setItem(row, startCol, mainItem);

        // If course spans multiple hours, merge cells
        if (colSpan > 1) {
            ui->timetableTable->setSpan(row, startCol, 1, colSpan);
        }
    }
}

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

int TIMETABLE::calculateTotalHours()
{
    int total = 0;
    for (const Course &course : coursesData) {
        int start = timeToColumn(course.startTime);
        int end = timeToColumn(course.endTime);
        if (start >= 0 && end >= 0) {
            total += (end - start);
        }
    }
    return total;
}

int TIMETABLE::detectConflicts()
{
    int conflicts = 0;

    // Check every pair of courses for time conflicts
    for (int i = 0; i < coursesData.size(); ++i) {
        for (int j = i + 1; j < coursesData.size(); ++j) {
            const Course &course1 = coursesData[i];
            const Course &course2 = coursesData[j];

            // Only check courses on the same day
            if (course1.day != course2.day) {
                continue;
            }

            int start1 = timeToColumn(course1.startTime);
            int end1 = timeToColumn(course1.endTime);
            int start2 = timeToColumn(course2.startTime);
            int end2 = timeToColumn(course2.endTime);

            if (start1 < 0 || end1 < 0 || start2 < 0 || end2 < 0) {
                continue;
            }

            // Check if time ranges overlap
            // Two ranges overlap if: start1 < end2 AND start2 < end1
            if (start1 < end2 && start2 < end1) {
                conflicts++;
            }
        }
    }

    return conflicts;
}

// converts time string (like "8am", "2pm") into column number for the table
// basically maps time to table column position
int TIMETABLE::timeToColumn(const QString &time)
{
    QString t = time.toLower().trimmed();

    int hour = 0;
    bool isPM = t.contains("pm");

    // clean up the string - remove am/pm and .00
    QString numStr = t;
    numStr.remove("am").remove("pm").remove(".00");
    hour = numStr.toInt();

    // handle 12 hour to 24 hour conversion
    if (isPM && hour != 12) {
        hour += 12;  // 2pm becomes 14
    } else if (!isPM && hour == 12) {
        hour = 0;  // 12am is actually 0 (midnight)
    }

    // our timetable starts at 8am, so 8am = column 0, 9am = column 1, etc
    if (hour >= 8 && hour <= 21) {
        return hour - 8;
    }

    return -1; // something went wrong, time not in range
}

int TIMETABLE::dayToRow(const QString &day)
{
    QMap<QString, int> dayMap;
    dayMap["Monday"] = 0;
    dayMap["Tuesday"] = 1;
    dayMap["Wednesday"] = 2;
    dayMap["Thursday"] = 3;
    dayMap["Friday"] = 4;
    dayMap["Saturday"] = 5;
    dayMap["Sunday"] = 6;

    return dayMap.value(day, -1);
}

void TIMETABLE::onSaveAs()
{
    if (!ui->timetableTable) {
        QMessageBox::warning(this, "Error", "Timetable table not found!");
        return;
    }

    // Open file dialog to choose save location
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save Timetable As",
        QDir::homePath() + "/timetable.png",
        "PNG Image (*.png);;JPEG Image (*.jpg);;All Files (*.*)"
    );

    if (fileName.isEmpty()) {
        return; // User cancelled
    }

    // Save original geometry and policies
    QSize originalSize = ui->timetableTable->size();
    Qt::ScrollBarPolicy oldHPolicy = ui->timetableTable->horizontalScrollBarPolicy();
    Qt::ScrollBarPolicy oldVPolicy = ui->timetableTable->verticalScrollBarPolicy();

    // Temporarily disable scrollbars
    ui->timetableTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->timetableTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Calculate exact size needed for all content (all 7 days)
    int totalWidth = ui->timetableTable->verticalHeader()->width();
    for (int col = 0; col < ui->timetableTable->columnCount(); ++col) {
        totalWidth += ui->timetableTable->columnWidth(col);
    }

    int totalHeight = ui->timetableTable->horizontalHeader()->height();
    for (int row = 0; row < ui->timetableTable->rowCount(); ++row) {
        totalHeight += ui->timetableTable->rowHeight(row);
    }

    // Temporarily resize table to fit all content
    ui->timetableTable->resize(totalWidth, totalHeight);

    // Create pixmap with exact table size
    QPixmap pixmap(totalWidth, totalHeight);
    pixmap.fill(QColor("#0D1B2A"));  // Use table background color

    // Render only the table content
    QPainter painter(&pixmap);
    ui->timetableTable->render(&painter, QPoint(), QRegion(), QWidget::DrawChildren);
    painter.end();

    // Restore original size and scrollbar policies
    ui->timetableTable->resize(originalSize);
    ui->timetableTable->setHorizontalScrollBarPolicy(oldHPolicy);
    ui->timetableTable->setVerticalScrollBarPolicy(oldVPolicy);

    // Save the pixmap to file
    if (pixmap.save(fileName)) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Success");
        msgBox.setText("Full timetable (Monday to Sunday) saved successfully!");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("QMessageBox{background-color: #ffffff;} QLabel{color: #000000; font-size: 11px; background-color: transparent;} QPushButton{background-color: #e0e0e0; color: #000000; font-size: 11px; min-width: 60px; padding: 5px;}");
        msgBox.exec();
    } else {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Error");
        msgBox.setText("Failed to save timetable!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QMessageBox{background-color: #ffffff;} QLabel{color: #000000; font-size: 11px; background-color: transparent;} QPushButton{background-color: #e0e0e0; color: #000000; font-size: 11px; min-width: 60px; padding: 5px;}");
        msgBox.exec();
    }
}

void TIMETABLE::onBack()
{
    this->close();
}

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

void TIMETABLE::onTogglePage()
{
    if (allCombinations.isEmpty() || allCombinations.size() <= 1) return;

    // Toggle between pages
    currentCombinationIndex++;
    if (currentCombinationIndex >= allCombinations.size()) {
        currentCombinationIndex = 0;  // Wrap back to first
    }

    displayCurrentCombination();
    updatePageLabel();

    // Update button text to show what's next
    if (ui->nextPageBtn) {
        int nextPage = currentCombinationIndex + 1;
        if (nextPage >= allCombinations.size()) {
            nextPage = 0;
        }
        ui->nextPageBtn->setText(QString("Next (%1/%2)").arg(nextPage + 1).arg(allCombinations.size()));
    }
}

void TIMETABLE::onDelete()
{
    // Confirm before clearing timetable view
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Confirm Clear View");
    msgBox.setText("Are you sure you want to clear this timetable view?\n\nNote: Your courses in Manage Courses page will not be deleted.");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setStyleSheet("QMessageBox{background-color: #ffffff;} QLabel{color: #000000; font-size: 11px; background-color: transparent;} QPushButton{background-color: #e0e0e0; color: #000000; font-size: 11px; min-width: 60px; padding: 5px;}");

    if (msgBox.exec() == QMessageBox::Yes) {
        // Clear local timetable data only (does not affect ManageCoursesPage)
        coursesData.clear();
        allCombinations.clear();

        // Refresh display
        populateTimetable();
        updateStatistics();

        QMessageBox confirmBox(this);
        confirmBox.setWindowTitle("Success");
        confirmBox.setText("Timetable view cleared!\n\nYour courses are still saved in Manage Courses page.");
        confirmBox.setIcon(QMessageBox::Information);
        confirmBox.setStyleSheet("QMessageBox{background-color: #ffffff;} QLabel{color: #000000; font-size: 11px; background-color: transparent;} QPushButton{background-color: #e0e0e0; color: #000000; font-size: 11px; min-width: 60px; padding: 5px;}");
        confirmBox.exec();

        // Close timetable window
        this->close();
    }
}

// Main function that generates all valid timetable combinations
// For example: if TDS1123 has 3 time slots and MPU3113 has 2 slots,
// this will create 6 possible timetables (3 x 2) and filter out any with conflicts
void TIMETABLE::generateAllCombinations()
{
    // first, group courses by their name
    // so all TDS1123 options go together, all MPU3113 options go together, etc
    QMap<QString, QVector<Course>> courseGroups;
    for (const Course &course : coursesData) {
        courseGroups[course.name].append(course);
    }

    // convert the map to a list so we can work with it easier
    QList<QVector<Course>> groups;
    for (auto it = courseGroups.begin(); it != courseGroups.end(); ++it) {
        groups.append(it.value());
    }

    if (groups.isEmpty()) return;

    // use recursion to try all combinations
    // starts with empty combination and builds from there
    QVector<Course> currentCombination;
    generateCombinationsRecursive(groups, 0, currentCombination);

    // no artificial limit - will generate as many as needed
}

// This is where the magic happens - recursively builds all combinations
// Think of it like a decision tree: for each course, try all its time options
void TIMETABLE::generateCombinationsRecursive(const QList<QVector<Course>> &groups,
                                               int groupIndex,
                                               QVector<Course> &currentCombination)
{
    // done! we've picked one option for each course
    if (groupIndex >= groups.size()) {
        // only save it if there's no time conflicts
        if (!hasConflict(currentCombination)) {
            allCombinations.append(currentCombination);
        }
        return;
    }

    // try each possible time slot for this course
    const QVector<Course> &currentGroup = groups[groupIndex];
    for (const Course &course : currentGroup) {
        currentCombination.append(course);  // try this option

        // recursively fill in the rest of the courses
        generateCombinationsRecursive(groups, groupIndex + 1, currentCombination);

        // backtrack - undo this choice and try the next option
        currentCombination.removeLast();
    }
}

// Check if a combination of courses has any time conflicts
bool TIMETABLE::hasConflict(const QVector<Course> &combination)
{
    for (int i = 0; i < combination.size(); ++i) {
        for (int j = i + 1; j < combination.size(); ++j) {
            const Course &c1 = combination[i];
            const Course &c2 = combination[j];

            // Check if same day
            if (c1.day != c2.day) continue;

            // Convert times to columns
            int start1 = timeToColumn(c1.startTime);
            int end1 = timeToColumn(c1.endTime);
            int start2 = timeToColumn(c2.startTime);
            int end2 = timeToColumn(c2.endTime);

            // Check for time overlap
            if (!(end1 <= start2 || end2 <= start1)) {
                return true;  // Conflict found
            }
        }
    }
    return false;  // No conflicts
}

// Display the current combination on the timetable
void TIMETABLE::displayCurrentCombination()
{
    if (allCombinations.isEmpty() || currentCombinationIndex < 0 ||
        currentCombinationIndex >= allCombinations.size()) {
        return;
    }

    // Temporarily set coursesData to the current combination
    QVector<Course> originalData = coursesData;
    coursesData = allCombinations[currentCombinationIndex];

    // Populate the timetable with this combination
    populateTimetable();

    // Update statistics for current combination (use the current combination data)
    updateStatistics();

    // Restore original data AFTER updating statistics
    coursesData = originalData;
}

// Update the page label to show current page
void TIMETABLE::updatePageLabel()
{
    if (!allCombinations.isEmpty()) {
        // Update the page number label
        if (ui->pageNumberLabel) {
            ui->pageNumberLabel->setText(QString("%1/%2")
                                         .arg(currentCombinationIndex + 1)
                                         .arg(allCombinations.size()));
        }

        // Update window title
        this->setWindowTitle(QString("View Timetable - Page %1 of %2")
                             .arg(currentCombinationIndex + 1)
                             .arg(allCombinations.size()));
    } else {
        if (ui->pageNumberLabel) {
            ui->pageNumberLabel->setText("1/1");
        }
        this->setWindowTitle("View Timetable - No valid combinations");
    }
}
