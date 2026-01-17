/**
 * ============================================================================
 * Manage Courses Page Implementation (managecoursespage.cpp)
 * ============================================================================
 *
 * This file implements the main course management interface where users can:
 * - Add, edit, and delete courses (CRUD operations)
 * - Search courses using LINEAR SEARCH algorithm
 * - Sort courses using QUICKSORT algorithm
 * - Generate timetable combinations
 * - Import/Export courses to/from CSV files
 *
 * Key Algorithms:
 * - Linear Search: O(n) - searches through all courses for keyword matches
 * - QuickSort: O(n log n) - sorts courses by name/day/time/classroom
 *
 * ============================================================================
 */

#include "managecoursespage.h"
#include "ui_managecoursespage.h"
#include "mainwindow.h"
#include "timetable.h"
#include "loadingdialog.h"
#include "uistyles.h"
#include <QMessageBox>
#include <QPushButton>
#include <QCheckBox>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QCoreApplication>
#include <QFileDialog>

// Constructor - Initialize course management page
// Note: Initialization order must match declaration order in header file
ManageCoursesPage::ManageCoursesPage(const QString &currentUser, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ManageCoursesPage)
    , isSearching(false)
    , currentUser(currentUser)
    , editingRow(-1)
    , selectedRow(-1)
    , timetableWindow(nullptr)
    , loadingDialog(nullptr)
    , timetableGenerated(false) {

    ui->setupUi(this);
    this->setWindowTitle("Manage Courses");
    this->setWindowState(Qt::WindowMaximized);
    this->showMaximized();
    this->setStyleSheet("background-color: " + UIColors::BACKGROUND_DARK_BLUE_GRAY);

    if (ui->dayCombo) {  // Setup day selection
        ui->dayCombo->addItems({"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"});
    }

    if (ui->startTimeLabel) {  // Setup start time (8am to 10pm)
        QStringList hours;
        for (int i = 8; i <= 11; ++i) hours << QString("%1am").arg(i);
        hours << "12pm";
        for (int i = 1; i <= 10; ++i) hours << QString("%1pm").arg(i);
        ui->startTimeLabel->addItems(hours);
    }

    if (ui->endTimeInput) {  // Setup end time (8am to 10pm)
        QStringList hours;
        for (int i = 8; i <= 11; ++i) hours << QString("%1am").arg(i);
        hours << "12pm";
        for (int i = 1; i <= 10; ++i) hours << QString("%1pm").arg(i);
        ui->endTimeInput->addItems(hours);
    }

    if (ui->coursetable) {  // Setup course table
        ui->coursetable->setColumnCount(6);
        ui->coursetable->setHorizontalHeaderLabels({"Select", "Course Name", "Day", "Time", "Classroom", "Actions"});
        ui->coursetable->setRowCount(0);
        ui->coursetable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->coursetable->setColumnWidth(0, 80);
        ui->coursetable->setColumnWidth(1, 180);
        ui->coursetable->setColumnWidth(2, 120);
        ui->coursetable->setColumnWidth(3, 120);
        ui->coursetable->setColumnWidth(4, 120);
        ui->coursetable->setColumnWidth(5, 180);
        ui->coursetable->setMaximumHeight(180);  // Limit height to 180px, enable scrolling
        ui->coursetable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->coursetable->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->coursetable->horizontalHeader()->setStyleSheet(UIStyles::tableHeaderBlue());
        ui->coursetable->setStyleSheet(UIStyles::tableWidgetWhite());
        ui->coursetable->setAlternatingRowColors(false);
    }

    if (ui->logupbutton) {  // Setup logout button (lambda function)
        connect(ui->logupbutton, &QPushButton::clicked, this, [this, parent]() {
            this->hide();
            MainWindow *mainWindow = qobject_cast<MainWindow*>(parent);
            if (mainWindow) {
                mainWindow->switchToLoginPage();
            }
        });
    }

    // Create Search label (ABOVE TABLE - Y=500)
    QLabel *searchSortLabel = new QLabel("Search:", this);
    searchSortLabel->setGeometry(170, 502, 60, 25);
    searchSortLabel->setStyleSheet("font: bold 10pt \"Segoe UI\"; color: #FFFFFF; background-color: transparent;");
    searchSortLabel->show();
    searchSortLabel->raise();

    // Create search & sort UI components (Y=500, same row as label)
    searchInput = new QLineEdit(this);
    searchInput->setPlaceholderText("Search courses...");
    searchInput->setGeometry(240, 498, 280, 30);
    searchInput->setStyleSheet("QLineEdit { background-color: #FFFFFF; color: #000000; border: 2px solid #3498db; border-radius: 4px; padding: 5px; font-size: 11px; }");
    searchInput->show();
    searchInput->raise();

    searchBtn = new QPushButton("Search", this);
    searchBtn->setGeometry(530, 498, 70, 30);
    searchBtn->setStyleSheet("QPushButton { background-color: #3498db; color: white; border: none; border-radius: 4px; padding: 5px; font-size: 11px; font-weight: bold; } QPushButton:hover { background-color: #2980b9; }");
    searchBtn->show();
    searchBtn->raise();

    clearSearchBtn = new QPushButton("Clear", this);
    clearSearchBtn->setGeometry(605, 498, 60, 30);
    clearSearchBtn->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; border: none; border-radius: 4px; padding: 5px; font-size: 11px; font-weight: bold; } QPushButton:hover { background-color: #7f8c8d; }");
    clearSearchBtn->show();
    clearSearchBtn->raise();

    // Sort label and combo
    QLabel *sortLabel = new QLabel("Sort:", this);
    sortLabel->setGeometry(680, 502, 40, 25);
    sortLabel->setStyleSheet("font: bold 10pt \"Segoe UI\"; color: #FFFFFF; background-color: transparent;");
    sortLabel->show();
    sortLabel->raise();

    sortCombo = new QComboBox(this);
    sortCombo->setGeometry(720, 498, 180, 30);
    sortCombo->addItems({"Sort by...", "Name (A-Z)", "Day (Mon-Sun)", "Time (Early-Late)", "Classroom (A-Z)"});
    sortCombo->setStyleSheet("QComboBox { background-color: #FFFFFF; color: #000000; border: 2px solid #2ecc71; border-radius: 4px; padding: 5px; font-size: 11px; }");
    sortCombo->show();
    sortCombo->raise();

    // Create Import/Export buttons
    exportBtn = new QPushButton("Export", this);
    exportBtn->setGeometry(910, 498, 70, 30);
    exportBtn->setStyleSheet("QPushButton { background-color: #2ecc71; color: white; border: none; border-radius: 4px; padding: 5px; font-size: 11px; font-weight: bold; } QPushButton:hover { background-color: #27ae60; }");
    exportBtn->show();
    exportBtn->raise();

    importBtn = new QPushButton("Import", this);
    importBtn->setGeometry(985, 498, 70, 30);
    importBtn->setStyleSheet("QPushButton { background-color: #3498db; color: white; border: none; border-radius: 4px; padding: 5px; font-size: 11px; font-weight: bold; } QPushButton:hover { background-color: #2980b9; }");
    importBtn->show();
    importBtn->raise();

    // Delete All button - red color to indicate danger action
    deleteAllBtn = new QPushButton("Delete All", this);
    deleteAllBtn->setGeometry(1060, 498, 80, 30);
    deleteAllBtn->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; border: none; border-radius: 4px; padding: 5px; font-size: 11px; font-weight: bold; } QPushButton:hover { background-color: #c0392b; }");
    deleteAllBtn->show();
    deleteAllBtn->raise();

    setupConnections();  // Connect all signals to slots
    loadCoursesFromFile();  // Load saved courses

    // Disable View Timetable button until Generate is clicked
    if (ui->viewTimetableBtn) {
        ui->viewTimetableBtn->setEnabled(false);
        ui->viewTimetableBtn->setToolTip("Please click 'Generate Timetable' first");
    }
}

