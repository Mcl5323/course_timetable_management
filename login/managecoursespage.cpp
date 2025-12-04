/**
 * ManageCoursesPage Implementation File
 *
 * This file implements the course management interface, which is the most
 * complex part of the application. It handles dynamic UI creation,
 * data management, and user interactions.
 */

#include "managecoursespage.h"
#include "ui_managecoursespage.h"
#include "mainwindow.h"
#include "timetable.h"
#include "loadingdialog.h"
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

/**
 * ManageCoursesPage Constructor
 *
 * Initializes the course management page with all UI components.
 * This is a complex constructor that sets up:
 * - Window properties (title, size, style)
 * - ComboBox items (days and times)
 * - Table structure and styling
 * - Button connections
 *
 * Initialization list:
 * - QDialog(parent): Call parent constructor
 * - ui(new Ui::ManageCoursesPage): Create UI components
 * - editingRow(-1): Start in "add mode" (not editing)
 */
ManageCoursesPage::ManageCoursesPage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ManageCoursesPage)
    , editingRow(-1)
    , timetableWindow(nullptr)
    , loadingDialog(nullptr) {

    // Set up the user interface from Qt Designer
    ui->setupUi(this);

    // Configure window properties - force full screen
    this->setWindowTitle("Manage Courses");
    this->setWindowState(Qt::WindowMaximized);  // Force maximize window
    this->showMaximized();  // Maximize window
    this->setStyleSheet("background-color: #34495e;");  // Dark blue-gray background

    /**
     * Setup Day Selection ComboBox
     * Populate with all days of the week
     */
    if (ui->dayCombo) {
        ui->dayCombo->addItems({
            "Monday", "Tuesday", "Wednesday", "Thursday",
            "Friday", "Saturday", "Sunday"
        });
    }

    /**
     * Setup Time Selection ComboBoxes
     *
     * Populates start and end time dropdowns with university hours only.
     * Format: 8am to 10pm (normal university class hours)
     *
     * This provides a user-friendly time selection interface instead
     * of requiring users to type times manually.
     */
    if (ui->startTimeLabel) {
        QStringList hours;

        // Add 8am to 11am
        for (int i = 8; i <= 11; ++i) {
            hours << QString("%1am").arg(i);
        }

        // Add 12pm to 10pm
        hours << "12pm";
        for (int i = 1; i <= 10; ++i) {
            hours << QString("%1pm").arg(i);
        }

        ui->startTimeLabel->addItems(hours);
    }

    // Populate end time combo box with same times
    if (ui->endTimeInput) {
        QStringList hours;

        // Add 8am to 11am
        for (int i = 8; i <= 11; ++i) {
            hours << QString("%1am").arg(i);
        }

        // Add 12pm to 10pm
        hours << "12pm";
        for (int i = 1; i <= 10; ++i) {
            hours << QString("%1pm").arg(i);
        }

        ui->endTimeInput->addItems(hours);
    }

    /**
     * Setup Course Table
     *
     * Configures the table that displays all added courses.
     * This is a complex setup with multiple styling properties.
     */
    if (ui->coursetable) {
        // Set table structure: 6 columns
        ui->coursetable->setColumnCount(6);

        // Set column headers
        ui->coursetable->setHorizontalHeaderLabels({
            "Select",      // Column 0: Checkbox for selecting course
            "Course Name", // Column 1: Course name
            "Day",         // Column 2: Day of week
            "Time",        // Column 3: Start-End time
            "Classroom",   // Column 4: Classroom location
            "Actions"      // Column 5: Edit and Delete buttons
        });

        // Start with 0 rows (empty table)
        ui->coursetable->setRowCount(0);

        // Set specific column widths to show full content
        ui->coursetable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->coursetable->setColumnWidth(0, 80);   // Select checkbox
        ui->coursetable->setColumnWidth(1, 180);  // Course Name
        ui->coursetable->setColumnWidth(2, 120);  // Day
        ui->coursetable->setColumnWidth(3, 120);  // Time
        ui->coursetable->setColumnWidth(4, 120);  // Classroom
        ui->coursetable->setColumnWidth(5, 180);  // Actions (Edit + Delete buttons)

        /**
         * Scrolling Configuration
         * Limit table height to ~10 rows, enable vertical scrolling
         * Enable horizontal scrolling to show full content
         */
        ui->coursetable->setMaximumHeight(400);
        ui->coursetable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->coursetable->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        /**
         * Table Header Styling (CSS-like syntax)
         * Sets background color, text color, padding, borders, and font
         * All columns have the same blue background
         */
        ui->coursetable->horizontalHeader()->setStyleSheet(
            "QHeaderView::section {"
            "background-color: #3498db;"  // Blue background for all columns
            "color: white;"               // White text
            "padding: 8px;"               // Spacing inside header
            "border: none;"               // No borders
            "font-weight: bold;"          // Bold text
            "font-size: 11px;"            // Font size
            "}"
            );

        /**
         * Table Cell Styling (CSS-like syntax)
         * Removes selection highlighting - only checkbox will control row highlighting
         */
        ui->coursetable->setStyleSheet(
            "QTableWidget {"
            "background-color: white;"                     // White background for all rows
            "gridline-color: #d0d0d0;"                    // Grid line color
            "font-size: 11px;"
            "selection-background-color: transparent;"    // Remove selection background
            "}"
            "QTableWidget::item {"
            "color: black;"                               // Black text
            "padding: 0px;"                               // No padding for full background coverage
            "}"
            "QTableWidget::item:selected {"
            "background-color: transparent;"              // No blue selection effect
            "color: black;"                               // Keep text black
            "}"
            "QTableWidget::item:hover {"
            "background-color: transparent;"              // No hover effect
            "}"
            );

        // Disable alternating row colors - all rows should be white
        ui->coursetable->setAlternatingRowColors(false);
    }

    /**
     * Setup Logout Button
     *
     * This uses a lambda function (inline anonymous function) to handle
     * the logout action. Lambda syntax: [captures](parameters) { code }
     */
    if (ui->logupbutton) {
        connect(ui->logupbutton, &QPushButton::clicked, this, [this, parent]() {
            // Hide this window
            this->hide();

            /**
             * Cast parent to MainWindow type
             * qobject_cast is Qt's safe type casting (like dynamic_cast)
             * Returns nullptr if the cast fails
             */
            MainWindow *mainWindow = qobject_cast<MainWindow*>(parent);
            if (mainWindow) {
                // Tell MainWindow to switch back to login page
                mainWindow->switchToLoginPage();
            }
        });
    }

    // Connect all button signals to their slots
    setupConnections();
}

