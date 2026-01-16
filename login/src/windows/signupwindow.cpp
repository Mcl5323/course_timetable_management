#include "signupwindow.h"
#include "ui_signupwindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <QLineEdit>
#include <QRegularExpression>

// Constructor - Initialize signup dialog
SignupWindow::SignupWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignupWindow)
    , togglePasswordBtn(nullptr)
    , toggleConfirmPasswordBtn(nullptr)
{
    ui->setupUi(this);

    // Set password fields to hide characters
    ui->lineEdit_Password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_ConfirmPassword_2->setEchoMode(QLineEdit::Password);

    // Create password visibility toggle button
    togglePasswordBtn = new QPushButton("Show", this);
    togglePasswordBtn->setGeometry(ui->lineEdit_Password->x() + ui->lineEdit_Password->width() + 5,
                                    ui->lineEdit_Password->y(),
                                    50, ui->lineEdit_Password->height());
    togglePasswordBtn->setStyleSheet("QPushButton { background-color: #3498db; color: white; border: none; border-radius: 3px; font-size: 10px; }");
    connect(togglePasswordBtn, &QPushButton::clicked, this, &SignupWindow::togglePasswordVisibility);

    // Create confirm password visibility toggle button
    toggleConfirmPasswordBtn = new QPushButton("Show", this);
    toggleConfirmPasswordBtn->setGeometry(ui->lineEdit_ConfirmPassword_2->x() + ui->lineEdit_ConfirmPassword_2->width() + 5,
                                           ui->lineEdit_ConfirmPassword_2->y(),
                                           50, ui->lineEdit_ConfirmPassword_2->height());
    toggleConfirmPasswordBtn->setStyleSheet("QPushButton { background-color: #3498db; color: white; border: none; border-radius: 3px; font-size: 10px; }");
    connect(toggleConfirmPasswordBtn, &QPushButton::clicked, this, &SignupWindow::toggleConfirmPasswordVisibility);

    connect(ui->pushButton_Confirm, SIGNAL(clicked()), this, SLOT(on_pushButton_Confirm_clicked()));
    connect(ui->pushButton_Back, SIGNAL(clicked()), this, SLOT(on_pushButton_Back_clicked()));
}

// Destructor
SignupWindow::~SignupWindow()
{
    delete ui;
    if (togglePasswordBtn) delete togglePasswordBtn;
    if (toggleConfirmPasswordBtn) delete toggleConfirmPasswordBtn;
}

// Toggle password visibility
void SignupWindow::togglePasswordVisibility()
{
    if (ui->lineEdit_Password->echoMode() == QLineEdit::Password) {
        ui->lineEdit_Password->setEchoMode(QLineEdit::Normal);
        togglePasswordBtn->setText("Hide");
    } else {
        ui->lineEdit_Password->setEchoMode(QLineEdit::Password);
        togglePasswordBtn->setText("Show");
    }
}

// Toggle confirm password visibility
void SignupWindow::toggleConfirmPasswordVisibility()
{
    if (ui->lineEdit_ConfirmPassword_2->echoMode() == QLineEdit::Password) {
        ui->lineEdit_ConfirmPassword_2->setEchoMode(QLineEdit::Normal);
        toggleConfirmPasswordBtn->setText("Hide");
    } else {
        ui->lineEdit_ConfirmPassword_2->setEchoMode(QLineEdit::Password);
        toggleConfirmPasswordBtn->setText("Show");
    }
}

// Validate Student ID format (alphanumeric, 5-15 characters)
// Supports formats like: 12345, 242UT244D2, ABC12345
bool SignupWindow::validateStudentID(const QString &studentID)
{
    // Check length (5-15 characters)
    if (studentID.length() < 5 || studentID.length() > 15) {
        return false;
    }

    // Check if all characters are alphanumeric (letters and numbers only)
    QRegularExpression alphanumericRegex("^[A-Za-z0-9]+$");
    return alphanumericRegex.match(studentID).hasMatch();
}

// Validate password strength
// Rules: minimum 6 characters, must contain at least one letter and one number
bool SignupWindow::validatePasswordStrength(const QString &password)
{
    if (password.length() < 6) {
        return false;
    }

    bool hasLetter = false;
    bool hasDigit = false;

    for (int i = 0; i < password.length(); ++i) {
        QChar c = password[i];
        if (c.isLetter()) hasLetter = true;
        if (c.isDigit()) hasDigit = true;
    }

    return hasLetter && hasDigit;
}

// Confirm button - Validate and register user
void SignupWindow::on_pushButton_Confirm_clicked()
{
    QString studentID = ui->lineEdit_StudentID->text().trimmed();
    QString password = ui->lineEdit_Password->text();
    QString confirmPassword = ui->lineEdit_ConfirmPassword_2->text();

    // Check empty fields
    if (studentID.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Input Error");
        msgBox.setText("Please fill in all fields!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        return;
    }

    // Validate Student ID format
    if (!validateStudentID(studentID)) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Invalid Student ID");
        msgBox.setText("Student ID must be 5-15 characters (letters and numbers only)!\n\nExample: 12345, 242UT244D2, ABC12345");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        ui->lineEdit_StudentID->setFocus();
        return;
    }

    // Validate password strength
    if (!validatePasswordStrength(password)) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Weak Password");
        msgBox.setText("Password is too weak!\n\nRequirements:\n- Minimum 6 characters\n- Must contain at least one letter\n- Must contain at least one number\n\nExample: abc123 or Pass1234");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        ui->lineEdit_Password->clear();
        ui->lineEdit_ConfirmPassword_2->clear();
        ui->lineEdit_Password->setFocus();
        return;
    }

    // Check password match
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