// Destructor - Clean up UI components
ManageCoursesPage::~ManageCoursesPage() {
    delete ui;
    if (timetableWindow) delete timetableWindow;
    if (loadingDialog) delete loadingDialog;
}

// Convert time string to 24-hour int (e.g., "2pm" -> 14) for comparison
int ManageCoursesPage::timeToInt(const QString &time) {
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
        hour += 12;  // 2pm becomes 14
    } else if (!isPM && hour == 12) {
        hour = 0;  // midnight edge case
    }

    return hour;
}

// Setup all signal-slot connections
void ManageCoursesPage::setupConnections() {
    if (ui->addCourseBtn) {
        connect(ui->addCourseBtn, &QPushButton::clicked, this, &ManageCoursesPage::onAddCourse);
    }
    if (ui->generateBtn) {
        connect(ui->generateBtn, &QPushButton::clicked, this, &ManageCoursesPage::onGenerateTimetable);
    }
    if (ui->viewTimetableBtn) {
        connect(ui->viewTimetableBtn, &QPushButton::clicked, this, &ManageCoursesPage::onViewTimetable);
    }
    connect(searchBtn, &QPushButton::clicked, this, &ManageCoursesPage::onSearchCourse);
    connect(clearSearchBtn, &QPushButton::clicked, this, &ManageCoursesPage::onClearSearch);
    connect(sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ManageCoursesPage::onSortCourses);
    connect(exportBtn, &QPushButton::clicked, this, &ManageCoursesPage::onExportCourses);
    connect(importBtn, &QPushButton::clicked, this, &ManageCoursesPage::onImportCourses);
    connect(deleteAllBtn, &QPushButton::clicked, this, &ManageCoursesPage::onDeleteAllCourses);
}

