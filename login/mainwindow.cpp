/**
 * ============================================================================
 * MainWindow Implementation File
 * ============================================================================
 *
 * This file implements the MainWindow class for login functionality
 * Uses a manually implemented hash table to store user data
 *
 * Code Organization:
 * 1. Hash Table Operations    - User data storage and validation
 * 2. Initialization & Cleanup  - Setup and memory management
 * 3. UI Event Handlers         - Button clicks and user interactions
 * 4. Page Navigation           - Switching between login and main page
 * ============================================================================
 */

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


// ============================================================================
// PART 1: HASH TABLE OPERATIONS
// Purpose: Manage user data storage, search, and validation
// Note: These are NOT from Qt UI - manually implemented data structures
// ============================================================================

/**
 * Hash Function
 * Purpose: Convert student ID string into a bucket index
 * Algorithm: Sum all character Unicode values, then modulo by capacity
 *
 * @param key: Student ID to hash
 * @return: Bucket index (0 to capacity-1)
 */
int MainWindow::hashFunction(const QString& key)
{
    int hash = 0;

    // Sum up all character Unicode values
    for (int i = 0; i < key.length(); i++) {
        hash += key[i].unicode();
    }

    // Use modulo to keep index within array bounds
    return hash % registeredUsers->capacity;
}

/**
 * Add User to Hash Table
 * Purpose: Insert new user into hash table (used for registration)
 * Features: Auto-check for duplicates, uses linked list for collision handling
 *
 * @param studentID: New user's student ID
 * @param password: New user's password
 * @return: true if added successfully, false if duplicate exists
 */
bool MainWindow::addUser(const QString& studentID, const QString& password)
{
    // Step 1: Calculate which bucket this user belongs to
    int index = hashFunction(studentID);

    // Step 2: Get the head of the linked list at this bucket
    UserNode* current = registeredUsers->buckets[index];

    // Step 3: Traverse linked list to check for duplicate student ID
    while (current != nullptr) {
        if (current->studentID == studentID) {
            return false;  // Duplicate found - registration fails
        }
        current = current->next;
    }

    // Step 4: No duplicate - create new node
    UserNode* newNode = new UserNode(studentID, password);

    // Step 5: Insert at head of linked list (faster than tail insertion)
    newNode->next = registeredUsers->buckets[index];
    registeredUsers->buckets[index] = newNode;

    return true;  // Successfully added
}

/**
 * Validate Login Credentials
 * Purpose: Check if entered student ID and password match a registered user
 * Process: Use hash function to locate bucket -> traverse linked list to find match
 *
 * @param studentID: Student ID to validate
 * @param password: Password to validate
 * @return: true if valid, false if invalid
 */
bool MainWindow::validateLogin(const QString &studentID, const QString &password)
{
    // Step 1: Use hash function to find the correct bucket
    int index = hashFunction(studentID);

    // Step 2: Get the head of the linked list at this bucket
    UserNode* current = registeredUsers->buckets[index];

    // Step 3: Traverse linked list to search for matching user
    while (current != nullptr) {
        // Check if both student ID and password match
        if (current->studentID == studentID && current->password == password) {
            return true;  // Match found - login successful
        }
        current = current->next;
    }

    return false;  // No match found - login failed
}



// ============================================================================
// PART 2: INITIALIZATION & CLEANUP
// Purpose: Setup window, create data structures, establish connections, free memory
// Note: Items marked with [NOT from Qt UI] are manually created
// ============================================================================

