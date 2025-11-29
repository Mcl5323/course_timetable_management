/********************************************************************************
** Form generated from reading UI file 'signupwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNUPWINDOW_H
#define UI_SIGNUPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SignupWindow
{
public:
    QLineEdit *lineEdit_StudentID;
    QPushButton *pushButton_Back;
    QLineEdit *lineEdit_Password;
    QLabel *label;
    QLabel *label_3;
    QPushButton *pushButton_Confirm;
    QLabel *label_2;
    QLabel *label_4;
    QLineEdit *lineEdit_ConfirmPassword_2;

    void setupUi(QDialog *SignupWindow)
    {
        if (SignupWindow->objectName().isEmpty())
            SignupWindow->setObjectName("SignupWindow");
        SignupWindow->resize(1005, 554);
        SignupWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(40, 40, 40);\n"
""));
        lineEdit_StudentID = new QLineEdit(SignupWindow);
        lineEdit_StudentID->setObjectName("lineEdit_StudentID");
        lineEdit_StudentID->setGeometry(QRect(420, 260, 311, 28));
        lineEdit_StudentID->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"font-size:15px;\n"
"}"));
        pushButton_Back = new QPushButton(SignupWindow);
        pushButton_Back->setObjectName("pushButton_Back");
        pushButton_Back->setGeometry(QRect(570, 420, 81, 31));
        pushButton_Back->setStyleSheet(QString::fromUtf8("font: 10pt \"Segoe UI\";"));
        lineEdit_Password = new QLineEdit(SignupWindow);
        lineEdit_Password->setObjectName("lineEdit_Password");
        lineEdit_Password->setGeometry(QRect(420, 310, 311, 28));
        lineEdit_Password->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"font-size:15px;\n"
"}"));
        lineEdit_Password->setEchoMode(QLineEdit::EchoMode::Password);
        label = new QLabel(SignupWindow);
        label->setObjectName("label");
        label->setEnabled(true);
        label->setGeometry(QRect(410, 190, 271, 51));
        label->setStyleSheet(QString::fromUtf8("QLabel{\n"
"font-size:30px\n"
"}"));
        label_3 = new QLabel(SignupWindow);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(290, 310, 111, 21));
        label_3->setStyleSheet(QString::fromUtf8("font: 14pt \"Segoe UI\";"));
        pushButton_Confirm = new QPushButton(SignupWindow);
        pushButton_Confirm->setObjectName("pushButton_Confirm");
        pushButton_Confirm->setGeometry(QRect(420, 420, 81, 31));
        pushButton_Confirm->setStyleSheet(QString::fromUtf8("font: 10pt \"Segoe UI\";"));
        label_2 = new QLabel(SignupWindow);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(290, 260, 121, 21));
        label_2->setStyleSheet(QString::fromUtf8("font: 14pt \"Segoe UI\";"));
        label_4 = new QLabel(SignupWindow);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(290, 360, 221, 21));
        label_4->setStyleSheet(QString::fromUtf8("font: 14pt \"Segoe UI\";"));
        lineEdit_ConfirmPassword_2 = new QLineEdit(SignupWindow);
        lineEdit_ConfirmPassword_2->setObjectName("lineEdit_ConfirmPassword_2");
        lineEdit_ConfirmPassword_2->setGeometry(QRect(500, 360, 231, 28));
        lineEdit_ConfirmPassword_2->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"font-size:15px;\n"
"}"));
        lineEdit_ConfirmPassword_2->setEchoMode(QLineEdit::EchoMode::Password);

        retranslateUi(SignupWindow);

        QMetaObject::connectSlotsByName(SignupWindow);
    } // setupUi

    void retranslateUi(QDialog *SignupWindow)
    {
        SignupWindow->setWindowTitle(QCoreApplication::translate("SignupWindow", "Sign Up", nullptr));
        pushButton_Back->setText(QCoreApplication::translate("SignupWindow", "Back", nullptr));
        label->setText(QCoreApplication::translate("SignupWindow", "Sign Up Page", nullptr));
        label_3->setText(QCoreApplication::translate("SignupWindow", "Password\357\274\232", nullptr));
        pushButton_Confirm->setText(QCoreApplication::translate("SignupWindow", "Confirm", nullptr));
        label_2->setText(QCoreApplication::translate("SignupWindow", "Student ID\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("SignupWindow", "Confirm Password\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SignupWindow: public Ui_SignupWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNUPWINDOW_H