// Add or update course (editingRow == -1: add mode, >= 0: edit mode)
void ManageCoursesPage::onAddCourse() {
    QString name = ui->courseNameInput->text().trimmed();
    QString day = ui->dayCombo->currentText();
    QString startTime = ui->startTimeLabel->currentText();
    QString endTime = ui->endTimeInput->currentText();
    QString classroom = ui->classroomInput->text().trimmed();

    // Validate all fields
    if (name.isEmpty()) {
        UIDialogs::showWarning(this, "Invalid Input", "Please enter course name!");
        ui->courseNameInput->setFocus();
        return;
    }
    if (name.contains("|")) {
        UIDialogs::showWarning(this, "Invalid Input", "Course name cannot contain the '|' character!");
        ui->courseNameInput->setFocus();
        return;
    }
    if (name.length() > 100) {
        UIDialogs::showWarning(this, "Invalid Input", "Course name is too long (max 100 characters)!");
        ui->courseNameInput->setFocus();
        return;
    }
    if (startTime.isEmpty()) {
        UIDialogs::showWarning(this, "Invalid Input", "Please select start time!");
        ui->startTimeLabel->setFocus();
        return;
    }
    if (endTime.isEmpty()) {
        UIDialogs::showWarning(this, "Invalid Input", "Please select end time!");
        ui->endTimeInput->setFocus();
        return;
    }
    if (classroom.isEmpty()) {
        UIDialogs::showWarning(this, "Invalid Input", "Please enter classroom!");
        ui->classroomInput->setFocus();
        return;
    }
    if (classroom.contains("|")) {
        UIDialogs::showWarning(this, "Invalid Input", "Classroom cannot contain the '|' character!");
        ui->classroomInput->setFocus();
        return;
    }
    if (classroom.length() > 50) {
        UIDialogs::showWarning(this, "Invalid Input", "Classroom name is too long (max 50 characters)!");
        ui->classroomInput->setFocus();
        return;
    }
    if (timeToInt(startTime) >= timeToInt(endTime)) {  // Validate time logic
        UIDialogs::showWarning(this, "Invalid Time", QString("End time (%1) must be after start time (%2)!").arg(endTime, startTime));
        ui->endTimeInput->setFocus();
        return;
    }

    // Check for exact duplicates (allow same course name with different times/rooms)
    for (int i = 0; i < courses.size(); ++i) {
        if (i == editingRow) continue;  // Skip current editing course
        if (courses[i].name == name && courses[i].day == day && courses[i].startTime == startTime &&
            courses[i].endTime == endTime && courses[i].classroom == classroom) {
            UIDialogs::showWarning(this, "Duplicate Course",
                QString("This exact course already exists!\n\nCourse: %1\nDay: %2\nTime: %3 - %4\nClassroom: %5")
                    .arg(name, day, startTime, endTime, classroom));
            return;
        }
    }

    // Create temporary course object for conflict check
    Course newCourse;
    newCourse.name = name;
    newCourse.day = day;
    newCourse.startTime = startTime;
    newCourse.endTime = endTime;
    newCourse.classroom = classroom;

    // Check for time conflicts and warn user
    LinkedList<Course> conflicts = getConflictingCourses(newCourse, editingRow);
    if (!conflicts.isEmpty()) {
        QString conflictList;
        for (int i = 0; i < conflicts.size(); ++i) {
            conflictList += QString("- %1 (%2 %3-%4)\n")
                .arg(conflicts[i].name, conflicts[i].day, conflicts[i].startTime, conflicts[i].endTime);
        }

        // Ask user if they want to continue despite conflicts
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Time Conflict Warning",
            QString("This course conflicts with the following existing courses:\n\n%1\n"
                    "Do you still want to add this course?").arg(conflictList),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (reply == QMessageBox::No) {
            return;  // User cancelled
        }
    }

    // Exit search mode when adding/editing courses
    if (isSearching) {
        courses = allCourses;
        isSearching = false;
    }

    if (editingRow >= 0 && editingRow < courses.size()) {  /* Update existing course */
        courses[editingRow].name = name;
        courses[editingRow].day = day;
        courses[editingRow].startTime = startTime;
        courses[editingRow].endTime = endTime;
        courses[editingRow].classroom = classroom;

        /* Reset selection state before refresh to prevent stale row highlighting */
        selectedRow = -1;
        editingRow = -1;

        refreshTable();
        clearForm();
        saveCoursesToFile();

        // Disable View Timetable - need to regenerate after editing
        timetableGenerated = false;
        if (ui->viewTimetableBtn) {
            ui->viewTimetableBtn->setEnabled(false);
            ui->viewTimetableBtn->setToolTip("Course modified - please click 'Generate Timetable' again");
        }

        UIDialogs::showInfo(this, "Success", QString("Course '%1' updated successfully!").arg(name));
    } else {  /* Add new course */
        Course course;
        course.name = name;
        course.day = day;
        course.startTime = startTime;
        course.endTime = endTime;
        course.classroom = classroom;
        courses.append(course);

        /* Reset selection state before refresh */
        selectedRow = -1;

        refreshTable();
        clearForm();
        saveCoursesToFile();

        // Disable View Timetable - need to regenerate after adding
        timetableGenerated = false;
        if (ui->viewTimetableBtn) {
            ui->viewTimetableBtn->setEnabled(false);
            ui->viewTimetableBtn->setToolTip("New course added - please click 'Generate Timetable' again");
        }

        UIDialogs::showInfo(this, "Success", QString("Course '%1' added successfully!").arg(name));
    }
}

/**
 * Delete course and update edit mode state if needed
 */
void ManageCoursesPage::onDeleteCourse(int row) {
    /* Exit search mode when deleting courses */
    if (isSearching) {
        courses = allCourses;
        isSearching = false;
    }

    if (row >= 0 && row < courses.size()) {
        QString name = courses[row].name;
        courses.removeAt(row);

        if (editingRow == row) {  /* Deleted the course being edited */
            editingRow = -1;
            clearForm();
        } else if (editingRow > row) {  /* Deleted a course before the one being edited */
            editingRow--;  /* Adjust for index shift */
        }

        /* Reset selection state - the deleted row is no longer valid */
        selectedRow = -1;

        refreshTable();
        saveCoursesToFile();

        // Disable View Timetable - need to regenerate after deleting
        timetableGenerated = false;
        if (ui->viewTimetableBtn) {
            ui->viewTimetableBtn->setEnabled(false);
            ui->viewTimetableBtn->setToolTip("Course deleted - please click 'Generate Timetable' again");
        }

        UIDialogs::showInfo(this, "Deleted", QString("Course '%1' deleted!").arg(name));
    }
}

