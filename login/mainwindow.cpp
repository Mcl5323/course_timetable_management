/**
 * MainWindow Implementation File
 *
 * This file contains the implementation of the MainWindow class,
 * which manages the login functionality of the application.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "signupwindow.h"
#include "managecoursespage.h"
#include <QMessageBox>

/**
 * MainWindow Constructor
 *
 * Initializes the login window and sets up the user interface.
 * This is called when the application starts.
 *
 * Initialization list explanation:
 * - QMainWindow(parent): Calls parent class constructor
 * - ui(new Ui::MainWindow): Creates UI components from Qt Designer
 * - signupWindow(nullptr): Initially no signup window exists
 * - manageCoursesPage(nullptr): Initially no course page exists
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , signupWindow(nullptr)
    , manageCoursesPage(nullptr)
{
    // Set up the user interface defined in Qt Designer (.ui file)
    ui->setupUi(this);

    // Set the window title that appears in the title bar
    this->setWindowTitle("Login");

    // Initialize the user database with a default test account
    // This allows immediate testing without registration
    // Key: Student ID, Value: Password
    registeredUsers["12345"] = "password123";

    /**
     * Connect button signals to slots (Signal-Slot Mechanism)
     *
     * Qt's signal-slot mechanism is a way to handle events:
     * - Signal: An event that is emitted (e.g., button clicked)
     * - Slot: A function that responds to the signal
     *
     * Syntax: connect(sender, SIGNAL(signal), receiver, SLOT(slot))
     */

    // When Login button is clicked, call on_submit_clicked() to validate credentials
    connect(ui->pushButton_Login, SIGNAL(clicked()), this, SLOT(on_submit_clicked()));

    // When SignUp button is clicked, call on_toggle_mode_clicked() to open signup window
    connect(ui->pushButton_SignUp, SIGNAL(clicked()), this, SLOT(on_toggle_mode_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
    if (signupWindow) {
        delete signupWindow;
    }
    if (manageCoursesPage) {
        delete manageCoursesPage;
    }
}

void MainWindow::on_submit_clicked()
{
    QString studentID = ui->lineEdit_StudentID->text();
    QString password = ui->lineEdit_Password->text();

    // Check if input is empty
    if (studentID.isEmpty() || password.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Input Error");
        msgBox.setText("Please fill in all fields!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        return;
    }

    // Login mode
    if (validateLogin(studentID, password)) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Login Successful");
        msgBox.setText("Welcome " + studentID + "!");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        switchToManageCoursesPage();
    } else {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Login Failed");
        msgBox.setText("Invalid Student ID or Password!");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        ui->lineEdit_Password->clear();
    }
}

void MainWindow::on_toggle_mode_clicked()
{
    // Open SignupWindow dialog
    if (signupWindow == nullptr) {
        signupWindow = new SignupWindow(this);
        connect(signupWindow, SIGNAL(userRegistered(const QString&, const QString&)),
                this, SLOT(on_userRegistered(const QString&, const QString&)));
    }

    signupWindow->exec();
}

bool MainWindow::validateLogin(const QString &studentID, const QString &password)
{
    return registeredUsers.contains(studentID) && registeredUsers[studentID] == password;
}

void MainWindow::on_userRegistered(const QString &studentID, const QString &password)
{
    // Check if user already exists
    if (registeredUsers.contains(studentID)) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Registration Failed");
        msgBox.setText("Student ID already exists!");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        return;
    }

    // Register the new user
    registeredUsers[studentID] = password;
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Registration Successful");
    msgBox.setText("Account created! You can now login.");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStyleSheet("QLabel{font-size: 13px;} QPushButton{font-size: 11px;}");
    msgBox.exec();
}

void MainWindow::switchToManageCoursesPage()
{
    if (manageCoursesPage == nullptr) {
        manageCoursesPage = new ManageCoursesPage(this);
    }

    this->hide();
    manageCoursesPage->show();
}

void MainWindow::switchToLoginPage()
{
    if (manageCoursesPage) {
        manageCoursesPage->hide();
    }
    ui->lineEdit_StudentID->clear();
    ui->lineEdit_Password->clear();
    this->show();
}
