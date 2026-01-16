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
#include <QCryptographicHash>
#include <QCheckBox>
#include <QPushButton>
#include <QSettings>
#include <QInputDialog>

// Hash password using SHA-256 with salt for security
QString MainWindow::hashPassword(const QString &password)
{
    // Add salt to make rainbow table attacks harder
    QString salt = "CourseRegSalt2024";
    QString saltedPassword = salt + password + salt;
    QByteArray hash = QCryptographicHash::hash(saltedPassword.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

// Add user to hash table (returns false if duplicate exists)
// Note: password should already be hashed before calling this function
bool MainWindow::addUser(const QString& studentID, const QString& hashedPassword)
{
    if (registeredUsers.contains(studentID)) {
        return false;  // Duplicate found
    }
    registeredUsers.insert(studentID, hashedPassword);
    return true;
}

// Validate login credentials (returns true if match found)
// Compares hashed password for security
bool MainWindow::validateLogin(const QString &studentID, const QString &password)
{
    if (!registeredUsers.contains(studentID)) {
        return false;  // Student ID not found
    }
    QString storedHashedPassword = registeredUsers.value(studentID);
    QString inputHashedPassword = hashPassword(password);
    return (storedHashedPassword == inputHashedPassword);
}


// Constructor - Initialize login window
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , signupWindow(nullptr)
    , manageCoursesPage(nullptr)
    , registeredUsers()  // HashTable from datastructures.h
    , failedLoginAttempts(0)
    , rememberMeCheckbox(nullptr)
    , togglePasswordBtn(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle("Login");

    loadUsersFromFile();  // Load saved users

    if (!registeredUsers.contains("12345")) {  // Add default test account on first run
        addUser("12345", hashPassword("password123"));
        saveUsersToFile();
    }

    // Create Remember Me checkbox (positioned below password field with more spacing)
    rememberMeCheckbox = new QCheckBox("Remember Me", this);
    rememberMeCheckbox->setGeometry(ui->lineEdit_Password->x(),
                                     ui->lineEdit_Password->y() + ui->lineEdit_Password->height() + 15,
                                     180, 30);
    // Dark mode friendly style - white text with visible checkbox
    rememberMeCheckbox->setStyleSheet(
        "QCheckBox { "
        "   color: white; "
        "   font-size: 12px; "
        "   spacing: 8px; "
        "}"
        "QCheckBox::indicator { "
        "   width: 18px; "
        "   height: 18px; "
        "   border: 2px solid #3498db; "
        "   border-radius: 4px; "
        "   background-color: #2c3e50; "
        "}"
        "QCheckBox::indicator:hover { "
        "   border: 2px solid #5dade2; "
        "   background-color: #34495e; "
        "}"
        "QCheckBox::indicator:checked { "
        "   background-color: #3498db; "
        "   border: 2px solid #3498db; "
        "   image: url(none); "
        "}"
        "QCheckBox::indicator:checked:hover { "
        "   background-color: #5dade2; "
        "   border: 2px solid #5dade2; "
        "}"
    );

    // Create password visibility toggle button
    togglePasswordBtn = new QPushButton("Show", this);
    togglePasswordBtn->setGeometry(ui->lineEdit_Password->x() + ui->lineEdit_Password->width() + 5,
                                    ui->lineEdit_Password->y(),
                                    50, ui->lineEdit_Password->height());
    togglePasswordBtn->setStyleSheet("QPushButton { background-color: #3498db; color: white; border: none; border-radius: 3px; font-size: 10px; }");
    connect(togglePasswordBtn, &QPushButton::clicked, this, &MainWindow::togglePasswordVisibility);

    // Set password field to hide characters
    ui->lineEdit_Password->setEchoMode(QLineEdit::Password);

    // Create Reset Password button (same line as Remember Me, on the right)
    resetPasswordBtn = new QPushButton("Forgot Password?", this);
    resetPasswordBtn->setGeometry(ui->lineEdit_Password->x() + 190,
                                   ui->lineEdit_Password->y() + ui->lineEdit_Password->height() + 15,
                                   120, 30);
    resetPasswordBtn->setStyleSheet(
        "QPushButton { "
        "   color: #3498db; "
        "   background-color: transparent; "
        "   border: none; "
        "   font-size: 11px; "
        "   text-decoration: underline; "
        "}"
        "QPushButton:hover { "
        "   color: #5dade2; "
        "}"
    );
    resetPasswordBtn->setCursor(Qt::PointingHandCursor);
    resetPasswordBtn->hide();  // Hidden by default
    connect(resetPasswordBtn, &QPushButton::clicked, this, &MainWindow::onResetPassword);

    connect(ui->pushButton_Login, SIGNAL(clicked()), this, SLOT(on_submit_clicked()));
    connect(ui->pushButton_SignUp, SIGNAL(clicked()), this, SLOT(on_toggle_mode_clicked()));

    // Load remembered user if exists
    loadRememberedUser();
}

// Destructor - Clean up memory
MainWindow::~MainWindow()
{
    delete ui;
    if (signupWindow) delete signupWindow;
    if (manageCoursesPage) delete manageCoursesPage;
    if (rememberMeCheckbox) delete rememberMeCheckbox;
    if (togglePasswordBtn) delete togglePasswordBtn;
    if (resetPasswordBtn) delete resetPasswordBtn;
    // HashTable destructor auto-called
}

// Toggle password visibility
void MainWindow::togglePasswordVisibility()
{
    if (ui->lineEdit_Password->echoMode() == QLineEdit::Password) {
        ui->lineEdit_Password->setEchoMode(QLineEdit::Normal);
        togglePasswordBtn->setText("Hide");
    } else {
        ui->lineEdit_Password->setEchoMode(QLineEdit::Password);
        togglePasswordBtn->setText("Show");
    }
}

// Save remembered user to settings
void MainWindow::saveRememberedUser()
{
    QSettings settings("CourseRegistration", "Login");
    if (rememberMeCheckbox && rememberMeCheckbox->isChecked()) {
        settings.setValue("rememberedUser", currentUser);
    } else {
        settings.remove("rememberedUser");
    }
}

// Load remembered user from settings
void MainWindow::loadRememberedUser()
{
    QSettings settings("CourseRegistration", "Login");
    QString rememberedUser = settings.value("rememberedUser", "").toString();
    if (!rememberedUser.isEmpty() && ui->lineEdit_StudentID) {
        ui->lineEdit_StudentID->setText(rememberedUser);
        if (rememberMeCheckbox) {
            rememberMeCheckbox->setChecked(true);
        }
        // Focus on password field since student ID is already filled
        if (ui->lineEdit_Password) {
            ui->lineEdit_Password->setFocus();
        }
    }
}


// Login button handler with brute force protection
void MainWindow::on_submit_clicked()
{
    QString studentID = ui->lineEdit_StudentID->text().trimmed();
    QString password = ui->lineEdit_Password->text();

    if (studentID.isEmpty() || password.isEmpty()) {
        UIDialogs::showWarning(this, "Input Error", "Please fill in all fields!");
        return;
    }

    // Reset counter if different user is trying to login
    if (studentID != lastAttemptedUser) {
        failedLoginAttempts = 0;
        lastAttemptedUser = studentID;
    }

    // Check if account is locked due to too many failed attempts
    if (failedLoginAttempts >= MAX_LOGIN_ATTEMPTS) {
        resetPasswordBtn->show();  // Show reset password option
        UIDialogs::showError(this, "Account Locked",
            QString("Too many failed login attempts (%1)!\n\n"
                    "Click 'Forgot Password?' to reset your password.")
                .arg(MAX_LOGIN_ATTEMPTS));
        return;
    }

    if (validateLogin(studentID, password)) {
        currentUser = studentID;
        failedLoginAttempts = 0;  // Reset on successful login
        saveRememberedUser();  // Save if Remember Me is checked
        UIDialogs::showInfo(this, "Login Successful", "Welcome " + studentID + "!");
        switchToManageCoursesPage();
    } else {
        failedLoginAttempts++;
        int remainingAttempts = MAX_LOGIN_ATTEMPTS - failedLoginAttempts;

        if (remainingAttempts > 0) {
            UIDialogs::showError(this, "Login Failed",
                QString("Invalid Student ID or Password!\n\nRemaining attempts: %1")
                    .arg(remainingAttempts));
        } else {
            resetPasswordBtn->show();  // Show reset password option
            UIDialogs::showError(this, "Account Locked",
                QString("Too many failed login attempts!\n\n"
                        "Your account has been locked.\n"
                        "Click 'Forgot Password?' to reset your password."));
        }
        ui->lineEdit_Password->clear();
    }
}

// Reset password function - allows user to set a new password
void MainWindow::onResetPassword()
{
    QString studentID = ui->lineEdit_StudentID->text().trimmed();

    // Check if student ID is entered
    if (studentID.isEmpty()) {
        UIDialogs::showWarning(this, "Reset Password",
            "Please enter your Student ID first, then click 'Forgot Password?'");
        ui->lineEdit_StudentID->setFocus();
        return;
    }

    // Check if student ID exists
    if (!registeredUsers.contains(studentID)) {
        UIDialogs::showError(this, "Reset Password",
            "Student ID not found!\n\nPlease check your Student ID or create a new account.");
        return;
    }

    // Ask for new password
    bool ok;
    QString newPassword = QInputDialog::getText(this, "Reset Password",
        QString("Reset password for: %1\n\nEnter new password (min 6 chars, must have letter + number):").arg(studentID),
        QLineEdit::Password, "", &ok);

    if (!ok || newPassword.isEmpty()) {
        return;  // User cancelled
    }

    // Validate new password strength
    if (newPassword.length() < 6) {
        UIDialogs::showWarning(this, "Weak Password", "Password must be at least 6 characters!");
        return;
    }

    bool hasLetter = false, hasDigit = false;
    for (int i = 0; i < newPassword.length(); ++i) {
        if (newPassword[i].isLetter()) hasLetter = true;
        if (newPassword[i].isDigit()) hasDigit = true;
    }

    if (!hasLetter || !hasDigit) {
        UIDialogs::showWarning(this, "Weak Password",
            "Password must contain at least one letter and one number!");
        return;
    }

    // Confirm new password
    QString confirmPassword = QInputDialog::getText(this, "Confirm Password",
        "Please confirm your new password:",
        QLineEdit::Password, "", &ok);

    if (!ok) {
        return;  // User cancelled
    }

    if (newPassword != confirmPassword) {
        UIDialogs::showError(this, "Password Mismatch", "Passwords do not match!");
        return;
    }

    // Update password in hash table
    QString hashedPassword = hashPassword(newPassword);
    registeredUsers.remove(studentID);
    registeredUsers.insert(studentID, hashedPassword);
    saveUsersToFile();

    // Reset login attempts and hide reset button
    failedLoginAttempts = 0;
    lastAttemptedUser.clear();
    resetPasswordBtn->hide();

    UIDialogs::showInfo(this, "Password Reset Successful",
        "Your password has been reset!\n\nYou can now login with your new password.");

    ui->lineEdit_Password->clear();
    ui->lineEdit_Password->setFocus();
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
// Password is hashed before storing for security
void MainWindow::on_userRegistered(const QString &studentID, const QString &password)
{
    QString hashedPassword = hashPassword(password);
    if (!addUser(studentID, hashedPassword)) {
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

// Switch back to login page (logout) - properly cleans up memory
void MainWindow::switchToLoginPage()
{
    if (manageCoursesPage) {
        manageCoursesPage->hide();
        delete manageCoursesPage;  // Fix memory leak: delete instead of just hiding
        manageCoursesPage = nullptr;
    }
    ui->lineEdit_StudentID->clear();
    ui->lineEdit_Password->clear();
    currentUser.clear();  // Clear current user on logout
    failedLoginAttempts = 0;  // Reset failed attempts on logout
    this->show();
}

// Save all users to users.txt (format: studentID,hashedPassword)
// Note: Passwords are stored as SHA-256 hashes for security
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