/**
 * Delete all courses at once
 * Shows confirmation dialog before deleting
 */
void ManageCoursesPage::onDeleteAllCourses() {
    /* Check if there are any courses to delete */
    if (courses.isEmpty()) {
        UIDialogs::showInfo(this, "No Courses", "There are no courses to delete.");
        return;
    }

    /* Show confirmation dialog */
    bool confirmed = UIDialogs::showConfirmation(
        this,
        "Delete All Courses",
        QString("Are you sure you want to delete ALL %1 courses?\n\nThis action cannot be undone!")
            .arg(courses.size())
    );

    if (!confirmed) {
        return;  /* User cancelled */
    }

    /* Exit search mode if active */
    if (isSearching) {
        isSearching = false;
        allCourses.clear();
    }

    /* Clear all courses */
    int count = courses.size();
    courses.clear();

    /* Reset state */
    editingRow = -1;
    selectedRow = -1;
    clearForm();

    /* Update UI */
    refreshTable();
    saveCoursesToFile();

    /* Disable View Timetable - need to regenerate */
    timetableGenerated = false;
    if (ui->viewTimetableBtn) {
        ui->viewTimetableBtn->setEnabled(false);
        ui->viewTimetableBtn->setToolTip("All courses deleted - please add courses and click 'Generate Timetable'");
    }

    UIDialogs::showInfo(this, "Deleted", QString("All %1 courses have been deleted!").arg(count));
}

/**
 * Rebuild table with all courses
 * Creates checkboxes, text items, and action buttons for each row
 */
void ManageCoursesPage::refreshTable() {
    if (!ui->coursetable) return;

    /* Clear existing content before rebuilding to prevent stale data display */
    ui->coursetable->clearContents();
    ui->coursetable->setRowCount(courses.size());

    for (int row = 0; row < courses.size(); ++row) {
        ui->coursetable->setRowHeight(row, 40);
    }

    for (int row = 0; row < courses.size(); ++row) {
        // Column 0: Checkbox widget
        QWidget *checkBoxWidget = new QWidget();
        checkBoxWidget->setStyleSheet("background-color: transparent;");
        QHBoxLayout *checkBoxLayout = new QHBoxLayout(checkBoxWidget);
        checkBoxLayout->setContentsMargins(0, 0, 0, 0);
        QCheckBox *checkBox = new QCheckBox();
        checkBox->setStyleSheet(UIStyles::checkboxBlue());
        checkBoxLayout->addWidget(checkBox);
        checkBoxLayout->setAlignment(Qt::AlignCenter);
        checkBoxLayout->setContentsMargins(0, 0, 0, 0);
        checkBoxWidget->setLayout(checkBoxLayout);
        ui->coursetable->setCellWidget(row, 0, checkBoxWidget);

        // Columns 1-4: Course data (non-editable text items)
        QTableWidgetItem *nameItem = new QTableWidgetItem(courses[row].name);
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        nameItem->setForeground(QBrush(Qt::black));
        ui->coursetable->setItem(row, 1, nameItem);

        QTableWidgetItem *dayItem = new QTableWidgetItem(courses[row].day);
        dayItem->setFlags(dayItem->flags() & ~Qt::ItemIsEditable);
        dayItem->setForeground(QBrush(Qt::black));
        ui->coursetable->setItem(row, 2, dayItem);

        QString timeStr = QString("%1 - %2").arg(courses[row].startTime, courses[row].endTime);
        QTableWidgetItem *timeItem = new QTableWidgetItem(timeStr);
        timeItem->setFlags(timeItem->flags() & ~Qt::ItemIsEditable);
        timeItem->setForeground(QBrush(Qt::black));
        ui->coursetable->setItem(row, 3, timeItem);

        QTableWidgetItem *classroomItem = new QTableWidgetItem(courses[row].classroom);
        classroomItem->setFlags(classroomItem->flags() & ~Qt::ItemIsEditable);
        classroomItem->setForeground(QBrush(Qt::black));
        ui->coursetable->setItem(row, 4, classroomItem);

        // Column 5: Action buttons (Edit and Delete)
        QWidget *actionsWidget = new QWidget();
        actionsWidget->setStyleSheet("background-color: transparent;");
        QHBoxLayout *actionsLayout = new QHBoxLayout(actionsWidget);
        actionsLayout->setContentsMargins(5, 0, 5, 0);
        actionsLayout->setSpacing(5);
        actionsLayout->setAlignment(Qt::AlignCenter);

        QPushButton *editBtn = new QPushButton("✏️ Edit");
        editBtn->setEnabled(false);
        editBtn->setStyleSheet(UIStyles::blueButtonCompact());
        connect(editBtn, &QPushButton::clicked, this, [this, row]() {
            onEditCourse(row);
        });

        QPushButton *deleteBtn = new QPushButton("🗑️ Delete");
        deleteBtn->setEnabled(false);
        deleteBtn->setStyleSheet(UIStyles::redButtonCompact());
        connect(deleteBtn, &QPushButton::clicked, this, [this, row]() {
            if (UIDialogs::showConfirmation(this, "Confirm Delete",
                QString("Are you sure you want to delete '%1'?").arg(courses[row].name))) {
                onDeleteCourse(row);
            }
        });

        // Checkbox toggle: enable/disable buttons and highlight row (SINGLE SELECTION MODE)
        connect(checkBox, &QCheckBox::toggled, this, [this, row, editBtn, deleteBtn, checkBoxWidget, actionsWidget](bool checked) {
            // If this checkbox is checked, uncheck all other checkboxes (single selection)
            if (checked) {
                selectedRow = row;  // Save selected row for state preservation
                for (int i = 0; i < ui->coursetable->rowCount(); ++i) {
                    if (i != row) {  // Skip current row
                        QWidget *otherCheckBoxWidget = ui->coursetable->cellWidget(i, 0);
                        if (otherCheckBoxWidget) {
                            QCheckBox *otherCheckBox = otherCheckBoxWidget->findChild<QCheckBox*>();
                            if (otherCheckBox && otherCheckBox->isChecked()) {
                                otherCheckBox->setChecked(false);  // Uncheck other checkboxes
                            }
                        }
                    }
                }
            } else {
                // If unchecked, clear selection
                if (selectedRow == row) {
                    selectedRow = -1;
                }
            }

            // Enable/disable buttons for current row
            editBtn->setEnabled(checked);
            deleteBtn->setEnabled(checked);

            // Highlight current row
            QColor bgColor = checked ? QColor(232, 232, 232) : QColor(255, 255, 255);
            QString bgColorStr = checked ? UIColors::BACKGROUND_SELECTED_ROW : UIColors::TRANSPARENT;
            if (checkBoxWidget) {
                checkBoxWidget->setStyleSheet(QString("background-color: %1;").arg(bgColorStr));
            }
            for (int col = 1; col < 5; ++col) {
                if (ui->coursetable->item(row, col)) {
                    ui->coursetable->item(row, col)->setBackground(QBrush(bgColor));
                }
            }
            if (actionsWidget) {
                actionsWidget->setStyleSheet(QString("background-color: %1;").arg(bgColorStr));
            }
        });

        actionsLayout->addWidget(editBtn);
        actionsLayout->addWidget(deleteBtn);
        actionsWidget->setLayout(actionsLayout);
        ui->coursetable->setCellWidget(row, 5, actionsWidget);
    }

    if (ui->courseCountLabel) {  // Update course count display
        ui->courseCountLabel->setText(QString("View & Manage Courses (%1)").arg(courses.size()));
    }

    // Restore checkbox selection state after refresh
    if (selectedRow >= 0 && selectedRow < courses.size()) {
        QWidget *checkBoxWidget = ui->coursetable->cellWidget(selectedRow, 0);
        if (checkBoxWidget) {
            QCheckBox *checkBox = checkBoxWidget->findChild<QCheckBox*>();
            if (checkBox) {
                checkBox->setChecked(true);  // Restore checked state
            }
        }
    }
}

