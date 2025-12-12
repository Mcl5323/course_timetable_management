#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "signupwindow.h"
#include "managecoursespage.h"
#include "uistyles.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QCoreApplication>
#include <QStandardPaths>

// Add user to hash table (returns false if duplicate exists)
bool MainWindow::addUser(const QString& studentID, const QString& password)
{
    if (registeredUsers.contains(studentID)) {
        return false;  // Duplicate found
    }
    registeredUsers.insert(studentID, password);
    return true;
}

// Validate login credentials (returns true if match found)
bool MainWindow::validateLogin(const QString &studentID, const QString &password)
{
    if (!registeredUsers.contains(studentID)) {
        return false;  // Student ID not found
    }
    QString storedPassword = registeredUsers.value(studentID);
    return (storedPassword == password);
}


// Constructor - Initialize login window
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , signupWindow(nullptr)
    , manageCoursesPage(nullptr)
    , registeredUsers()  // HashTable from datastructures.h
{
    ui->setupUi(this);
    this->setWindowTitle("Login");

    loadUsersFromFile();  // Load saved users

    if (!registeredUsers.contains("12345")) {  // Add default test account on first run
        addUser("12345", "password123");
        saveUsersToFile();
    }

    connect(ui->pushButton_Login, SIGNAL(clicked()), this, SLOT(on_submit_clicked()));
    connect(ui->pushButton_SignUp, SIGNAL(clicked()), this, SLOT(on_toggle_mode_clicked()));
}

// Destructor - Clean up memory
MainWindow::~MainWindow()
{
    delete ui;
    if (signupWindow) delete signupWindow;
    if (manageCoursesPage) delete manageCoursesPage;
    // HashTable destructor auto-called
}


// Login button handler
void MainWindow::on_submit_clicked()
{
    QString studentID = ui->lineEdit_StudentID->text();
    QString password = ui->lineEdit_Password->text();

    if (studentID.isEmpty() || password.isEmpty()) {
        UIDialogs::showWarning(this, "Input Error", "Please fill in all fields!");
        return;
    }

    if (validateLogin(studentID, password)) {
        currentUser = studentID;
        UIDialogs::showInfo(this, "Login Successful", "Welcome " + studentID + "!");
        switchToManageCoursesPage();
    } else {
        UIDialogs::showError(this, "Login Failed", "Invalid Student ID or Password!");
        ui->lineEdit_Password->clear();
    }
}

// SignUp button handler - Open signup window
void MainWindow::on_toggle_mode_clicked()
{
    if (signupWindow == nullptr) {  // Lazy initialization
        signupWindow = new SignupWindow(this);
        connect(signupWindow, SIGNAL(userRegistered(const QString&, const QString&)),
                this, SLOT(on_userRegistered(const QString&, const QString&)));
    }
    signupWindow->exec();
}

// Handle user registration from SignupWindow
void MainWindow::on_userRegistered(const QString &studentID, const QString &password)
{
    if (!addUser(studentID, password)) {
        UIDialogs::showError(this, "Registration Failed", "Student ID already exists!");
        return;
    }
    saveUsersToFile();
    UIDialogs::showInfo(this, "Registration Successful", "Account created! You can now login.");
}


// Switch to course management page after successful login
void MainWindow::switchToManageCoursesPage()
{
    if (manageCoursesPage != nullptr) {
        delete manageCoursesPage;
    }
    manageCoursesPage = new ManageCoursesPage(currentUser, this);
    this->hide();
    manageCoursesPage->show();
}

// Switch back to login page (logout)
void MainWindow::switchToLoginPage()
{
    if (manageCoursesPage) {
        manageCoursesPage->hide();
    }
    ui->lineEdit_StudentID->clear();
    ui->lineEdit_Password->clear();
    this->show();
}

// Save all users to users.txt (format: studentID,password)
void MainWindow::saveUsersToFile()
{
    QString appDir = QCoreApplication::applicationDirPath();
    QString filePath = appDir + "/users.txt";
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        UIDialogs::showWarning(this, "File Error", "Could not save user data to file!");
        return;
    }

    QTextStream out(&file);
    LinkedList<QString> studentIDs = registeredUsers.keys();  // Get all keys from HashTable

    for (int i = 0; i < studentIDs.size(); ++i) {
        QString studentID = studentIDs[i];
        QString password = registeredUsers.value(studentID);
        out << studentID << "," << password << "\n";  // Write in CSV format
    }

    file.close();
}

// Load users from users.txt on startup
void MainWindow::loadUsersFromFile()
{
    QString appDir = QCoreApplication::applicationDirPath();
    QString filePath = appDir + "/users.txt";
    QFile file(filePath);

    if (!file.exists()) return;  // Skip if file doesn't exist (first run)

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        UIDialogs::showWarning(this, "File Error", "Could not load user data from file!");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(",");
        if (parts.size() == 2) {
            QString studentID = parts[0].trimmed();
            QString password = parts[1].trimmed();
            addUser(studentID, password);
        }
    }

    file.close();
}
