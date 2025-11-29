/********************************************************************************
** Form generated from reading UI file 'managecoursespage.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MANAGECOURSESPAGE_H
#define UI_MANAGECOURSESPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_ManageCoursesPage
{
public:
    QLabel *titleLabel;
    QLabel *addNewCourseLabel;
    QLineEdit *courseNameInput;
    QComboBox *dayCombo;
    QLabel *startTimeLabel_3;
    QLabel *dayLabel;
    QLabel *label;
    QLineEdit *classroomInput;
    QLabel *classroomLabel;
    QPushButton *addCourseBtn;
    QLabel *courseCountLabel;
    QPushButton *generateBtn;
    QPushButton *viewTimetableBtn;
    QLabel *label_2;
    QComboBox *startTimeLabel;
    QComboBox *endTimeInput;
    QPushButton *logupbutton;
    QTableWidget *coursetable;

    void setupUi(QDialog *ManageCoursesPage)
    {
        if (ManageCoursesPage->objectName().isEmpty())
            ManageCoursesPage->setObjectName("ManageCoursesPage");
        ManageCoursesPage->resize(1606, 751);
        ManageCoursesPage->setStyleSheet(QString::fromUtf8("background-color: #0D1B2A;\n"
""));
        titleLabel = new QLabel(ManageCoursesPage);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setGeometry(QRect(530, 10, 401, 71));
        titleLabel->setStyleSheet(QString::fromUtf8("font: 700 32pt \"Segoe UI\";\n"
"\n"
"\n"
"\n"
""));
        addNewCourseLabel = new QLabel(ManageCoursesPage);
        addNewCourseLabel->setObjectName("addNewCourseLabel");
        addNewCourseLabel->setGeometry(QRect(130, 70, 291, 41));
        addNewCourseLabel->setStyleSheet(QString::fromUtf8("\n"
"font: 700 italic 20pt \"Segoe UI\";\n"
""));
        courseNameInput = new QLineEdit(ManageCoursesPage);
        courseNameInput->setObjectName("courseNameInput");
        courseNameInput->setGeometry(QRect(170, 130, 1171, 41));
        courseNameInput->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: #1E293B;\n"
"    color: #E2E8F0;\n"
"    border: 1px solid #334155;\n"
"    border-radius: 4px;\n"
"    padding: 8px;\n"
"    font-size: 12px;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 2px solid #3B82F6;   \n"
"    background-color: #273449;   \n"
"}"));
        dayCombo = new QComboBox(ManageCoursesPage);
        dayCombo->setObjectName("dayCombo");
        dayCombo->setGeometry(QRect(170, 190, 1171, 41));
        dayCombo->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    background-color: #1E293B;\n"
"    color: #E2E8F0;\n"
"    border: 1px solid #334155;\n"
"    border-radius: 4px;\n"
"    padding: 8px;\n"
"    font-size: 12px;\n"
"}\n"
"\n"
"/* \345\261\225\345\274\200\351\241\271\347\233\256 */\n"
"QComboBox QAbstractItemView {\n"
"    background-color: #1E293B;\n"
"    color: #E2E8F0;\n"
"    selection-background-color: #3B82F6;\n"
"    selection-color: white;\n"
"}\n"
"\n"
"QComboBox:focus {\n"
"    border: 2px solid #3B82F6;\n"
"    background-color: #273449;\n"
"}"));
        startTimeLabel_3 = new QLabel(ManageCoursesPage);
        startTimeLabel_3->setObjectName("startTimeLabel_3");
        startTimeLabel_3->setGeometry(QRect(60, 260, 101, 21));
        startTimeLabel_3->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        dayLabel = new QLabel(ManageCoursesPage);
        dayLabel->setObjectName("dayLabel");
        dayLabel->setGeometry(QRect(120, 200, 41, 21));
        dayLabel->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";\n"