/**
 * Destructor
 * Cleans up UI components
 */
ManageCoursesPage::~ManageCoursesPage() {
    delete ui;
    if (timetableWindow) {
        delete timetableWindow;
    }
    if (loadingDialog) {
        delete loadingDialog;
    }
}

// helper function to compare times
// turns "8am" into 8, "2pm" into 14, etc so we can check if end time > start time
int ManageCoursesPage::timeToInt(const QString &time) {
    QString t = time.toLower().trimmed();
    bool isPM = t.contains("pm");

    QString numStr = t;
    numStr.remove("am").remove("pm");
    int hour = numStr.toInt();

    // convert to 24 hour format for easier comparison
    if (isPM && hour != 12) {
        hour += 12;  // like 2pm becomes 14
    } else if (!isPM && hour == 12) {
        hour = 0;  // midnight edge case
    }

    return hour;
}

/**
 * Setup Signal-Slot Connections
 *
 * Connects all buttons to their respective handler functions.
 * This is separated into its own function for organization.
 */
void ManageCoursesPage::setupConnections() {
    // Add Course button - adds new course or saves edited course
    if (ui->addCourseBtn) {
        connect(ui->addCourseBtn, &QPushButton::clicked,
                this, &ManageCoursesPage::onAddCourse);
    }

    // Generate Timetable button - creates a schedule from courses
    if (ui->generateBtn) {
        connect(ui->generateBtn, &QPushButton::clicked,
                this, &ManageCoursesPage::onGenerateTimetable);
    }

    // View Timetable button - displays the generated schedule
    if (ui->viewTimetableBtn) {
        connect(ui->viewTimetableBtn, &QPushButton::clicked,
                this, &ManageCoursesPage::onViewTimetable);
    }
}

