#include "signupwindow.h"
#include "ui_signupwindow.h"
#include <QMessageBox>

// Constructor - Initialize signup dialog
SignupWindow::SignupWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignupWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_Confirm, SIGNAL(clicked()), this, SLOT(on_pushButton_Confirm_clicked()));
    connect(ui->pushButton_Back, SIGNAL(clicked()), this, SLOT(on_pushButton_Back_clicked()));
}

// Destructor
SignupWindow::~SignupWindow()
{
    delete ui;
}

// Confirm button - Validate and register user
void SignupWindow::on_pushButton_Confirm_clicked()
{
    QString studentID = ui->lineEdit_StudentID->text();
    QString password = ui->lineEdit_Password->text();
    QString confirmPassword = ui->lineEdit_ConfirmPassword_2->text();

    if (studentID.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {  // Check empty fields
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Input Error");
        msgBox.setText("Please fill in all fields!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        return;
    }

    if (password != confirmPassword) {  // Check password match
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Registration Failed");
        msgBox.setText("Passwords do not match!");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        ui->lineEdit_Password->clear();
        ui->lineEdit_ConfirmPassword_2->clear();
        return;
    }

    emit userRegistered(studentID, password);  // Send signal to MainWindow
    ui->lineEdit_StudentID->clear();
    ui->lineEdit_Password->clear();
    ui->lineEdit_ConfirmPassword_2->clear();
    this->accept();  // Close dialog
}

// Back button - Cancel and close dialog
void SignupWindow::on_pushButton_Back_clicked()
{
    ui->lineEdit_StudentID->clear();
    ui->lineEdit_Password->clear();
    ui->lineEdit_ConfirmPassword_2->clear();
    this->reject();  // Close dialog
}