/**
 * Constructor
 * Purpose: Initialize all components and data structures for login window
 * Called: Automatically when program starts
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)              // Create UI object (generated from Qt Designer)
    , signupWindow(nullptr)               // Signup window pointer (lazy initialization)
    , manageCoursesPage(nullptr)          // Course page pointer (lazy initialization)
    , registeredUsers(nullptr)            // User hash table pointer (initialized below)
{
    // -------------------------------------------------------------------------
    // 2.1 UI Initialization
    // Purpose: Load the interface designed in Qt Designer
    // -------------------------------------------------------------------------
    ui->setupUi(this);

    // -------------------------------------------------------------------------
    // 2.2 Window Properties Setup
    // Purpose: Set window title and other properties
    // -------------------------------------------------------------------------
    this->setWindowTitle("Login");

    // -------------------------------------------------------------------------
    // 2.3 Data Structure Initialization [NOT from Qt UI]
    // Purpose: Create hash table to store user data
    // Note: This is manually created, not from Qt Designer
    // -------------------------------------------------------------------------
    registeredUsers = new UserRegistry(10);  // Hash table with capacity 10

    // -------------------------------------------------------------------------
    // 2.4 Load Users from File [NOT from Qt UI]
    // Purpose: Read saved user data from users.txt
    // Note: File handling for data persistence
    // -------------------------------------------------------------------------
    loadUsersFromFile();                     // Load all registered users from file

    // -------------------------------------------------------------------------
    // 2.5 Default Data Initialization [NOT from Qt UI]
    // Purpose: Add test account if no users loaded (first run)
    // Note: This is manually added default data
    // -------------------------------------------------------------------------
    // Only add default account if no users were loaded from file
    if (registeredUsers->buckets[hashFunction("12345")] == nullptr) {
        addUser("12345", "password123");     // Add default test account
        saveUsersToFile();                   // Save to file
    }

    // -------------------------------------------------------------------------
    // 2.5 Signal-Slot Connections
    // Purpose: Connect button click events to handler functions
    // Note: This is Qt's core mechanism - event-driven programming
    // -------------------------------------------------------------------------

    // Connect Login button -> Login handler function
    connect(ui->pushButton_Login, SIGNAL(clicked()),
            this, SLOT(on_submit_clicked()));

    // Connect SignUp button -> Open signup window function
    connect(ui->pushButton_SignUp, SIGNAL(clicked()),
            this, SLOT(on_toggle_mode_clicked()));
}

/**
 * Destructor
 * Purpose: Clean up all dynamically allocated memory to prevent memory leaks
 * Called: Automatically when program closes or object is deleted
 */
MainWindow::~MainWindow()
{
    // -------------------------------------------------------------------------
    // Cleanup order: UI components -> Child windows -> Data structures
    // -------------------------------------------------------------------------

    // Delete UI object (Qt-generated interface)
    delete ui;

    // Delete signup window (if user ever opened it)
    if (signupWindow) {
        delete signupWindow;
    }

    // Delete course management page (if user ever logged in)
    if (manageCoursesPage) {
        delete manageCoursesPage;
    }

    // Delete user hash table [NOT from Qt UI]
    // Note: This will automatically free all linked list nodes
    if (registeredUsers) {
        delete registeredUsers;
    }
}



// ============================================================================
// PART 3: UI EVENT HANDLERS
// Purpose: Handle button clicks and user interactions
// Note: These are "slots" - functions called when signals are emitted
// ============================================================================

/**
 * Login Button Handler (Slot Function)
 * Purpose: Process login attempt when user clicks Login button
 * Called: When user clicks the Login button (signal-slot mechanism)
 */
void MainWindow::on_submit_clicked()
{
    // -------------------------------------------------------------------------
    // 3.1 Get Input from UI
    // Purpose: Retrieve student ID and password from text fields
    // -------------------------------------------------------------------------
    QString studentID = ui->lineEdit_StudentID->text();
    QString password = ui->lineEdit_Password->text();

    // -------------------------------------------------------------------------
    // 3.2 Input Validation
    // -------------------------------------------------------------------------
    if (studentID.isEmpty() || password.isEmpty()) {
        UIDialogs::showWarning(this, "Input Error", "Please fill in all fields!");
        return;
    }

    // -------------------------------------------------------------------------
    // 3.3 Authentication Process
    // -------------------------------------------------------------------------
    if (validateLogin(studentID, password)) {
        // Login successful - save current user
        currentUser = studentID;
        UIDialogs::showInfo(this, "Login Successful", "Welcome " + studentID + "!");
        switchToManageCoursesPage();
    } else {
        // Login failed
        UIDialogs::showError(this, "Login Failed", "Invalid Student ID or Password!");
        ui->lineEdit_Password->clear();
    }
}