/**
 * Add/Update Course Handler (Slot Function)
 *
 * This function handles both adding new courses AND updating existing courses.
 * The behavior depends on the editingRow variable:
 * - If editingRow == -1: Add new course
 * - If editingRow >= 0: Update the course at that index
 *
 * Process:
 * 1. Retrieve input from form fields
 * 2. Validate all required fields
 * 3. Check for duplicate courses
 * 4. Either update existing course or add new course
 * 5. Refresh table display
 */
void ManageCoursesPage::onAddCourse() {
    // Retrieve and clean input data
    QString name = ui->courseNameInput->text().trimmed();  // trimmed() removes leading/trailing spaces
    QString day = ui->dayCombo->currentText();
    QString startTime = ui->startTimeLabel->currentText();
    QString endTime = ui->endTimeInput->currentText();
    QString classroom = ui->classroomInput->text().trimmed();

    /**
     * Input Validation Section
     * Each field is checked individually with specific error messages
     * Using early returns to stop processing if validation fails
     */

    // Validate course name
    if (name.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Invalid Input");
        msgBox.setText("Please enter course name!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QMessageBox{background-color: #ffffff;} QLabel{color: #000000; font-size: 11px; background-color: transparent;} QPushButton{background-color: #e0e0e0; color: #000000; font-size: 11px; min-width: 60px; padding: 5px;}");
        msgBox.exec();
        ui->courseNameInput->setFocus();  // Move cursor to the field that needs input
        return;
    }

    // Validate start time
    if (startTime.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Invalid Input");
        msgBox.setText("Please select start time!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QMessageBox{background-color: #ffffff;} QLabel{color: #000000; font-size: 11px; background-color: transparent;} QPushButton{background-color: #e0e0e0; color: #000000; font-size: 11px; min-width: 60px; padding: 5px;}");
        msgBox.exec();
        ui->startTimeLabel->setFocus();
        return;
    }

    // Validate end time
    if (endTime.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Invalid Input");
        msgBox.setText("Please select end time!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QMessageBox{background-color: #ffffff;} QLabel{color: #000000; font-size: 11px; background-color: transparent;} QPushButton{background-color: #e0e0e0; color: #000000; font-size: 11px; min-width: 60px; padding: 5px;}");
        msgBox.exec();
        ui->endTimeInput->setFocus();
        return;
    }

    // Validate classroom
    if (classroom.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Invalid Input");
        msgBox.setText("Please enter classroom!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QMessageBox{background-color: #ffffff;} QLabel{color: #000000; font-size: 11px; background-color: transparent;} QPushButton{background-color: #e0e0e0; color: #000000; font-size: 11px; min-width: 60px; padding: 5px;}");
        msgBox.exec();
        ui->classroomInput->setFocus();
        return;
    }

    // Validate time logic: end time must be after start time
    if (timeToInt(startTime) >= timeToInt(endTime)) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Invalid Time");
        msgBox.setText(QString("End time (%1) must be after start time (%2)!")
                      .arg(endTime, startTime));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QMessageBox{background-color: #ffffff;} QLabel{color: #000000; font-size: 11px; background-color: transparent;} QPushButton{background-color: #e0e0e0; color: #000000; font-size: 11px; min-width: 60px; padding: 5px;}");
        msgBox.exec();
        ui->endTimeInput->setFocus();
        return;
    }

    // check for duplicates
    // note: we allow same course name with different times/rooms
    // (useful for courses with multiple sections)
    // but prevent EXACT duplicates (same everything)
    for (int i = 0; i < courses.size(); ++i) {
        // when editing, don't compare with the course we're currently editing
        if (i == editingRow) continue;

        // check if everything matches
        if (courses[i].name == name &&
            courses[i].day == day &&
            courses[i].startTime == startTime &&
            courses[i].endTime == endTime &&
            courses[i].classroom == classroom) {

            // show error message with details
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Duplicate Course");
            msgBox.setText(QString("This exact course already exists!\n\n"
                          "Course: %1\n"
                          "Day: %2\n"
                          "Time: %3 - %4\n"
                          "Classroom: %5")
                          .arg(name)
                          .arg(day)
                          .arg(startTime)
                          .arg(endTime)
                          .arg(classroom));
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setStyleSheet("QMessageBox{background-color: #ffffff;} QLabel{color: #000000; font-size: 11px; background-color: transparent;} QPushButton{background-color: #e0e0e0; color: #000000; font-size: 11px; min-width: 60px; padding: 5px;}");
            msgBox.exec();
            return;
        }
    }

    /**
     * Update Mode: Edit Existing Course
     *
     * If editingRow is valid (>= 0), we're updating an existing course
     * instead of adding a new one.
     */
    if (editingRow >= 0 && editingRow < courses.size()) {
        // Update all fields of the existing course
        courses[editingRow].name = name;
        courses[editingRow].day = day;
        courses[editingRow].startTime = startTime;
        courses[editingRow].endTime = endTime;
        courses[editingRow].classroom = classroom;

        // Exit edit mode by resetting editingRow to -1
        editingRow = -1;

        // Update the table display with the modified data
        refreshTable();

        // Clear the form for next use
        clearForm();

        // Show success message
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Success");
        msgBox.setText(QString("Course '%1' updated successfully!").arg(name));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("QMessageBox{background-color: #ffffff;} QLabel{color: #000000; font-size: 11px; background-color: transparent;} QPushButton{background-color: #e0e0e0; color: #000000; font-size: 11px; min-width: 60px; padding: 5px;}");
        msgBox.exec();
    } else {
        /**
         * Add Mode: Create New Course
         *
         * Create a new Course struct and add it to the courses vector
         */
        Course course;  // Create new Course struct
        course.name = name;
        course.day = day;
        course.startTime = startTime;
        course.endTime = endTime;
        course.classroom = classroom;

        // Add the course to our vector (dynamic array)
        courses.append(course);

        // Update the table to show the new course
        refreshTable();

        // Clear the form for next course
        clearForm();

        // Show success message
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Success");
        msgBox.setText(QString("Course '%1' added successfully!").arg(name));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("QMessageBox{background-color: #ffffff;} QLabel{color: #000000; font-size: 11px; background-color: transparent;} QPushButton{background-color: #e0e0e0; color: #000000; font-size: 11px; min-width: 60px; padding: 5px;}");
        msgBox.exec();
    }
}