// Clear all form fields and exit edit mode
void ManageCoursesPage::clearForm() {
    if (ui->courseNameInput) ui->courseNameInput->clear();
    if (ui->classroomInput) ui->classroomInput->clear();
    if (ui->startTimeLabel) ui->startTimeLabel->setCurrentIndex(0);
    if (ui->endTimeInput) ui->endTimeInput->setCurrentIndex(0);
    if (ui->dayCombo) ui->dayCombo->setCurrentIndex(0);
    if (ui->courseNameInput) ui->courseNameInput->setFocus();
    editingRow = -1;
    if (ui->addCourseBtn) {
        ui->addCourseBtn->setText("+ Add Course");
    }
}

// Load course data into form for editing
void ManageCoursesPage::onEditCourse(int row) {
    if (row >= 0 && row < courses.size()) {
        editingRow = row;
        const Course &course = courses[row];
        if (ui->courseNameInput) ui->courseNameInput->setText(course.name);
        if (ui->dayCombo) {
            int dayIndex = ui->dayCombo->findText(course.day);
            if (dayIndex >= 0) ui->dayCombo->setCurrentIndex(dayIndex);
        }
        if (ui->startTimeLabel) {
            int startIndex = ui->startTimeLabel->findText(course.startTime);
            if (startIndex >= 0) ui->startTimeLabel->setCurrentIndex(startIndex);
        }
        if (ui->endTimeInput) {
            int endIndex = ui->endTimeInput->findText(course.endTime);
            if (endIndex >= 0) ui->endTimeInput->setCurrentIndex(endIndex);
        }
        if (ui->classroomInput) ui->classroomInput->setText(course.classroom);
        if (ui->addCourseBtn) {
            ui->addCourseBtn->setText("Confirm Edit");
        }
        if (ui->courseNameInput) ui->courseNameInput->setFocus();
    }
}

// Show loading dialog then generate timetable
void ManageCoursesPage::onGenerateTimetable() {
    if (courses.isEmpty()) {
        UIDialogs::showWarning(this, "No Courses", "Please add at least one course first!");
        return;
    }
    if (!loadingDialog) {
        loadingDialog = new LoadingDialog(this);
        connect(loadingDialog, &LoadingDialog::loadingComplete, this, &ManageCoursesPage::onLoadingComplete);
    }
    loadingDialog->startLoading();
    loadingDialog->exec();
}

// Create and show timetable window with course data
void ManageCoursesPage::onLoadingComplete() {
    if (timetableWindow) {
        timetableWindow->close();
        delete timetableWindow;
        timetableWindow = nullptr;
    }
    timetableWindow = new TIMETABLE(this);
    timetableWindow->setAttribute(Qt::WA_DeleteOnClose, false);  // Don't auto-delete
    timetableWindow->setCoursesData(courses);
    timetableWindow->show();
    timetableWindow->raise();
    timetableWindow->activateWindow();

    // Connect to finished signal - disable View button when closed
    connect(timetableWindow, &QDialog::finished, this, [this]() {
        timetableWindow = nullptr;
        timetableGenerated = false;
        if (ui->viewTimetableBtn) {
            ui->viewTimetableBtn->setEnabled(false);
            ui->viewTimetableBtn->setToolTip("Please click 'Generate Timetable' first");
        }
    });

    // Enable View Timetable button after generation
    timetableGenerated = true;
    if (ui->viewTimetableBtn) {
        ui->viewTimetableBtn->setEnabled(true);
        ui->viewTimetableBtn->setToolTip("View the generated timetable");
    }
}