/**
 * SignUp Button Handler (Slot Function)
 * Purpose: Open signup window for new user registration
 * Called: When user clicks the SignUp button
 */
void MainWindow::on_toggle_mode_clicked()
{
    // -------------------------------------------------------------------------
    // 3.4 Lazy Initialization [NOT from Qt UI]
    // Purpose: Create signup window only when needed (saves memory)
    // Note: This is manually created, not from Qt Designer
    // -------------------------------------------------------------------------
    if (signupWindow == nullptr) {
        signupWindow = new SignupWindow(this);

        // Connect signal from SignupWindow to slot in MainWindow
        // When new user registers, MainWindow will receive the data
        connect(signupWindow, SIGNAL(userRegistered(const QString&, const QString&)),
                this, SLOT(on_userRegistered(const QString&, const QString&)));
    }

    // -------------------------------------------------------------------------
    // 3.5 Open Modal Dialog
    // Purpose: Show signup window as a modal dialog (blocks main window)
    // -------------------------------------------------------------------------
    signupWindow->exec();
}

/**
 * User Registration Handler (Slot Function)
 * Purpose: Process new user registration data from SignupWindow
 * Called: When SignupWindow emits userRegistered signal
 *
 * @param studentID: New user's student ID
 * @param password: New user's password
 */
void MainWindow::on_userRegistered(const QString &studentID, const QString &password)
{
    // -------------------------------------------------------------------------
    // 3.6 Insert into Hash Table
    // -------------------------------------------------------------------------
    if (!addUser(studentID, password)) {
        UIDialogs::showError(this, "Registration Failed", "Student ID already exists!");
        return;
    }

    // -------------------------------------------------------------------------
    // 3.7 Save to File
    // -------------------------------------------------------------------------
    saveUsersToFile();
    UIDialogs::showInfo(this, "Registration Successful", "Account created! You can now login.");
}



// ============================================================================
// PART 4: PAGE NAVIGATION
// Purpose: Switch between login page and course management page
// Note: Items marked with [NOT from Qt UI] are manually created
// ============================================================================

/**
 * Switch to Course Management Page
 * Purpose: Navigate from login page to course page after successful login
 * Called: After user successfully logs in
 */
void MainWindow::switchToManageCoursesPage()
{
    // -------------------------------------------------------------------------
    // 4.1 Delete old page and create new one with current user
    // Purpose: Each login creates a new page with user-specific data
    // -------------------------------------------------------------------------
    if (manageCoursesPage != nullptr) {
        delete manageCoursesPage;
    }
    manageCoursesPage = new ManageCoursesPage(currentUser, this);

    // -------------------------------------------------------------------------
    // 4.2 Window Switching
    // Purpose: Hide login window and show course management window
    // -------------------------------------------------------------------------
    this->hide();                    // Hide login window
    manageCoursesPage->show();       // Show course page
}

/**
 * Switch Back to Login Page
 * Purpose: Navigate from course page back to login page (logout)
 * Called: When user logs out from course management page
 */
void MainWindow::switchToLoginPage()
{
    // -------------------------------------------------------------------------
    // 4.3 Hide Course Page
    // Purpose: Hide the course management window
    // -------------------------------------------------------------------------
    if (manageCoursesPage) {
        manageCoursesPage->hide();
    }

    // -------------------------------------------------------------------------
    // 4.4 Clear Sensitive Data
    // Purpose: Clear login fields for security (prevent auto-login)
    // -------------------------------------------------------------------------
    ui->lineEdit_StudentID->clear();
    ui->lineEdit_Password->clear();

    // -------------------------------------------------------------------------
    // 4.5 Show Login Window
    // Purpose: Display the login window again
    // -------------------------------------------------------------------------
    this->show();
}