""));
        label = new QLabel(ManageCoursesPage);
        label->setObjectName("label");
        label->setGeometry(QRect(700, 260, 91, 21));
        label->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        classroomInput = new QLineEdit(ManageCoursesPage);
        classroomInput->setObjectName("classroomInput");
        classroomInput->setGeometry(QRect(170, 310, 1171, 41));
        classroomInput->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: #1E293B;\n"
"    color: #E2E8F0;\n"
"    border: 1px solid #334155;\n"
"    border-radius: 4px;\n"
"    padding: 8px;\n"
"    font-size: 12px;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 2px solid #3B82F6;   \n"
"    background-color: #273449;   \n"
"}"));
        classroomLabel = new QLabel(ManageCoursesPage);
        classroomLabel->setObjectName("classroomLabel");
        classroomLabel->setGeometry(QRect(60, 320, 101, 21));
        classroomLabel->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        addCourseBtn = new QPushButton(ManageCoursesPage);
        addCourseBtn->setObjectName("addCourseBtn");
        addCourseBtn->setGeometry(QRect(170, 370, 1171, 51));
        addCourseBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #2ecc71;\n"
"    color: white;\n"
"    font-weight: bold;\n"
"    font-size: 14px;\n"
"    border-radius: 5px;\n"
"    padding: 10px;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #27ae60;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #1e874b;\n"
"}"));
        courseCountLabel = new QLabel(ManageCoursesPage);
        courseCountLabel->setObjectName("courseCountLabel");
        courseCountLabel->setGeometry(QRect(490, 420, 631, 71));
        courseCountLabel->setStyleSheet(QString::fromUtf8("font: 700 32pt \"Segoe UI\";"));
        generateBtn = new QPushButton(ManageCoursesPage);
        generateBtn->setObjectName("generateBtn");
        generateBtn->setGeometry(QRect(170, 670, 571, 61));
        generateBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #9b59b6;\n"
"    color: white;\n"
"    font-weight: bold;\n"
"    font-size: 14px;\n"
"    border-radius: 5px;\n"
"    padding: 10px;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #8e44ad;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #7d3c98;\n"
"}"));
        viewTimetableBtn = new QPushButton(ManageCoursesPage);
        viewTimetableBtn->setObjectName("viewTimetableBtn");
        viewTimetableBtn->setGeometry(QRect(760, 670, 591, 61));
        viewTimetableBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #ff8c42;\n"
"    color: white;\n"
"    font-weight: bold;\n"
"    font-size: 14px;\n"
"    border-radius: 5px;\n"
"    padding: 10px;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #e67e22;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #d35400;\n"
"}"));
        label_2 = new QLabel(ManageCoursesPage);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(30, 140, 131, 21));
        label_2->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        startTimeLabel = new QComboBox(ManageCoursesPage);
        startTimeLabel->setObjectName("startTimeLabel");
        startTimeLabel->setGeometry(QRect(170, 250, 521, 41));
        startTimeLabel->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    background-color: #1E293B;\n"
"    color: #E2E8F0;\n"
"    border: 1px solid #334155;\n"
"    border-radius: 4px;\n"
"    padding: 8px;\n"
"    font-size: 12px;\n"
"}\n"
"\n"
"/* \345\261\225\345\274\200\351\241\271\347\233\256 */\n"
"QComboBox QAbstractItemView {\n"
"    background-color: #1E293B;\n"
"    color: #E2E8F0;\n"
"    selection-background-color: #3B82F6;\n"
"    selection-color: white;\n"
"}\n"
"\n"
"QComboBox:focus {\n"
"    border: 2px solid #3B82F6;\n"
"    background-color: #273449;\n"
"}"));
        endTimeInput = new QComboBox(ManageCoursesPage);
        endTimeInput->setObjectName("endTimeInput");
        endTimeInput->setGeometry(QRect(800, 250, 541, 41));
        endTimeInput->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    background-color: #1E293B;\n"
