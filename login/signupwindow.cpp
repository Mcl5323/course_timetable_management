/**
 * SignupWindow Implementation File
 *
 * Implements the user registration dialog functionality.
 */

#include "signupwindow.h"
#include "ui_signupwindow.h"
#include <QMessageBox>

/**
 * SignupWindow Constructor
 *
 * Initializes the signup dialog and sets up button connections.
 */
SignupWindow::SignupWindow(QWidget *parent)
    : QDialog(parent)  // Call parent constructor - makes this a modal dialog
    , ui(new Ui::SignupWindow)  // Create UI components
{
    // Set up the user interface from Qt Designer
    ui->setupUi(this);

    /**
     * Connect button signals to slots
     * These connections handle button click events
     */

    // When Confirm button clicked, validate and register user
    connect(ui->pushButton_Confirm, SIGNAL(clicked()), this, SLOT(on_pushButton_Confirm_clicked()));

    // When Back button clicked, cancel registration and close dialog
    connect(ui->pushButton_Back, SIGNAL(clicked()), this, SLOT(on_pushButton_Back_clicked()));
}

/**
 * SignupWindow Destructor
 * Cleans up UI components to prevent memory leaks
 */
SignupWindow::~SignupWindow()
{
    delete ui;
}

/**
 * Confirm Button Handler (Slot Function)
 *
 * Validates registration input and emits signal if successful.
 *
 * Validation Process:
 * 1. Check all fields are filled
 * 2. Verify passwords match
 * 3. Emit signal to MainWindow
 * 4. Clean up and close dialog
 */
void SignupWindow::on_pushButton_Confirm_clicked()
{
    // Retrieve input from all three fields
    QString studentID = ui->lineEdit_StudentID->text();
    QString password = ui->lineEdit_Password->text();
    QString confirmPassword = ui->lineEdit_ConfirmPassword_2->text();

    /**
     * Validation Step 1: Check for Empty Fields
     * All three fields must be filled before proceeding
     */
    if (studentID.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Input Error");
        msgBox.setText("Please fill in all fields!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        return;  // Exit early if validation fails
    }

    /**
     * Validation Step 2: Password Matching
     * Ensures user typed the same password twice (prevents typos)
     */
    if (password != confirmPassword) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Registration Failed");
        msgBox.setText("Passwords do not match!");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();

        // Clear password fields so user can re-enter
        ui->lineEdit_Password->clear();
        ui->lineEdit_ConfirmPassword_2->clear();
        return;
    }

    /**
     * Emit Signal to MainWindow
     *
     * The 'emit' keyword sends the userRegistered signal with the credentials.
     * MainWindow's on_userRegistered slot will receive this signal and
     * process the registration (check for duplicates and save).
     *
     * This is how the child window (SignupWindow) communicates with
     * the parent window (MainWindow).
     */
    emit userRegistered(studentID, password);

    // Clear all input fields for security and to prepare for next use
    ui->lineEdit_StudentID->clear();
    ui->lineEdit_Password->clear();
    ui->lineEdit_ConfirmPassword_2->clear();

    /**
     * Close the Dialog with Success Status
     *
     * accept() closes the dialog and returns QDialog::Accepted
     * This is the standard way to close a dialog after successful operation
     */
    this->accept();
}

/**
 * Back Button Handler (Slot Function)
 *
 * Cancels registration and closes the dialog.
 * Clears all fields to protect user privacy.
 */
void SignupWindow::on_pushButton_Back_clicked()
{
    // Clear all input fields
    ui->lineEdit_StudentID->clear();
    ui->lineEdit_Password->clear();
    ui->lineEdit_ConfirmPassword_2->clear();

    /**
     * Close Dialog with Rejection Status
     *
     * reject() closes the dialog and returns QDialog::Rejected
     * This indicates the user canceled the operation
     */
    this->reject();
}