/**
 * Delete Course Handler (Slot Function)
 *
 * Removes a course from the courses vector and updates the display.
 *
 * @param row: Index of the course to delete in the courses vector
 *
 * Important: This function also handles edit mode state management.
 * If we delete the course being edited, we exit edit mode.
 */
void ManageCoursesPage::onDeleteCourse(int row) {
    // Validate row index
    if (row >= 0 && row < courses.size()) {
        // Save course name for confirmation message
        QString name = courses[row].name;

        // Remove the course from the vector
        courses.removeAt(row);

        /**
         * Edit Mode State Management
         *
         * After deletion, we need to update editingRow to maintain consistency:
         * 1. If we deleted the course being edited, exit edit mode
         * 2. If we deleted a course before the one being edited,
         *    decrement editingRow (indices shift down after deletion)
         */
        if (editingRow == row) {
            // We deleted the course we were editing
            editingRow = -1;
            clearForm();
        } else if (editingRow > row) {
            // We deleted a course before the one being edited
            // Decrement to account for index shift
            editingRow--;
        }

        // Update table display
        refreshTable();

        // Show confirmation message
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Deleted");
        msgBox.setText(QString("Course '%1' deleted!").arg(name));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("QMessageBox{background-color: #ffffff;} QLabel{color: #000000; font-size: 11px; background-color: transparent;} QPushButton{background-color: #e0e0e0; color: #000000; font-size: 11px; min-width: 60px; padding: 5px;}");
        msgBox.exec();
    }
}