// ============================================================================
// PART 5: FILE HANDLING OPERATIONS
// Purpose: Save and load user data to/from file for persistence
// Note: These are NOT from Qt UI - manually implemented file I/O
// ============================================================================

/**
 * Save Users to File
 * Purpose: Write all registered users to users.txt file
 * Called: After new user registration
 *
 * File Format: Each line contains "studentID,password"
 * Example: 12345,password123
 */
void MainWindow::saveUsersToFile()
{
    // -------------------------------------------------------------------------
    // 5.1 Open File for Writing
    // Purpose: Create or overwrite users.txt file
    // Use application directory to ensure consistent path
    // -------------------------------------------------------------------------
    QString appDir = QCoreApplication::applicationDirPath();
    QString filePath = appDir + "/users.txt";
    QFile file(filePath);

    // Try to open file in write mode
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        UIDialogs::showWarning(this, "File Error", "Could not save user data to file!");
        return;
    }

    // -------------------------------------------------------------------------
    // 5.2 Write Data to File
    // Purpose: Iterate through hash table and write all users
    // -------------------------------------------------------------------------
    QTextStream out(&file);

    // Traverse all buckets in the hash table
    for (int i = 0; i < registeredUsers->capacity; i++) {
        UserNode* current = registeredUsers->buckets[i];

        // Traverse linked list in this bucket
        while (current != nullptr) {
            // Write user data in CSV format: studentID,password
            out << current->studentID << "," << current->password << "\n";
            current = current->next;
        }
    }

    // -------------------------------------------------------------------------
    // 5.3 Close File
    // Purpose: Ensure data is written to disk
    // -------------------------------------------------------------------------
    file.close();
}

/**
 * Load Users from File
 * Purpose: Read all registered users from users.txt file
 * Called: During program startup (in constructor)
 *
 * File Format: Each line contains "studentID,password"
 * Example: 12345,password123
 */
void MainWindow::loadUsersFromFile()
{
    // -------------------------------------------------------------------------
    // 5.4 Check if File Exists
    // Purpose: Only load if file exists (first run won't have file)
    // Use application directory to ensure consistent path
    // -------------------------------------------------------------------------
    QString appDir = QCoreApplication::applicationDirPath();
    QString filePath = appDir + "/users.txt";
    QFile file(filePath);

    // If file doesn't exist, skip loading (not an error)
    if (!file.exists()) {
        return;
    }

    // -------------------------------------------------------------------------
    // 5.5 Open File for Reading
    // -------------------------------------------------------------------------
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        UIDialogs::showWarning(this, "File Error", "Could not load user data from file!");
        return;
    }

    // -------------------------------------------------------------------------
    // 5.6 Read Data Line by Line
    // Purpose: Parse each line and add user to hash table
    // -------------------------------------------------------------------------
    QTextStream in(&file);

    while (!in.atEnd()) {
        // Read one line from file
        QString line = in.readLine().trimmed();

        // Skip empty lines
        if (line.isEmpty()) {
            continue;
        }

        // Parse line: split by comma
        QStringList parts = line.split(",");

        // Validate format: must have exactly 2 parts (studentID, password)
        if (parts.size() == 2) {
            QString studentID = parts[0].trimmed();
            QString password = parts[1].trimmed();

            // Add user to hash table
            // Note: addUser() handles duplicate checking
            addUser(studentID, password);
        }
    }

    // -------------------------------------------------------------------------
    // 5.7 Close File
    // Purpose: Release file handle
    // -------------------------------------------------------------------------
    file.close();
}


// ============================================================================
// END OF FILE
// ============================================================================