// View timetable directly without loading dialog
void ManageCoursesPage::onViewTimetable() {
    if (!timetableGenerated || !timetableWindow) {
        UIDialogs::showWarning(this, "No Timetable", "Please click 'Generate Timetable' first!");
        return;
    }

    if (courses.isEmpty()) {
        UIDialogs::showWarning(this, "No Courses", "Please add courses first!");
        return;
    }

    // Show the existing timetable window
    timetableWindow->show();
    timetableWindow->raise();
    timetableWindow->activateWindow();
}

// Create backup of course data before saving
void ManageCoursesPage::createBackup()
{
    QString appDir = QCoreApplication::applicationDirPath();
    QString filename = QString("%1/courses_%2.txt").arg(appDir, currentUser);
    QString backupFilename = QString("%1/courses_%2_backup.txt").arg(appDir, currentUser);

    QFile originalFile(filename);
    if (originalFile.exists()) {
        // Remove old backup if exists
        QFile::remove(backupFilename);
        // Create new backup
        originalFile.copy(backupFilename);
    }
}

// Restore courses from backup file
void ManageCoursesPage::restoreFromBackup()
{
    QString appDir = QCoreApplication::applicationDirPath();
    QString backupFilename = QString("%1/courses_%2_backup.txt").arg(appDir, currentUser);

    QFile backupFile(backupFilename);
    if (!backupFile.exists()) {
        UIDialogs::showWarning(this, "Restore Error", "No backup file found!");
        return;
    }

    QString filename = QString("%1/courses_%2.txt").arg(appDir, currentUser);
    QFile::remove(filename);
    backupFile.copy(filename);

    // Reload courses
    courses.clear();
    loadCoursesFromFile();
    UIDialogs::showInfo(this, "Restore Complete", "Courses restored from backup!");
}

// Check if new course has time conflict with existing courses
bool ManageCoursesPage::checkTimeConflict(const Course &newCourse, int excludeRow)
{
    for (int i = 0; i < courses.size(); ++i) {
        if (i == excludeRow) continue;  // Skip the course being edited

        const Course &existing = courses[i];
        if (existing.day != newCourse.day) continue;  // Different days, no conflict

        int newStart = timeToInt(newCourse.startTime);
        int newEnd = timeToInt(newCourse.endTime);
        int existStart = timeToInt(existing.startTime);
        int existEnd = timeToInt(existing.endTime);

        // Check for time overlap
        if (newStart < existEnd && existStart < newEnd) {
            return true;  // Conflict found
        }
    }
    return false;
}

// Get list of courses that conflict with the new course
LinkedList<Course> ManageCoursesPage::getConflictingCourses(const Course &newCourse, int excludeRow)
{
    LinkedList<Course> conflicts;

    for (int i = 0; i < courses.size(); ++i) {
        if (i == excludeRow) continue;

        const Course &existing = courses[i];
        if (existing.day != newCourse.day) continue;

        int newStart = timeToInt(newCourse.startTime);
        int newEnd = timeToInt(newCourse.endTime);
        int existStart = timeToInt(existing.startTime);
        int existEnd = timeToInt(existing.endTime);

        if (newStart < existEnd && existStart < newEnd) {
            conflicts.append(existing);
        }
    }
    return conflicts;
}

// Save all courses to file (format: name|day|startTime|endTime|classroom)
void ManageCoursesPage::saveCoursesToFile()
{
    // Create backup before saving
    createBackup();

    QString appDir = QCoreApplication::applicationDirPath();
    QString filename = QString("%1/courses_%2.txt").arg(appDir, currentUser);
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        UIDialogs::showWarning(this, "File Error", "Could not save course data to file!");
        return;
    }

    QTextStream out(&file);
    for (const Course &course : courses) {
        out << course.name << "|" << course.day << "|" << course.startTime << "|"
            << course.endTime << "|" << course.classroom << "\n";
    }
    file.close();
}

// Load courses from file on startup
void ManageCoursesPage::loadCoursesFromFile()
{
    QString appDir = QCoreApplication::applicationDirPath();
    QString filename = QString("%1/courses_%2.txt").arg(appDir, currentUser);
    QFile file(filename);

    if (!file.exists()) return;  // Skip if file doesn't exist (first run)

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        UIDialogs::showWarning(this, "File Error", "Could not load course data from file!");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split("|");
        if (parts.size() == 5) {
            Course course;
            course.name = parts[0].trimmed();
            course.day = parts[1].trimmed();
            course.startTime = parts[2].trimmed();
            course.endTime = parts[3].trimmed();
            course.classroom = parts[4].trimmed();
            courses.append(course);
        }
    }
    file.close();
    refreshTable();
}