/**
 * Refresh Table Display (Complex Function)
 *
 * This is the most complex function in the application. It rebuilds
 * the entire course table from scratch using dynamic widget creation.
 *
 * Process for each row:
 * 1. Create checkbox widget (Column 0)
 * 2. Add course data as table items (Columns 1-4)
 * 3. Create Edit/Delete button widgets (Column 5)
 * 4. Connect checkbox to enable/disable buttons
 *
 * Key Concept: Dynamic Widget Creation
 * Unlike simple text cells, checkboxes and buttons require creating
 * widgets and layouts programmatically, then inserting them into table cells.
 */
void ManageCoursesPage::refreshTable() {
    // Safety check
    if (!ui->coursetable) return;

    // Set number of rows to match number of courses
    ui->coursetable->setRowCount(courses.size());

    // Set row height to accommodate buttons (compact design)
    for (int row = 0; row < courses.size(); ++row) {
        ui->coursetable->setRowHeight(row, 40);
    }

    /**
     * Main Loop: Process Each Course
     * Iterates through all courses and creates a table row for each
     */
    for (int row = 0; row < courses.size(); ++row) {

        /**
         * COLUMN 0: Checkbox Widget (Complex Widget Creation)
         *
         * This demonstrates how to create custom widgets for table cells.
         * Steps:
         * 1. Create container widget
         * 2. Create layout to position checkbox
         * 3. Create and style the checkbox
         * 4. Add checkbox to layout
         * 5. Set layout in container
         * 6. Insert container into table cell
         */

        // Create a container widget for the checkbox
        QWidget *checkBoxWidget = new QWidget();
        checkBoxWidget->setStyleSheet("background-color: transparent;");

        // Create horizontal layout to center the checkbox
        QHBoxLayout *checkBoxLayout = new QHBoxLayout(checkBoxWidget);
        checkBoxLayout->setContentsMargins(0, 0, 0, 0);  // Remove margins for full coverage

        // Create the actual checkbox
        QCheckBox *checkBox = new QCheckBox();

        // Style the checkbox with blue design
        checkBox->setStyleSheet(
            "QCheckBox::indicator {"
            "width: 20px;"
            "height: 20px;"
            "border: 2px solid #3498db;"        // Blue border
            "border-radius: 4px;"
            "background-color: white;"
            "}"
            "QCheckBox::indicator:hover {"
            "border: 2px solid #2980b9;"        // Darker blue on hover
            "background-color: #ecf0f1;"
            "}"
            "QCheckBox::indicator:checked {"
            "background-color: #3498db;"        // Blue background
            "border: 2px solid #2980b9;"
            "}"
            "QCheckBox::indicator:checked:hover {"
            "background-color: #2980b9;"        // Darker blue on hover
            "}"
            );

        // Add checkbox to layout and center it
        checkBoxLayout->addWidget(checkBox);
        checkBoxLayout->setAlignment(Qt::AlignCenter);
        checkBoxLayout->setContentsMargins(0, 0, 0, 0);  // No padding

        // Apply layout to container widget
        checkBoxWidget->setLayout(checkBoxLayout);

        // Insert the entire widget into the table cell
        ui->coursetable->setCellWidget(row, 0, checkBoxWidget);

        /**
         * COLUMNS 1-4: Simple Text Items
         *
         * These are simple table cells containing text.
         * QTableWidgetItem is used for basic text display.
         */

        // Column 1: Course Name
        QTableWidgetItem *nameItem = new QTableWidgetItem(courses[row].name);
        // Remove editable flag (prevents user from clicking to edit)
        // Bitwise operation: removes ItemIsEditable flag from existing flags
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        nameItem->setForeground(QBrush(Qt::black));  // Black text color
        ui->coursetable->setItem(row, 1, nameItem);

        // Column 2: Day
        QTableWidgetItem *dayItem = new QTableWidgetItem(courses[row].day);
        dayItem->setFlags(dayItem->flags() & ~Qt::ItemIsEditable);
        dayItem->setForeground(QBrush(Qt::black));
        ui->coursetable->setItem(row, 2, dayItem);

        // Column 3: Time (formatted as "Start - End")
        QString timeStr = QString("%1 - %2")
                              .arg(courses[row].startTime, courses[row].endTime);
        QTableWidgetItem *timeItem = new QTableWidgetItem(timeStr);
        timeItem->setFlags(timeItem->flags() & ~Qt::ItemIsEditable);
        timeItem->setForeground(QBrush(Qt::black));
        ui->coursetable->setItem(row, 3, timeItem);

        // Column 4: Classroom
        QTableWidgetItem *classroomItem = new QTableWidgetItem(courses[row].classroom);
        classroomItem->setFlags(classroomItem->flags() & ~Qt::ItemIsEditable);
        classroomItem->setForeground(QBrush(Qt::black));
        ui->coursetable->setItem(row, 4, classroomItem);

        /**
         * COLUMN 5: Action Buttons (Complex Widget Creation)
         *
         * Creates Edit and Delete buttons for each row.
         * This demonstrates:
         * - Creating multiple buttons in a layout
         * - Conditional button styling
         * - Lambda functions with captures
         * - Dynamic button enabling/disabling
         */

        // Create container widget for the buttons
        QWidget *actionsWidget = new QWidget();
        actionsWidget->setStyleSheet("background-color: transparent;");

        // Create horizontal layout to hold both buttons side by side
        QHBoxLayout *actionsLayout = new QHBoxLayout(actionsWidget);
        actionsLayout->setContentsMargins(5, 0, 5, 0);  // Remove top/bottom margins for full coverage
        actionsLayout->setSpacing(5);  // Space between buttons
        actionsLayout->setAlignment(Qt::AlignCenter);  // Center align buttons

        /**
         * Edit Button Creation and Configuration
         */
        QPushButton *editBtn = new QPushButton("✏️ Edit");

        // Start disabled - only enabled when checkbox is checked
        editBtn->setEnabled(false);

        // Blue color scheme - compact design like in the screenshot
        editBtn->setStyleSheet(
            "QPushButton {"
            "background-color: #3498db;"       // Blue background
            "color: white;"
            "border: none;"
            "border-radius: 4px;"              // Smaller rounded corners
            "padding: 4px 8px;"                // Compact padding
            "font-size: 11px;"                 // Smaller font
            "font-weight: 600;"
            "min-width: 55px;"                 // Compact width
            "min-height: 26px;"                // Compact height
            "max-height: 26px;"                // Fixed compact height
            "}"
            "QPushButton:disabled {"
            "background-color: #3498db;"       // Keep blue when disabled
            "color: white;"
            "opacity: 0.6;"                    // Semi-transparent to show disabled
            "}"
            "QPushButton:hover:enabled {"
            "background-color: #2980b9;"       // Darker blue on hover
            "}"
            "QPushButton:pressed {"
            "background-color: #21618c;"       // Even darker when pressed
            "}"
            );

        /**
         * Edit Button Click Handler (Lambda Function)
         *
         * Lambda syntax: [captures](parameters) { code }
         * - [this, row]: Capture these variables by value
         * - this: Needed to call onEditCourse()
         * - row: Needed to know which course to edit
         */
        connect(editBtn, &QPushButton::clicked, this, [this, row]() {
            // Call the edit function
            onEditCourse(row);
        });

        /**
         * Delete Button Creation and Configuration
         * Red color scheme - disabled until checkbox is checked
         */
        QPushButton *deleteBtn = new QPushButton("🗑️ Delete");

        // Start disabled - only enabled when checkbox is checked
        deleteBtn->setEnabled(false);

        // Red color scheme - compact design like in the screenshot
        deleteBtn->setStyleSheet(
            "QPushButton {"
            "background-color: #e74c3c;"       // Red background
            "color: white;"
            "border: none;"
            "border-radius: 4px;"              // Smaller rounded corners to match edit button
            "padding: 4px 8px;"                // Compact padding
            "font-size: 11px;"                 // Smaller font
            "font-weight: 600;"
            "min-width: 55px;"                 // Compact width
            "min-height: 26px;"                // Compact height
            "max-height: 26px;"                // Fixed compact height
            "}"
            "QPushButton:disabled {"
            "background-color: #e74c3c;"       // Keep red when disabled
            "color: white;"
            "opacity: 0.6;"                    // Semi-transparent to show disabled
            "}"
            "QPushButton:hover:enabled {"
            "background-color: #c0392b;"       // Darker red on hover
            "}"
            "QPushButton:pressed {"
            "background-color: #a93226;"       // Even darker when pressed
            "}"
            );

        /**
         * Delete Button Click Handler (Lambda Function)
         * Calls delete function with confirmation
         */
        connect(deleteBtn, &QPushButton::clicked, this, [this, row]() {
            // Ask for confirmation before deleting
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Confirm Delete");
            msgBox.setText(QString("Are you sure you want to delete '%1'?")
                          .arg(courses[row].name));
            msgBox.setIcon(QMessageBox::Question);
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setStyleSheet("QMessageBox{background-color: #ffffff;} QLabel{color: #000000; font-size: 11px; background-color: transparent;} QPushButton{background-color: #e0e0e0; color: #000000; font-size: 11px; min-width: 60px; padding: 5px;}");

            if (msgBox.exec() == QMessageBox::Yes) {
                // Call the delete function
                onDeleteCourse(row);
            }
        });

        /**
         * Checkbox Connection - Enable/Disable Buttons and Highlight Row
         *
         * When checkbox is checked:
         * 1. Enable Edit and Delete buttons
         * 2. Highlight the entire row with gray background
         *
         * Multiple rows can be selected simultaneously
         */
        connect(checkBox, &QCheckBox::toggled, this, [this, row, editBtn, deleteBtn, checkBoxWidget, actionsWidget](bool checked) {
            // Enable or disable the buttons
            editBtn->setEnabled(checked);
            deleteBtn->setEnabled(checked);

            // Change background color for the entire row
            // Light gray (#E8E8E8) when checked, white when unchecked
            QColor bgColor = checked ? QColor(232, 232, 232) : QColor(255, 255, 255);
            QString bgColorStr = checked ? "#E8E8E8" : "transparent";

            // Change background for checkbox widget (column 0)
            if (checkBoxWidget) {
                checkBoxWidget->setStyleSheet(QString("background-color: %1;").arg(bgColorStr));
            }

            // Change background for all data cells (columns 1-4)
            for (int col = 1; col < 5; ++col) {
                if (ui->coursetable->item(row, col)) {
                    ui->coursetable->item(row, col)->setBackground(QBrush(bgColor));
                }
            }

            // Change background for actions widget (column 5)
            if (actionsWidget) {
                actionsWidget->setStyleSheet(QString("background-color: %1;").arg(bgColorStr));
            }
        });

        // Add both buttons to the layout
        actionsLayout->addWidget(editBtn);
        actionsLayout->addWidget(deleteBtn);

        // Apply layout to container widget
        actionsWidget->setLayout(actionsLayout);

        // Insert the entire widget into the table cell
        ui->coursetable->setCellWidget(row, 5, actionsWidget);
    }

    /**
     * Update Course Count Display
     * Shows total number of courses in the label
     */
    if (ui->courseCountLabel) {
        ui->courseCountLabel->setText(
            QString("View & Manage Courses (%1)").arg(courses.size()));
    }
}

