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

/**
 * MainWindow Destructor
 *
 * Cleans up dynamically allocated memory to prevent memory leaks.
 * This is called automatically when the window is closed.
 */
MainWindow::~MainWindow()
{
    // Delete the UI components
    delete ui;

    // Delete signup window if it was created
    if (signupWindow) {
        delete signupWindow;
    }

    // Delete course management page if it was created
    if (manageCoursesPage) {
        delete manageCoursesPage;
    }
}

/**
 * Login Submit Handler (Slot Function)
 *
 * This function is called when the Login button is clicked.
 * It validates user input and authenticates credentials.
 *
 * Process Flow:
 * 1. Retrieve input from text fields
 * 2. Validate that fields are not empty
 * 3. Check credentials against registered users
 * 4. Show appropriate message and navigate if successful
 */
void MainWindow::on_submit_clicked()
{
    // Get text from input fields
    QString studentID = ui->lineEdit_StudentID->text();
    QString password = ui->lineEdit_Password->text();

    /**
     * Input Validation - Check for empty fields
     * Important: Always validate user input before processing
     */
    if (studentID.isEmpty() || password.isEmpty()) {
        // Create and configure a warning message box
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Input Error");
        msgBox.setText("Please fill in all fields!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();  // Display the message box (blocks until user closes it)
        return;  // Exit the function early
    }

    /**
     * Authentication Process
     * Uses validateLogin() to check if credentials match a registered user
     */
    if (validateLogin(studentID, password)) {
        // Login successful - show welcome message
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Login Successful");
        msgBox.setText("Welcome " + studentID + "!");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();

        // Navigate to the course management page
        switchToManageCoursesPage();
    } else {
        // Login failed - show error message
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Login Failed");
        msgBox.setText("Invalid Student ID or Password!");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();

        // Clear password field for security (but keep student ID for convenience)
        ui->lineEdit_Password->clear();
    }
}

/**
 * Signup Button Handler (Slot Function)
 *
 * Opens the signup window dialog for new user registration.
 * This function uses lazy initialization - only creates the window once.
 */
void MainWindow::on_toggle_mode_clicked()
{
    /**
     * Lazy Initialization Pattern
     * Only create the signup window the first time it's needed
     * This saves memory if the user never clicks signup
     */
    if (signupWindow == nullptr) {
        // Create the signup window with this window as parent
        signupWindow = new SignupWindow(this);

        /**
         * Connect Signal from SignupWindow to Slot in MainWindow
         *
         * This establishes communication between windows:
         * - When SignupWindow emits userRegistered signal (new user created)
         * - MainWindow's on_userRegistered slot is called to save the user
         *
         * This is an example of inter-window communication using signals/slots
         */
        connect(signupWindow, SIGNAL(userRegistered(const QString&, const QString&)),
                this, SLOT(on_userRegistered(const QString&, const QString&)));
    }

    // Open the signup window as a modal dialog (blocks interaction with main window)
    signupWindow->exec();
}

/**
 * Validate Login Credentials
 *
 * Checks if the provided credentials match a registered user.
 *
 * @param studentID: The student ID to check
 * @param password: The password to verify
 * @return true if credentials are valid, false otherwise
 *
 * How it works:
 * 1. Check if studentID exists in the QMap (contains())
 * 2. If exists, check if the stored password matches the provided password
 * 3. Both conditions must be true for successful login
 */
bool MainWindow::validateLogin(const QString &studentID, const QString &password)
{
    return registeredUsers.contains(studentID) && registeredUsers[studentID] == password;
}

/**
 * User Registration Handler (Slot Function)
 *
 * This slot is called when SignupWindow emits the userRegistered signal.
 * It receives the new user's credentials and adds them to the database.
 *
 * @param studentID: New user's student ID
 * @param password: New user's password
 *
 * Process:
 * 1. Check if student ID already exists (prevent duplicates)
 * 2. If new, add to registeredUsers map
 * 3. Show appropriate feedback message
 */
void MainWindow::on_userRegistered(const QString &studentID, const QString &password)
{
    /**
     * Duplicate Check
     * Prevent the same student ID from being registered twice
     */
    if (registeredUsers.contains(studentID)) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Registration Failed");
        msgBox.setText("Student ID already exists!");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        return;
    }

    /**
     * Add New User to Database
     * QMap automatically creates a new entry with the given key-value pair
     */
    registeredUsers[studentID] = password;

    // Show success message
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Registration Successful");
    msgBox.setText("Account created! You can now login.");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStyleSheet("QLabel{font-size: 13px;} QPushButton{font-size: 11px;}");
    msgBox.exec();
}

/**
 * Switch to Course Management Page
 *
 * Called after successful login. Hides the login window and
 * displays the course management interface.
 *
 * Uses lazy initialization like the signup window.
 */
void MainWindow::switchToManageCoursesPage()
{
    // Create course page only if it doesn't exist yet
    if (manageCoursesPage == nullptr) {
        manageCoursesPage = new ManageCoursesPage(this);
    }

    // Hide login window and show course page
    this->hide();
    manageCoursesPage->show();
}

/**
 * Switch Back to Login Page
 *
 * Called when user logs out from the course management page.
 * Clears sensitive input fields and shows the login window.
 */
void MainWindow::switchToLoginPage()
{
    // Hide course management page if it exists
    if (manageCoursesPage) {
        manageCoursesPage->hide();
    }

    // Clear input fields for security (prevent password from remaining visible)
    ui->lineEdit_StudentID->clear();
    ui->lineEdit_Password->clear();

    // Show the login window again
    this->show();
}
