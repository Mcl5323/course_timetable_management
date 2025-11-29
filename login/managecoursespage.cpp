#include "managecoursespage.h"
#include "ui_managecoursespage.h"
#include "mainwindow.h"
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

ManageCoursesPage::ManageCoursesPage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ManageCoursesPage)
    , editingRow(-1) {
    ui->setupUi(this);

    this->setWindowTitle("Manage Courses");
    this->setGeometry(100, 100, 1400, 900);
    this->setStyleSheet("background-color: #34495e;");

    // Setup ComboBox items
    if (ui->dayCombo) {
        ui->dayCombo->addItems({
            "Monday", "Tuesday", "Wednesday", "Thursday",
            "Friday", "Saturday", "Sunday"
        });
    }

    // Setup time combo boxes with AM/PM format
    if (ui->startTimeLabel) {
        QStringList hours;
        hours << "12am";
        for (int i = 1; i <= 11; ++i) {
            hours << QString("%1am").arg(i);
        }
        hours << "12pm";
        for (int i = 1; i <= 11; ++i) {
            hours << QString("%1pm").arg(i);
        }
        ui->startTimeLabel->addItems(hours);
    }
    if (ui->endTimeInput) {
        QStringList hours;
        hours << "12am";
        for (int i = 1; i <= 11; ++i) {
            hours << QString("%1am").arg(i);
        }
        hours << "12pm";
        for (int i = 1; i <= 11; ++i) {
            hours << QString("%1pm").arg(i);
        }
        ui->endTimeInput->addItems(hours);
    }

    // Setup table
    if (ui->coursetable) {
        ui->coursetable->setColumnCount(6);
        ui->coursetable->setHorizontalHeaderLabels({
            "Select", "Course Name", "Day", "Time", "Classroom", "Actions"
        });
        ui->coursetable->setRowCount(0);
        ui->coursetable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        // Set maximum height to allow scrolling after ~10 rows
        ui->coursetable->setMaximumHeight(400);
        ui->coursetable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->coursetable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        // Set table header style
        ui->coursetable->horizontalHeader()->setStyleSheet(
            "QHeaderView::section {"
            "background-color: #3498db;"
            "color: white;"
            "padding: 8px;"
            "border: none;"
            "font-weight: bold;"
            "font-size: 11px;"
            "}"
            );

        // Set table style for better visibility
        ui->coursetable->setStyleSheet(
            "QTableWidget {"
            "background-color: white;"
            "alternate-background-color: #f0f0f0;"
            "gridline-color: #d0d0d0;"
            "font-size: 11px;"
            "}"
            "QTableWidget::item {"
            "color: black;"
            "padding: 5px;"
            "}"
            "QTableWidget::item:selected {"
            "background-color: #3498db;"
            "color: white;"
            "}"
            "QTableWidget::item:hover {"
            "background-color: transparent;"
            "}"
            );
        ui->coursetable->setAlternatingRowColors(true);
    }

    // Connect logup button
    if (ui->logupbutton) {
        connect(ui->logupbutton, &QPushButton::clicked, this, [this, parent]() {
            this->hide();
            MainWindow *mainWindow = qobject_cast<MainWindow*>(parent);
            if (mainWindow) {
                mainWindow->switchToLoginPage();
            }
        });
    }

    // Setup connections
    setupConnections();
}

ManageCoursesPage::~ManageCoursesPage() {
    delete ui;
}

void ManageCoursesPage::setupConnections() {
    if (ui->addCourseBtn) {
        connect(ui->addCourseBtn, &QPushButton::clicked,
                this, &ManageCoursesPage::onAddCourse);
    }
    if (ui->generateBtn) {
        connect(ui->generateBtn, &QPushButton::clicked,
                this, &ManageCoursesPage::onGenerateTimetable);
    }
    if (ui->viewTimetableBtn) {
        connect(ui->viewTimetableBtn, &QPushButton::clicked,
                this, &ManageCoursesPage::onViewTimetable);
    }
}

