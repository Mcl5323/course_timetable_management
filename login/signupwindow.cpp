#include "signupwindow.h"
#include "ui_signupwindow.h"
#include <QMessageBox>

SignupWindow::SignupWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignupWindow)
{
    ui->setupUi(this);
    
    // Connect button signals
    connect(ui->pushButton_Confirm, SIGNAL(clicked()), this, SLOT(on_pushButton_Confirm_clicked()));
    connect(ui->pushButton_Back, SIGNAL(clicked()), this, SLOT(on_pushButton_Back_clicked()));
}

SignupWindow::~SignupWindow()
{
    delete ui;
}

void SignupWindow::on_pushButton_Confirm_clicked()
{
    QString studentID = ui->lineEdit_StudentID->text();
    QString password = ui->lineEdit_Password->text();
    QString confirmPassword = ui->lineEdit_ConfirmPassword_2->text();
    
    // Check if input is empty
    if (studentID.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Input Error");
        msgBox.setText("Please fill in all fields!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        return;
    }

    // Check if passwords match
    if (password != confirmPassword) {
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
    
    // Emit signal to notify MainWindow about new registration
    emit userRegistered(studentID, password);
    
    // Clear fields
    ui->lineEdit_StudentID->clear();
    ui->lineEdit_Password->clear();
    ui->lineEdit_ConfirmPassword_2->clear();
    
    // Close the dialog
    this->accept();
}

void SignupWindow::on_pushButton_Back_clicked()
{
    // Clear fields and close
    ui->lineEdit_StudentID->clear();
    ui->lineEdit_Password->clear();
    ui->lineEdit_ConfirmPassword_2->clear();
    this->reject();
}
