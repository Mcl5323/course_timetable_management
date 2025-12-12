/********************************************************************************
** Form generated from reading UI file 'timetable.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIMETABLE_H
#define UI_TIMETABLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TIMETABLE
{
public:
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QWidget *widget;
    QPushButton *prevPageBtn;
    QPushButton *nextPageBtn;
    QLabel *pageNumberLabel;
    QPushButton *saveAsBtn;
    QLabel *titleLabel;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget_2;
    QTableWidget *timetableTable;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QWidget *widget_3;
    QLabel *totalCourseLabel;
    QLabel *totalHoursLabel;
    QLabel *conflictsLabel;
    QPushButton *backBtn;
    QPushButton *deleteBtn;

    void setupUi(QDialog *TIMETABLE)
    {
        if (TIMETABLE->objectName().isEmpty())
            TIMETABLE->setObjectName("TIMETABLE");
        TIMETABLE->resize(1606, 770);
        TIMETABLE->setStyleSheet(QString::fromUtf8("background-color: #FFFFFF;"));
        horizontalLayoutWidget = new QWidget(TIMETABLE);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(20, 20, 1541, 131));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(horizontalLayoutWidget);
        widget->setObjectName("widget");
        widget->setStyleSheet(QString::fromUtf8("QWidget {\n"
"  background-color: #0D1B2A;\n"
"    border-bottom: 2px solid #2d5a8c;\n"
"}"));
        prevPageBtn = new QPushButton(widget);
        prevPageBtn->setObjectName("prevPageBtn");
        prevPageBtn->setGeometry(QRect(1220, 80, 41, 31));
        prevPageBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #F5F5F5;\n"
"    color: #333333;\n"
"    border: 1px solid #CCCCCC;\n"
"    border-radius: 4px;\n"
"    padding: 6px 12px;\n"
"    font-size: 12px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #E8E8E8;\n"
"    border: 1px solid #2d5a8c;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #D8D8D8;\n"
"}"));
        nextPageBtn = new QPushButton(widget);
        nextPageBtn->setObjectName("nextPageBtn");
        nextPageBtn->setGeometry(QRect(1320, 80, 41, 29));
        nextPageBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #F5F5F5;\n"
"    color: #333333;\n"
"    border: 1px solid #CCCCCC;\n"
"    border-radius: 4px;\n"
"    padding: 6px 12px;\n"
"    font-size: 12px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #E8E8E8;\n"
"    border: 1px solid #2d5a8c;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #D8D8D8;\n"
"}"));
        pageNumberLabel = new QLabel(widget);
        pageNumberLabel->setObjectName("pageNumberLabel");
        pageNumberLabel->setGeometry(QRect(1270, 80, 51, 31));
        pageNumberLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: #FFF;\n"
"    font-weight: bold;\n"
"    font-size: 14px;\n"
"    background-color: transparent;\n"
"}"));
        pageNumberLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        saveAsBtn = new QPushButton(widget);
        saveAsBtn->setObjectName("saveAsBtn");
        saveAsBtn->setGeometry(QRect(1370, 80, 131, 29));
        saveAsBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #2d5a8c;\n"
"    color: #FFFFFF;\n"
"    border: none;\n"
"    border-radius: 4px;\n"
"    padding: 8px 24px;\n"
"    font-weight: bold;\n"
" \n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #3d6fa8;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #1d4a7c;\n"
"}"));
        titleLabel = new QLabel(widget);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setGeometry(QRect(520, 20, 461, 81));
        titleLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: #FFF;\n"
"    font: bold 32pt \"Segoe UI\";\n"
"    background-color: transparent;\n"
"    padding: 15px 20px;\n"
"}"));

        horizontalLayout->addWidget(widget);

        horizontalLayoutWidget_2 = new QWidget(TIMETABLE);
        horizontalLayoutWidget_2->setObjectName("horizontalLayoutWidget_2");
        horizontalLayoutWidget_2->setGeometry(QRect(20, 170, 1541, 451));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        widget_2 = new QWidget(horizontalLayoutWidget_2);
        widget_2->setObjectName("widget_2");
        widget_2->setStyleSheet(QString::fromUtf8("background-color: #0D1B2A;\n"
"border-bottom: 1px solid #E0E0E0;"));
        timetableTable = new QTableWidget(widget_2);
        if (timetableTable->columnCount() < 15)
            timetableTable->setColumnCount(15);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        timetableTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        timetableTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        timetableTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        timetableTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        timetableTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        timetableTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        timetableTable->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        timetableTable->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        timetableTable->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        timetableTable->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        timetableTable->setHorizontalHeaderItem(10, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        timetableTable->setHorizontalHeaderItem(11, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        timetableTable->setHorizontalHeaderItem(12, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        timetableTable->setHorizontalHeaderItem(13, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        timetableTable->setHorizontalHeaderItem(14, __qtablewidgetitem14);
        if (timetableTable->rowCount() < 7)
            timetableTable->setRowCount(7);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        timetableTable->setVerticalHeaderItem(0, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        timetableTable->setVerticalHeaderItem(1, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        timetableTable->setVerticalHeaderItem(2, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        timetableTable->setVerticalHeaderItem(3, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        timetableTable->setVerticalHeaderItem(4, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        timetableTable->setVerticalHeaderItem(5, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        timetableTable->setVerticalHeaderItem(6, __qtablewidgetitem21);
        timetableTable->setObjectName("timetableTable");
        timetableTable->setGeometry(QRect(10, 20, 1511, 380));
        timetableTable->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
"    background-color: #0D1B2A;\n"
"    alternate-background-color: #253445;\n"
"    gridline-color: #2d5a8c;\n"
"    border: 1px solid #2d5a8c;\n"
"}\n"
"\n"
"QTableWidget::item {\n"
"    padding: 8px;\n"
"    color: #FFFFFF;\n"
"    background-color: #1a2535;\n"
"    border: 1px solid #2d5a8c;\n"
"}\n"
"\n"
"QTableWidget::item:selected {\n"
"    background-color: #2d5a8c;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: #253445;\n"
"    color: #FFFFFF;\n"
"    padding: 8px;\n"
"    border: 1px solid #2d5a8c;\n"
"    font-weight: bold;\n"
"    font-size: 10pt;\n"
"}"));

        horizontalLayout_2->addWidget(widget_2);

        horizontalLayoutWidget_3 = new QWidget(TIMETABLE);
        horizontalLayoutWidget_3->setObjectName("horizontalLayoutWidget_3");
        horizontalLayoutWidget_3->setGeometry(QRect(20, 630, 1541, 131));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        widget_3 = new QWidget(horizontalLayoutWidget_3);
        widget_3->setObjectName("widget_3");
        widget_3->setStyleSheet(QString::fromUtf8("background-color: #0D1B2A;\n"
"border-bottom: 1px solid #E0E0E0;"));
        totalCourseLabel = new QLabel(widget_3);
        totalCourseLabel->setObjectName("totalCourseLabel");
        totalCourseLabel->setGeometry(QRect(10, 10, 211, 51));
        totalCourseLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: #2d5a8c;\n"
"    font: bold 18pt \"Segoe UI\";\n"
"    background-color: transparent;\n"
"    padding: 10px;\n"
"}"));
        totalHoursLabel = new QLabel(widget_3);
        totalHoursLabel->setObjectName("totalHoursLabel");
        totalHoursLabel->setGeometry(QRect(260, 10, 201, 51));
        totalHoursLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: #2d5a8c;\n"
"    font: bold 18pt \"Segoe UI\";\n"
"    background-color: transparent;\n"
"    padding: 10px;\n"
"}"));
        conflictsLabel = new QLabel(widget_3);
        conflictsLabel->setObjectName("conflictsLabel");
        conflictsLabel->setGeometry(QRect(500, 10, 191, 51));
        conflictsLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: #2d5a8c;\n"
"    font: bold 18pt \"Segoe UI\";\n"
"    background-color: transparent;\n"
"    padding: 10px;\n"
"}"));
        backBtn = new QPushButton(widget_3);
        backBtn->setObjectName("backBtn");
        backBtn->setGeometry(QRect(1170, 50, 141, 51));
        backBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #4472C4;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 4px;\n"
"    padding: 0px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #3A5BA8;\n"
"}"));
        deleteBtn = new QPushButton(widget_3);
        deleteBtn->setObjectName("deleteBtn");
        deleteBtn->setGeometry(QRect(1340, 50, 141, 51));
        deleteBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #4472C4;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 4px;\n"
"    padding: 0px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #3A5BA8;\n"
"}"));

        horizontalLayout_3->addWidget(widget_3);


        retranslateUi(TIMETABLE);

        QMetaObject::connectSlotsByName(TIMETABLE);
    } // setupUi

    void retranslateUi(QDialog *TIMETABLE)
    {
        TIMETABLE->setWindowTitle(QCoreApplication::translate("TIMETABLE", "Dialog", nullptr));
        prevPageBtn->setText(QCoreApplication::translate("TIMETABLE", "<", nullptr));
        nextPageBtn->setText(QCoreApplication::translate("TIMETABLE", ">", nullptr));
        pageNumberLabel->setText(QCoreApplication::translate("TIMETABLE", "1/1", nullptr));
        saveAsBtn->setText(QCoreApplication::translate("TIMETABLE", "Save as", nullptr));
        titleLabel->setText(QCoreApplication::translate("TIMETABLE", "View Timetable", nullptr));
        QTableWidgetItem *___qtablewidgetitem = timetableTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("TIMETABLE", "8.00am", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = timetableTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("TIMETABLE", "9.00am", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = timetableTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("TIMETABLE", "10.00am", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = timetableTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("TIMETABLE", "11.00am", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = timetableTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("TIMETABLE", "12.00am", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = timetableTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("TIMETABLE", "1.00pm", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = timetableTable->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("TIMETABLE", "2.00pm", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = timetableTable->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("TIMETABLE", "3.00pm", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = timetableTable->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("TIMETABLE", "4.00pm", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = timetableTable->horizontalHeaderItem(9);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("TIMETABLE", "5.00pm", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = timetableTable->horizontalHeaderItem(10);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("TIMETABLE", "6.00pm", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = timetableTable->horizontalHeaderItem(11);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("TIMETABLE", "7.00pm", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = timetableTable->horizontalHeaderItem(12);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("TIMETABLE", "8.00pm", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = timetableTable->horizontalHeaderItem(13);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("TIMETABLE", "9.00pm", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = timetableTable->horizontalHeaderItem(14);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("TIMETABLE", "10.00pm", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = timetableTable->verticalHeaderItem(0);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("TIMETABLE", "Monday", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = timetableTable->verticalHeaderItem(1);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("TIMETABLE", "Tuesday", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = timetableTable->verticalHeaderItem(2);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("TIMETABLE", "Wednesday", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = timetableTable->verticalHeaderItem(3);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("TIMETABLE", "Thursday", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = timetableTable->verticalHeaderItem(4);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("TIMETABLE", "Friday", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = timetableTable->verticalHeaderItem(5);
        ___qtablewidgetitem20->setText(QCoreApplication::translate("TIMETABLE", "Saturday", nullptr));
        QTableWidgetItem *___qtablewidgetitem21 = timetableTable->verticalHeaderItem(6);
        ___qtablewidgetitem21->setText(QCoreApplication::translate("TIMETABLE", "Sunday", nullptr));
        totalCourseLabel->setText(QCoreApplication::translate("TIMETABLE", "Total Course: 0", nullptr));
        totalHoursLabel->setText(QCoreApplication::translate("TIMETABLE", "Total Hours: 0", nullptr));
        conflictsLabel->setText(QCoreApplication::translate("TIMETABLE", "Conflicts: 0", nullptr));
        backBtn->setText(QCoreApplication::translate("TIMETABLE", "Back", nullptr));
        deleteBtn->setText(QCoreApplication::translate("TIMETABLE", "Delete", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TIMETABLE: public Ui_TIMETABLE {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIMETABLE_H