/**
 * Clear Form Function
 *
 * Resets all input fields to their initial state.
 * Called after adding/editing a course to prepare for next input.
 */
void ManageCoursesPage::clearForm() {
    // Clear text input fields
    if (ui->courseNameInput) ui->courseNameInput->clear();
    if (ui->classroomInput) ui->classroomInput->clear();

    // Reset combo boxes to first item (index 0)
    if (ui->startTimeLabel) ui->startTimeLabel->setCurrentIndex(0);
    if (ui->endTimeInput) ui->endTimeInput->setCurrentIndex(0);
    if (ui->dayCombo) ui->dayCombo->setCurrentIndex(0);

    // Set focus to first input field
    if (ui->courseNameInput) ui->courseNameInput->setFocus();

    // Exit edit mode
    editingRow = -1;

    // Reset button text to "Add" mode
    if (ui->addCourseBtn) {
        ui->addCourseBtn->setText("+ Add Course");
    }
}

/**
 * Edit Course Handler (Slot Function)
 *
 * Loads a course's data into the input form for editing.
 * Changes the interface to "edit mode" by:
 * 1. Setting editingRow to the row being edited
 * 2. Populating form fields with course data
 * 3. Changing button text to "Confirm Edit"
 *
 * @param row: Index of the course to edit
 */