/**
 * ============================================================================
 * LINEAR SEARCH ALGORITHM IMPLEMENTATION
 * ============================================================================
 *
 * Time Complexity: O(n) - checks every course once
 * Space Complexity: O(k) where k = number of matches
 *
 * How it works:
 * 1. Convert search text to lowercase for case-insensitive search
 * 2. Loop through each course in the LinkedList
 * 3. Check if search text is found in any field (name, day, time, classroom)
 * 4. If found, add the index to results list
 *
 * @param searchText - The text to search for
 * @return LinkedList<int> - List of indices where matches were found
 * ============================================================================
 */
LinkedList<int> ManageCoursesPage::linearSearchCourses(const QString &searchText)
{
    LinkedList<int> matchIndices;  // Store indices of matching courses
    QString lowerSearchText = searchText.toLower();  // Case-insensitive search

    // LINEAR SEARCH: Check each course one by one (O(n) time complexity)
    for (int i = 0; i < courses.size(); ++i) {
        bool matchFound = false;

        // Check all fields for the search text
        if (courses[i].name.toLower().contains(lowerSearchText)) matchFound = true;
        if (courses[i].day.toLower().contains(lowerSearchText)) matchFound = true;
        if (courses[i].startTime.toLower().contains(lowerSearchText)) matchFound = true;
        if (courses[i].endTime.toLower().contains(lowerSearchText)) matchFound = true;
        if (courses[i].classroom.toLower().contains(lowerSearchText)) matchFound = true;

        // If match found in any field, add index to results
        if (matchFound) matchIndices.append(i);
    }
    return matchIndices;
}

/**
 * ============================================================================
 * QUICKSORT ALGORITHM IMPLEMENTATION
 * ============================================================================
 *
 * Time Complexity: O(n log n) average case
 * Space Complexity: O(n) for temporary array
 *
 * How it works:
 * 1. Define comparison function based on sort criteria
 * 2. Call quickSort() method on LinkedList (implemented in datastructures.h)
 * 3. QuickSort uses partition and recursive divide-and-conquer
 *
 * Sort options:
 * - 0: Sort by Name (A-Z alphabetically)
 * - 1: Sort by Day (Monday -> Sunday)
 * - 2: Sort by Time (earliest first)
 * - 3: Sort by Classroom (A-Z alphabetically)
 *
 * @param sortBy - Sort criteria (0=name, 1=day, 2=time, 3=classroom)
 * ============================================================================
 */
void ManageCoursesPage::quickSortCourses(int sortBy)
{
    if (courses.isEmpty()) return;

    auto compare = [this, sortBy](const Course& c1, const Course& c2) -> bool {
        switch (sortBy) {
            case 0: return c1.name.toLower() < c2.name.toLower();  // Sort by name
            case 1: {  // Sort by day
                QStringList dayOrder = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
                int day1Index = dayOrder.indexOf(c1.day);
                int day2Index = dayOrder.indexOf(c2.day);
                if (day1Index == -1) day1Index = 999;
                if (day2Index == -1) day2Index = 999;
                return day1Index < day2Index;
            }
            case 2: return timeToInt(c1.startTime) < timeToInt(c2.startTime);  // Sort by time
            case 3: return c1.classroom.toLower() < c2.classroom.toLower();  // Sort by classroom
            default: return false;
        }
    };

    courses.quickSort(compare);
    refreshTable();
    saveCoursesToFile();
}

/**
 * Search courses and show matching results
 * Uses LINEAR SEARCH algorithm to find courses matching the search text
 */
void ManageCoursesPage::onSearchCourse()
{
    QString searchText = searchInput->text().trimmed();
    if (searchText.isEmpty()) {
        UIDialogs::showWarning(this, "Search Error", "Please enter search text!");
        return;
    }

    /* Reset selected row before search - old selection may not be valid after filtering */
    selectedRow = -1;

    /* If already searching, restore all courses first */
    if (isSearching) {
        courses = allCourses;
    }

    LinkedList<int> matchIndices = linearSearchCourses(searchText);
    if (matchIndices.isEmpty()) {
        UIDialogs::showInfo(this, "No Results", QString("No courses found matching '%1'").arg(searchText));
        return;
    }

    /* Backup all courses before filtering */
    allCourses = courses;

    /* Filter courses to show only search results */
    LinkedList<Course> searchResults;
    for (int i = 0; i < matchIndices.size(); ++i) {
        searchResults.append(courses[matchIndices[i]]);
    }
    courses = searchResults;

    /* Mark as searching and refresh display */
    isSearching = true;
    refreshTable();

    UIDialogs::showInfo(this, "Search Results",
        QString("Found %1 course(s) matching '%2'\n\nClick 'Clear Search' to show all courses again.")
            .arg(matchIndices.size()).arg(searchText));
}

/**
 * Clear search and show all courses
 * Restores the full course list from backup
 */
void ManageCoursesPage::onClearSearch()
{
    searchInput->clear();

    /* Reset selected row when clearing search */
    selectedRow = -1;

    /* If in search mode, restore all courses */
    if (isSearching) {
        courses = allCourses;
        isSearching = false;
        refreshTable();
        UIDialogs::showInfo(this, "Search Cleared", "Showing all courses.");
    } else {
        UIDialogs::showInfo(this, "No Active Search", "Already showing all courses.");
    }
}

/**
 * Sort courses by selected criteria
 * When in search mode, restore all courses first, then sort, then re-apply search
 */