void ManageCoursesPage::onAddCourse() {
    QString name = ui->courseNameInput->text().trimmed();
    QString day = ui->dayCombo->currentText();
    QString startTime = ui->startTimeLabel->currentText();
    QString endTime = ui->endTimeInput->currentText();
    QString classroom = ui->classroomInput->text().trimmed();

    // Validation
    if (name.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Invalid Input");
        msgBox.setText("Please enter course name!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        ui->courseNameInput->setFocus();
        return;
    }
    if (startTime.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Invalid Input");
        msgBox.setText("Please select start time!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        ui->startTimeLabel->setFocus();
        return;
    }
    if (endTime.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Invalid Input");
        msgBox.setText("Please select end time!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        ui->endTimeInput->setFocus();
        return;
    }
    if (classroom.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Invalid Input");
        msgBox.setText("Please enter classroom!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        ui->classroomInput->setFocus();
        return;
    }

    // Check for duplicate course (same day, time, and classroom)
    for (int i = 0; i < courses.size(); ++i) {
        if (i == editingRow) continue; // Skip if editing the same course
        if (courses[i].day == day &&
            courses[i].startTime == startTime &&
            courses[i].classroom == classroom) {
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Duplicate Course");
            msgBox.setText(QString("A course already exists at %1 on %2 in classroom %3!")
                          .arg(startTime, day, classroom));
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
            msgBox.exec();
            return;
        }
    }

    // Check if editing existing course
    if (editingRow >= 0 && editingRow < courses.size()) {
        // Update existing course
        courses[editingRow].name = name;
        courses[editingRow].day = day;
        courses[editingRow].startTime = startTime;
        courses[editingRow].endTime = endTime;
        courses[editingRow].classroom = classroom;
        editingRow = -1;
        refreshTable();
        clearForm();

        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Success");
        msgBox.setText(QString("Course '%1' updated successfully!").arg(name));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
    } else {
        // Create new course object
        Course course;
        course.name = name;
        course.day = day;
        course.startTime = startTime;
        course.endTime = endTime;
        course.classroom = classroom;

        courses.append(course);
        refreshTable();
        clearForm();

        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Success");
        msgBox.setText(QString("Course '%1' added successfully!").arg(name));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
    }
}

void ManageCoursesPage::onDeleteCourse(int row) {
    if (row >= 0 && row < courses.size()) {
        QString name = courses[row].name;
        courses.removeAt(row);
        if (editingRow == row) {
            editingRow = -1;
            clearForm();
        } else if (editingRow > row) {
            editingRow--;
        }
        refreshTable();
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Deleted");
        msgBox.setText(QString("Course '%1' deleted!").arg(name));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
    }
}

void ManageCoursesPage::refreshTable() {
    if (!ui->coursetable) return;

    ui->coursetable->setRowCount(courses.size());

    for (int row = 0; row < courses.size(); ++row) {
        // Column 0: Checkbox for Select (centered)
        QWidget *checkBoxWidget = new QWidget();
        checkBoxWidget->setStyleSheet("background-color: transparent;");
        QHBoxLayout *checkBoxLayout = new QHBoxLayout(checkBoxWidget);
        QCheckBox *checkBox = new QCheckBox();
        checkBox->setStyleSheet(
            "QCheckBox::indicator {"
            "width: 18px;"
            "height: 18px;"
            "border: 2px solid #999;"
            "border-radius: 3px;"
            "background-color: white;"
            "}"
            "QCheckBox::indicator:checked {"
            "background-color: black;"
            "border: 2px solid black;"
            "}"
            "QCheckBox::indicator:checked::after {"
            "content: '✓';"
            "color: white;"
            "font-size: 14px;"
            "font-weight: bold;"
            "}"
            );
        checkBoxLayout->addWidget(checkBox);
        checkBoxLayout->setAlignment(Qt::AlignCenter);
        checkBoxLayout->setContentsMargins(0, 0, 0, 0);
        checkBoxWidget->setLayout(checkBoxLayout);
        ui->coursetable->setCellWidget(row, 0, checkBoxWidget);

        // Column 1: Course Name
        QTableWidgetItem *nameItem = new QTableWidgetItem(courses[row].name);
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        nameItem->setForeground(QBrush(Qt::black));
        ui->coursetable->setItem(row, 1, nameItem);

        // Column 2: Day
        QTableWidgetItem *dayItem = new QTableWidgetItem(courses[row].day);
        dayItem->setFlags(dayItem->flags() & ~Qt::ItemIsEditable);
        dayItem->setForeground(QBrush(Qt::black));
        ui->coursetable->setItem(row, 2, dayItem);

        // Column 3: Time (Start - End)
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

        // Column 5: Actions (Edit and Delete buttons)
        QWidget *actionsWidget = new QWidget();
        actionsWidget->setStyleSheet("background-color: transparent;");
        QHBoxLayout *actionsLayout = new QHBoxLayout(actionsWidget);
        actionsLayout->setContentsMargins(2, 2, 2, 2);
        actionsLayout->setSpacing(5);

        QPushButton *editBtn = new QPushButton("Edit");
        editBtn->setEnabled(false);
        editBtn->setStyleSheet(
            "QPushButton {"
            "background-color: white;"
            "color: #3498db;"
            "border: 1px solid #3498db;"
            "border-radius: 3px;"
            "padding: 5px;"
            "font-size: 9px;"
            "min-width: 50px;"
            "}"
            "QPushButton:disabled {"
            "background-color: #f5f5f5;"
            "color: #ccc;"
            "border: 1px solid #ccc;"
            "}"
            );
        connect(editBtn, &QPushButton::clicked, this, [editBtn, this, row]() {
            editBtn->setStyleSheet(
                "QPushButton {"
                "background-color: #27ae60;"
                "color: white;"
                "border: 1px solid #27ae60;"
                "border-radius: 3px;"
                "padding: 5px;"
                "font-size: 9px;"
                "min-width: 50px;"
                "}"
            );
            onEditCourse(row);
        });

        QPushButton *deleteBtn = new QPushButton("Delete");
        deleteBtn->setEnabled(false);
        deleteBtn->setStyleSheet(
            "QPushButton {"
            "background-color: white;"
            "color: #e74c3c;"
            "border: 1px solid #e74c3c;"
            "border-radius: 3px;"
            "padding: 5px;"
            "font-size: 9px;"
            "min-width: 50px;"
            "}"
            "QPushButton:disabled {"
            "background-color: #f5f5f5;"
            "color: #ccc;"
            "border: 1px solid #ccc;"
            "}"
            );
        connect(deleteBtn, &QPushButton::clicked, this, [deleteBtn, this, row]() {
            deleteBtn->setStyleSheet(
                "QPushButton {"
                "background-color: #e74c3c;"
                "color: white;"
                "border: 1px solid #e74c3c;"
                "border-radius: 3px;"
                "padding: 5px;"
                "font-size: 9px;"
                "min-width: 50px;"
                "}"
            );
            onDeleteCourse(row);
        });

        // Connect checkbox to enable/disable buttons
        connect(checkBox, &QCheckBox::toggled, this, [editBtn, deleteBtn](bool checked) {
            editBtn->setEnabled(checked);
            deleteBtn->setEnabled(checked);
        });

        actionsLayout->addWidget(editBtn);
        actionsLayout->addWidget(deleteBtn);
        actionsWidget->setLayout(actionsLayout);
        ui->coursetable->setCellWidget(row, 5, actionsWidget);
    }

    // Update course count label
    if (ui->courseCountLabel) {
        ui->courseCountLabel->setText(
            QString("View & Manage Courses (%1)").arg(courses.size()));
    }
}

void ManageCoursesPage::clearForm() {
    if (ui->courseNameInput) ui->courseNameInput->clear();
    if (ui->startTimeLabel) ui->startTimeLabel->setCurrentIndex(0);
    if (ui->endTimeInput) ui->endTimeInput->setCurrentIndex(0);
    if (ui->classroomInput) ui->classroomInput->clear();
    if (ui->dayCombo) ui->dayCombo->setCurrentIndex(0);
    if (ui->courseNameInput) ui->courseNameInput->setFocus();
    editingRow = -1;
    if (ui->addCourseBtn) {
        ui->addCourseBtn->setText("+ Add Course");
    }
}

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

void ManageCoursesPage::onGenerateTimetable() {
    if (courses.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("No Courses");
        msgBox.setText("Please add at least one course first!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        return;
    }
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Generate Timetable");
    msgBox.setText(QString("Timetable generated for %1 course(s)!").arg(courses.size()));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStyleSheet("QLabel{font-size: 9px;} QPushButton{font-size: 9px;}");
    msgBox.exec();
}

void ManageCoursesPage::onViewTimetable() {
    if (courses.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("No Courses");
        msgBox.setText("Please add courses first!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        return;
    }
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("View Timetable");
    msgBox.setText("Viewing generated timetables...");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStyleSheet("QLabel{font-size: 9px;} QPushButton{font-size: 9px;}");
    msgBox.exec();
}