void ManageCoursesPage::onEditCourse(int row) {
    // Validate row index
    if (row >= 0 && row < courses.size()) {
        // Enter edit mode
        editingRow = row;

        // Get reference to the course being edited
        const Course &course = courses[row];

        // Populate form fields with course data
        if (ui->courseNameInput) ui->courseNameInput->setText(course.name);

        // For combo boxes, find and select the matching item
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

        // Change button text to indicate edit mode
        if (ui->addCourseBtn) {
            ui->addCourseBtn->setText("Confirm Edit");
        }

        // Focus on first field for immediate editing
        if (ui->courseNameInput) ui->courseNameInput->setFocus();
    }
}

/**
 * Generate Timetable Handler (Slot Function)
 *
 * Generates a timetable from the added courses.
 * Shows loading dialog and then displays timetable window.
 */
void ManageCoursesPage::onGenerateTimetable() {
    // Check if there are any courses to generate timetable from
    if (courses.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("No Courses");
        msgBox.setText("Please add at least one course first!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QMessageBox{background-color: #ffffff;} QLabel{color: #000000; font-size: 11px; background-color: transparent;} QPushButton{background-color: #e0e0e0; color: #000000; font-size: 11px; min-width: 60px; padding: 5px;}");
        msgBox.exec();
        return;
    }

    // Create and show loading dialog
    if (!loadingDialog) {
        loadingDialog = new LoadingDialog(this);
        connect(loadingDialog, &LoadingDialog::loadingComplete,
                this, &ManageCoursesPage::onLoadingComplete);
    }

    loadingDialog->startLoading();
    loadingDialog->exec();
}