void ManageCoursesPage::onSortCourses(int index)
{
    if (index == 0) return;  /* Ignore "Select Sort Option" */

    /* Reset selection state - indices will change after sorting */
    selectedRow = -1;

    /* If in search mode, restore all courses first then sort */
    QString currentSearchText;
    if (isSearching) {
        currentSearchText = searchInput->text().trimmed();
        courses = allCourses;  /* Restore all courses */
        isSearching = false;
    }

    if (courses.isEmpty()) {
        UIDialogs::showWarning(this, "Sort Error", "No courses to sort!");
        return;
    }

    quickSortCourses(index - 1);

    /* Re-apply search filter if was searching */
    if (!currentSearchText.isEmpty()) {
        LinkedList<int> matchIndices = linearSearchCourses(currentSearchText);
        if (!matchIndices.isEmpty()) {
            allCourses = courses;  /* Backup sorted courses */
            LinkedList<Course> searchResults;
            for (int i = 0; i < matchIndices.size(); ++i) {
                searchResults.append(courses[matchIndices[i]]);
            }
            courses = searchResults;
            isSearching = true;
            refreshTable();
        }
    }

    QString sortCriteria;
    switch (index) {
        case 1: sortCriteria = "Name"; break;
        case 2: sortCriteria = "Day"; break;
        case 3: sortCriteria = "Time"; break;
        case 4: sortCriteria = "Classroom"; break;
    }
    UIDialogs::showInfo(this, "Sort Complete", QString("All courses sorted by %1!").arg(sortCriteria));
}

// Export courses to CSV file
void ManageCoursesPage::onExportCourses()
{
    if (courses.isEmpty()) {
        UIDialogs::showWarning(this, "Export Error", "No courses to export!");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Export Courses to CSV",
        QDir::homePath() + "/courses_export.csv",
        "CSV Files (*.csv);;All Files (*.*)"
    );

    if (fileName.isEmpty()) return;  // User cancelled

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        UIDialogs::showWarning(this, "Export Error", "Could not create export file!");
        return;
    }

    QTextStream out(&file);

    // Write CSV header
    out << "Course Name,Day,Start Time,End Time,Classroom\n";

    // Write course data
    for (int i = 0; i < courses.size(); ++i) {
        const Course &course = courses[i];
        // Escape commas in fields by wrapping in quotes
        out << "\"" << course.name << "\","
            << "\"" << course.day << "\","
            << "\"" << course.startTime << "\","
            << "\"" << course.endTime << "\","
            << "\"" << course.classroom << "\"\n";
    }

    file.close();
    UIDialogs::showInfo(this, "Export Successful",
        QString("Exported %1 course(s) to:\n%2").arg(courses.size()).arg(fileName));
}

// Import courses from CSV file
void ManageCoursesPage::onImportCourses()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Import Courses from CSV",
        QDir::homePath(),
        "CSV Files (*.csv);;Text Files (*.txt);;All Files (*.*)"
    );

    if (fileName.isEmpty()) return;  // User cancelled

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        UIDialogs::showWarning(this, "Import Error", "Could not open file!");
        return;
    }

    QTextStream in(&file);
    int importedCount = 0;
    int skippedCount = 0;
    bool isFirstLine = true;

    // Exit search mode before importing
    if (isSearching) {
        courses = allCourses;
        isSearching = false;
    }

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        // Skip header line
        if (isFirstLine) {
            isFirstLine = false;
            if (line.toLower().contains("course name") || line.toLower().contains("day")) {
                continue;  // Skip CSV header
            }
        }

        // Parse CSV line (handle quoted fields)
        QStringList parts;
        QString currentField;
        bool inQuotes = false;

        for (int i = 0; i < line.length(); ++i) {
            QChar c = line[i];
            if (c == '"') {
                inQuotes = !inQuotes;
            } else if (c == ',' && !inQuotes) {
                parts.append(currentField.trimmed());
                currentField.clear();
            } else {
                currentField += c;
            }
        }
        parts.append(currentField.trimmed());  // Add last field

        // Validate we have all 5 fields
        if (parts.size() >= 5) {
            Course course;
            course.name = parts[0];
            course.day = parts[1];
            course.startTime = parts[2];
            course.endTime = parts[3];
            course.classroom = parts[4];

            // Validate course data
            if (course.name.isEmpty() || course.day.isEmpty() ||
                course.startTime.isEmpty() || course.endTime.isEmpty()) {
                skippedCount++;
                continue;
            }

            // Check for exact duplicates
            bool isDuplicate = false;
            for (int i = 0; i < courses.size(); ++i) {
                if (courses[i].name == course.name && courses[i].day == course.day &&
                    courses[i].startTime == course.startTime &&
                    courses[i].endTime == course.endTime &&
                    courses[i].classroom == course.classroom) {
                    isDuplicate = true;
                    break;
                }
            }

            if (!isDuplicate) {
                courses.append(course);
                importedCount++;
            } else {
                skippedCount++;
            }
        } else {
            skippedCount++;
        }
    }

    file.close();

    if (importedCount > 0) {
        refreshTable();
        saveCoursesToFile();

        // Disable View Timetable - need to regenerate after importing
        timetableGenerated = false;
        if (ui->viewTimetableBtn) {
            ui->viewTimetableBtn->setEnabled(false);
            ui->viewTimetableBtn->setToolTip("Courses imported - please click 'Generate Timetable' again");
        }
    }

    QString message = QString("Import completed!\n\nImported: %1 course(s)\nSkipped: %2 (duplicates or invalid)")
        .arg(importedCount).arg(skippedCount);

    if (importedCount > 0) {
        UIDialogs::showInfo(this, "Import Successful", message);
    } else {
        UIDialogs::showWarning(this, "Import Result", message);
    }
}