"    color: #E2E8F0;\n"
"    border: 1px solid #334155;\n"
"    border-radius: 4px;\n"
"    padding: 8px;\n"
"    font-size: 12px;\n"
"}\n"
"\n"
"/* \345\261\225\345\274\200\351\241\271\347\233\256 */\n"
"QComboBox QAbstractItemView {\n"
"    background-color: #1E293B;\n"
"    color: #E2E8F0;\n"
"    selection-background-color: #3B82F6;\n"
"    selection-color: white;\n"
"}\n"
"\n"
"QComboBox:focus {\n"
"    border: 2px solid #3B82F6;\n"
"    background-color: #273449;\n"
"}"));
        logupbutton = new QPushButton(ManageCoursesPage);
        logupbutton->setObjectName("logupbutton");
        logupbutton->setGeometry(QRect(1230, 50, 111, 31));
        logupbutton->setStyleSheet(QString::fromUtf8("font: 350 9pt \"Segoe UI\";"));
        coursetable = new QTableWidget(ManageCoursesPage);
        if (coursetable->columnCount() < 6)
            coursetable->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        coursetable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        coursetable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        coursetable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        coursetable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        coursetable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        coursetable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        if (coursetable->rowCount() < 3)
            coursetable->setRowCount(3);
        coursetable->setObjectName("coursetable");
        coursetable->setGeometry(QRect(170, 500, 1171, 151));
        coursetable->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
"background-color: #f0f0f0;\n"
"alternate-background-color: #e0e0e0;\n"
"gridline-color: #d0d0d0;\n"
"font-family: Arial;\n"
"font-size: 12px;\n"
"}\n"
"\n"
"QTableWidget::item {\n"
"padding: 5px;\n"
"border: 1px solid #d0d0d0;\n"
"}\n"
"\n"
"QTableWidget::item:selected {\n"
"background-color: #a0a0a0;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"background-color: #4CAF50;\n"
"color: white;\n"
"padding: 5px;\n"
"border: 1px solid #4CAF50;\n"
"font-weight: bold;\n"
"}"));

        retranslateUi(ManageCoursesPage);

        QMetaObject::connectSlotsByName(ManageCoursesPage);
    } // setupUi

    void retranslateUi(QDialog *ManageCoursesPage)
    {
        ManageCoursesPage->setWindowTitle(QCoreApplication::translate("ManageCoursesPage", "Dialog", nullptr));
        titleLabel->setText(QCoreApplication::translate("ManageCoursesPage", "Manage Course", nullptr));
        addNewCourseLabel->setText(QCoreApplication::translate("ManageCoursesPage", "+ Add New Course", nullptr));
        startTimeLabel_3->setText(QCoreApplication::translate("ManageCoursesPage", "Start Time:", nullptr));
        dayLabel->setText(QCoreApplication::translate("ManageCoursesPage", "Day:", nullptr));
        label->setText(QCoreApplication::translate("ManageCoursesPage", "End Time:", nullptr));
        classroomLabel->setText(QCoreApplication::translate("ManageCoursesPage", "Classroom:", nullptr));
        addCourseBtn->setText(QCoreApplication::translate("ManageCoursesPage", "+ Add Course", nullptr));
        courseCountLabel->setText(QCoreApplication::translate("ManageCoursesPage", "View & Manage Course", nullptr));
        generateBtn->setText(QCoreApplication::translate("ManageCoursesPage", "\342\217\261\357\270\217 Generate Timetable", nullptr));
        viewTimetableBtn->setText(QCoreApplication::translate("ManageCoursesPage", "\360\237\223\205 View Generated Timetables", nullptr));
        label_2->setText(QCoreApplication::translate("ManageCoursesPage", "Course Name\357\274\232", nullptr));
        logupbutton->setText(QCoreApplication::translate("ManageCoursesPage", "Logup", nullptr));
        QTableWidgetItem *___qtablewidgetitem = coursetable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("ManageCoursesPage", "Select", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = coursetable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("ManageCoursesPage", "Course Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = coursetable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("ManageCoursesPage", "Day", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = coursetable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("ManageCoursesPage", "Time", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = coursetable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("ManageCoursesPage", "Classroom", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = coursetable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("ManageCoursesPage", "Actions", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ManageCoursesPage: public Ui_ManageCoursesPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MANAGECOURSESPAGE_H