/**
 * Loading Complete Handler
 *
 * Called when the loading dialog finishes.
 * Opens the timetable window and populates it with course data.
 */
void ManageCoursesPage::onLoadingComplete() {
    // Always create a fresh timetable window to ensure data is up-to-date
    // Delete old window if it exists
    if (timetableWindow) {
        delete timetableWindow;
        timetableWindow = nullptr;
    }

    // Create new timetable window
    timetableWindow = new TIMETABLE(this);

    // Set the course data and show the timetable
    timetableWindow->setCoursesData(courses);
    timetableWindow->show();
    timetableWindow->raise();
    timetableWindow->activateWindow();
}

/**
 * View Timetable Handler (Slot Function)
 *
 * Displays the generated timetable.
 */
void ManageCoursesPage::onViewTimetable() {
    // Check if there are any courses to view
    if (courses.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("No Courses");
        msgBox.setText("Please add courses first!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QMessageBox{background-color: #ffffff;} QLabel{color: #000000; font-size: 11px; background-color: transparent;} QPushButton{background-color: #e0e0e0; color: #000000; font-size: 11px; min-width: 60px; padding: 5px;}");
        msgBox.exec();
        return;
    }

    // Always create a fresh timetable window to ensure data is up-to-date
    // Delete old window if it exists
    if (timetableWindow) {
        delete timetableWindow;
        timetableWindow = nullptr;
    }

    // Create new timetable window
    timetableWindow = new TIMETABLE(this);

    // Set the course data and show the timetable
    timetableWindow->setCoursesData(courses);
    timetableWindow->show();
    timetableWindow->raise();
    timetableWindow->activateWindow();
}


