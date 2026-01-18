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

    /* Add minimize/maximize buttons to title bar */
    this->setWindowFlags(Qt::Dialog | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    this->setWindowState(Qt::WindowMaximized);
    this->showMaximized();

    // Connect button signals to slots
    connect(ui->saveAsBtn, &QPushButton::clicked, this, &TIMETABLE::onSaveAs);
    connect(ui->backBtn, &QPushButton::clicked, this, &TIMETABLE::onBack);
    connect(ui->prevPageBtn, &QPushButton::clicked, this, &TIMETABLE::onPrevPage);
    connect(ui->nextPageBtn, &QPushButton::clicked, this, &TIMETABLE::onNextPage);
    connect(ui->deleteBtn, &QPushButton::clicked, this, &TIMETABLE::onDelete);

    // Fix misleading button name: "Delete" -> "Close" (it just closes the window)
    if (ui->deleteBtn) {
        ui->deleteBtn->setText("Close");
    }

    // Configure timetable table properties
    if (ui->timetableTable) {
        ui->timetableTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // Disable editing

        /* Set fixed column width (130px) so adjacent courses don't overlap */
        ui->timetableTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        ui->timetableTable->horizontalHeader()->setDefaultSectionSize(130);  // Each time slot = 130px
        for (int col = 0; col < ui->timetableTable->columnCount(); ++col) {
            ui->timetableTable->setColumnWidth(col, 130);  // Set each column to 130px
        }

        ui->timetableTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);  // Enable horizontal scroll
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

    // Clear all existing spans first (important!)
    for (int row = 0; row < ui->timetableTable->rowCount(); ++row) {
        for (int col = 0; col < ui->timetableTable->columnCount(); ++col) {
            ui->timetableTable->setSpan(row, col, 1, 1);  // Reset to single cell
        }
    }

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

    // Remove spaces
    t = t.replace(" ", "");

    bool isPM = t.contains("pm");

    QString numStr = t;
    // Remove time suffixes and decimal points
    numStr.remove("am").remove("pm").remove(".00").remove(".");

    // Handle empty string after removal
    if (numStr.isEmpty()) {
        return -1;
    }

    bool ok;
    int hour = numStr.toInt(&ok);

    // Check if conversion was successful
    if (!ok) {
        return -1;
    }

    // Convert 12-hour format to 24-hour format
    if (isPM && hour != 12) {
        hour += 12;  // e.g., 2pm -> 14
    } else if (!isPM && hour == 12) {
        hour = 0;  // 12am -> 0
    }

    if (hour >= 8 && hour <= 22) {  // Changed from 21 to 22 to include 10pm
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

// Hide timetable window (can be shown again with View Timetable)
void TIMETABLE::onBack()
{
    this->hide();  // Just hide, don't close - can view again
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

// Close timetable window (renamed from onDelete for clarity)
void TIMETABLE::onDelete()
{
    this->close();  // This function just closes the window, not deletes anything
}

/**
 * ============================================================================
 * TIMETABLE COMBINATION GENERATION
 * ============================================================================
 *
 * This section generates all possible timetable arrangements when courses
 * have multiple time options (e.g., "Math" can be Monday 8am or Tuesday 2pm).
 *
 * Algorithm: RECURSIVE BACKTRACKING
 * Data Structures: HashTable (grouping), LinkedList (storage)
 *
 * ============================================================================
 */

/* Maximum combinations - set to large value to allow all combinations */
static const int MAX_COMBINATIONS = 10000;

/**
 * Generate All Timetable Combinations
 *
 * Step 1: Group courses by name using HashTable
 * Step 2: Check if any course has multiple time options
 * Step 3: If yes, use recursive backtracking to generate all combinations
 * Step 4: Sort combinations (non-conflicting first)
 */
void TIMETABLE::generateAllCombinations()
{
    HashTable<QString, LinkedList<Course>> courseGroups;  // Group courses by name

    // Group courses and remove duplicates (case-insensitive grouping)
    for (int i = 0; i < coursesData.size(); ++i) {
        const Course &course = coursesData[i];
        QString nameKey = course.name.toLower();  // Case-insensitive: "Math" == "math"

        bool found = false;
        if (courseGroups.contains(nameKey)) {
            // Get reference to existing group using operator[] (avoids copy)
            LinkedList<Course> &existingGroup = courseGroups[nameKey];
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
            courseGroups.insert(nameKey, newGroup);
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

    /**
     * Sort combinations by priority:
     * 1. No conflicts first (conflicts = 0)
     * 2. Among no-conflict ones, sort by total hours (highest first)
     * 3. Conflicting ones at the end, also sorted by hours
     */
    LinkedList<LinkedList<Course>> nonConflicting;
    LinkedList<LinkedList<Course>> conflicting;

    for (int i = 0; i < allCombinations.size(); ++i) {
        if (hasConflict(allCombinations[i])) {
            conflicting.append(allCombinations[i]);
        } else {
            nonConflicting.append(allCombinations[i]);
        }
    }

    /* Sort non-conflicting by total hours (highest first) using simple bubble sort */
    for (int i = 0; i < nonConflicting.size() - 1; ++i) {
        for (int j = 0; j < nonConflicting.size() - 1 - i; ++j) {
            int hours1 = 0, hours2 = 0;
            LinkedList<Course> combo1 = nonConflicting[j];
            LinkedList<Course> combo2 = nonConflicting[j + 1];

            for (int k = 0; k < combo1.size(); ++k) {
                int start = timeToColumn(combo1[k].startTime);
                int end = timeToColumn(combo1[k].endTime);
                if (start >= 0 && end >= 0) hours1 += (end - start);
            }
            for (int k = 0; k < combo2.size(); ++k) {
                int start = timeToColumn(combo2[k].startTime);
                int end = timeToColumn(combo2[k].endTime);
                if (start >= 0 && end >= 0) hours2 += (end - start);
            }

            /* Swap if hours2 > hours1 (we want highest hours first) */
            if (hours2 > hours1) {
                nonConflicting[j] = combo2;
                nonConflicting[j + 1] = combo1;
            }
        }
    }

    /* Rebuild allCombinations: best (no conflict, most hours) first */
    allCombinations.clear();
    for (int i = 0; i < nonConflicting.size(); ++i) {
        allCombinations.append(nonConflicting[i]);
    }
    for (int i = 0; i < conflicting.size(); ++i) {
        allCombinations.append(conflicting[i]);
    }
}

/**
 * ============================================================================
 * RECURSIVE BACKTRACKING ALGORITHM
 * ============================================================================
 *
 * This is the core algorithm for generating all timetable combinations.
 *
 * Time Complexity: O(n^m) where n = options per course, m = number of courses
 * Space Complexity: O(m) for recursion stack
 *
 * How it works:
 * 1. BASE CASE: If all groups processed, save current combination
 * 2. RECURSIVE CASE: For each option in current group:
 *    a. CHOOSE: Add course to current combination
 *    b. EXPLORE: Recursively process next group
 *    c. UNCHOOSE: Remove course (backtrack) to try next option
 *
 * Example with 2 courses, each with 2 time options:
 * - Math: [Mon 8am, Tue 2pm]
 * - Science: [Wed 10am, Thu 4pm]
 *
 * Generates combinations:
 * 1. Math(Mon 8am) + Science(Wed 10am)
 * 2. Math(Mon 8am) + Science(Thu 4pm)
 * 3. Math(Tue 2pm) + Science(Wed 10am)
 * 4. Math(Tue 2pm) + Science(Thu 4pm)
 *
 * @param groups - List of course groups (each group has multiple time options)
 * @param groupIndex - Current group being processed
 * @param currentCombination - Current combination being built
 * ============================================================================
 */
void TIMETABLE::generateCombinationsRecursive(const LinkedList<LinkedList<Course>> &groups,
                                               int groupIndex,
                                               LinkedList<Course> &currentCombination)
{
    // Limit check: Prevent exponential explosion
    if (allCombinations.size() >= MAX_COMBINATIONS) {
        return;
    }

    // BASE CASE: All groups have been processed
    if (groupIndex >= groups.size()) {
        // Save the complete combination
        allCombinations.append(currentCombination);
        return;
    }

    // RECURSIVE CASE: Try each option in current group
    const LinkedList<Course> &currentGroup = groups[groupIndex];
    for (int i = 0; i < currentGroup.size(); ++i) {
        // Check limit before recursing
        if (allCombinations.size() >= MAX_COMBINATIONS) {
            return;
        }

        const Course &course = currentGroup[i];

        // STEP 1: CHOOSE - Add this course option to current combination
        currentCombination.append(course);

        // STEP 2: EXPLORE - Recursively process next group
        generateCombinationsRecursive(groups, groupIndex + 1, currentCombination);

        // STEP 3: UNCHOOSE (BACKTRACK) - Remove course to try next option
        currentCombination.removeLast();
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

/**
 * Display the currently selected combination
 * Updates the timetable grid and statistics for the current combination
 */
void TIMETABLE::displayCurrentCombination()
{
    if (allCombinations.isEmpty() || currentCombinationIndex < 0 ||
        currentCombinationIndex >= allCombinations.size()) {
        return;
    }

    /* Save original data */
    LinkedList<Course> originalData = coursesData;

    /* Clear and rebuild coursesData from current combination */
    coursesData.clear();
    LinkedList<Course>& combo = allCombinations[currentCombinationIndex];
    for (int i = 0; i < combo.size(); ++i) {
        coursesData.append(combo[i]);
    }

    /* Display the timetable grid */
    populateTimetable();

    /* Calculate statistics - count directly from combo */
    int totalCourses = combo.size();

    /* Calculate total hours */
    int totalHours = 0;
    for (int i = 0; i < combo.size(); ++i) {
        int start = timeToColumn(combo[i].startTime);
        int end = timeToColumn(combo[i].endTime);
        if (start >= 0 && end >= 0) {
            totalHours += (end - start);
        }
    }

    /* Count conflicts */
    int conflicts = 0;
    for (int i = 0; i < combo.size(); ++i) {
        for (int j = i + 1; j < combo.size(); ++j) {
            if (combo[i].day != combo[j].day) continue;

            int start1 = timeToColumn(combo[i].startTime);
            int end1 = timeToColumn(combo[i].endTime);
            int start2 = timeToColumn(combo[j].startTime);
            int end2 = timeToColumn(combo[j].endTime);

            if (start1 >= 0 && end1 >= 0 && start2 >= 0 && end2 >= 0) {
                if (start1 < end2 && start2 < end1) {
                    conflicts++;
                }
            }
        }
    }

    /* Update UI labels */
    if (ui->totalCourseLabel) {
        ui->totalCourseLabel->setText(QString("Total Course: %1").arg(totalCourses));
    }
    if (ui->totalHoursLabel) {
        /* Blue color for Total Hours - indicates dynamic value */
        ui->totalHoursLabel->setText(QString("Total Hours: %1").arg(totalHours));
        ui->totalHoursLabel->setStyleSheet("QLabel { color: #3498db; font: bold 18pt \"Segoe UI\"; background-color: transparent; padding: 10px; }");
    }
    if (ui->conflictsLabel) {
        if (conflicts == 0) {
            /* Green for No conflicts - good, can save */
            ui->conflictsLabel->setText("Conflicts: No");
            ui->conflictsLabel->setStyleSheet("QLabel { color: #27ae60; font: bold 18pt \"Segoe UI\"; background-color: transparent; padding: 10px; }");
        } else {
            /* Red for Yes conflicts - bad, cannot use */
            ui->conflictsLabel->setText("Conflicts: Yes");
            ui->conflictsLabel->setStyleSheet("QLabel { color: #e74c3c; font: bold 18pt \"Segoe UI\"; background-color: transparent; padding: 10px; }");
        }
    }

    /* Restore original data for future operations */
    coursesData = originalData;
}

/**
 * Update page label and navigation buttons
 */
void TIMETABLE::updatePageLabel()
{
    if (!allCombinations.isEmpty()) {
        /* Update page number display - always show */
        if (ui->pageNumberLabel) {
            ui->pageNumberLabel->setText(QString("%1/%2")
                                         .arg(currentCombinationIndex + 1)
                                         .arg(allCombinations.size()));
            ui->pageNumberLabel->show();
        }

        /* Show/hide navigation buttons based on number of combinations */
        if (allCombinations.size() > 1) {
            if (ui->prevPageBtn) ui->prevPageBtn->show();
            if (ui->nextPageBtn) ui->nextPageBtn->show();
        } else {
            if (ui->prevPageBtn) ui->prevPageBtn->hide();
            if (ui->nextPageBtn) ui->nextPageBtn->hide();
        }

        /* Update window title */
        this->setWindowTitle(QString("View Timetable - Page %1 of %2")
                             .arg(currentCombinationIndex + 1)
                             .arg(allCombinations.size()));
    } else {
        if (ui->pageNumberLabel) {
            ui->pageNumberLabel->setText("1/1");
            ui->pageNumberLabel->show();
        }
        if (ui->prevPageBtn) ui->prevPageBtn->hide();
        if (ui->nextPageBtn) ui->nextPageBtn->hide();
        this->setWindowTitle("View Timetable");
    }
}
