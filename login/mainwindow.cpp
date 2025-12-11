/**
 * MainWindow Implementation File
 *
 * This file contains the implementation of the MainWindow class,
 * which manages the login functionality using a manual hash table.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "signupwindow.h"
#include "managecoursespage.h"
#include <QMessageBox>

/**
 * Hash Function
 *
 * Converts a QString key into a bucket index.
 * Uses simple character sum modulo capacity.
 *
 * @param key: The student ID to hash
 * @return: Bucket index (0 to capacity-1)
 */
int MainWindow::hashFunction(const QString& key)
{
    int hash = 0;

    // Sum up all character values
    for (int i = 0; i < key.length(); i++) {
        hash += key[i].unicode();  // Get Unicode value of character
    }

    // Use modulo to fit within bucket array size
    return hash % registeredUsers->capacity;
}

/**
 * Add User to Hash Table
 *
 * Inserts a new user into the hash table.
 * Checks for duplicates before insertion.
 *
 * @param studentID: The student ID to add
 * @param password: The password for this student
 * @return: true if added successfully, false if duplicate exists
 */
bool MainWindow::addUser(const QString& studentID, const QString& password)
{
    // Calculate which bucket this user belongs to
    int index = hashFunction(studentID);

    // Get the head of the linked list at this bucket
    UserNode* current = registeredUsers->buckets[index];

    // Traverse the linked list to check for duplicates
    while (current != nullptr) {
        if (current->studentID == studentID) {
            return false;  // Duplicate found
        }
        current = current->next;
    }

    // No duplicate found - create new node
    UserNode* newNode = new UserNode(studentID, password);

    // Insert at the beginning of the linked list (easier than end)
    newNode->next = registeredUsers->buckets[index];
    registeredUsers->buckets[index] = newNode;

    return true;  // Successfully added
}

/**
 * Validate Login Credentials
 *
 * Checks if the provided credentials match a registered user.
 * Uses hash function to find the correct bucket, then searches the linked list.
 *
 * @param studentID: The student ID to check
 * @param password: The password to verify
 * @return: true if credentials are valid, false otherwise
 */
bool MainWindow::validateLogin(const QString &studentID, const QString &password)
{
    // Use hash function to find the correct bucket
    int index = hashFunction(studentID);

    // Get the head of the linked list at this bucket
    UserNode* current = registeredUsers->buckets[index];

    // Traverse the linked list in this bucket
    while (current != nullptr) {
        // Check if both student ID and password match
        if (current->studentID == studentID && current->password == password) {
            return true;  // Match found - login successful
        }
        current = current->next;
    }

    return false;  // No match found - login failed
}

/**
 * MainWindow Constructor
 *
 * Initializes the login window and sets up the user interface.
 * Creates the hash table and adds default account.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , signupWindow(nullptr)
    , manageCoursesPage(nullptr)
    , registeredUsers(nullptr)
{
    // Set up the user interface defined in Qt Designer (.ui file)
    ui->setupUi(this);

    // Set the window title that appears in the title bar
    this->setWindowTitle("Login");

    // Initialize the user registry with capacity 10
    registeredUsers = new UserRegistry(10);

    // Add default test account
    addUser("12345", "password123");

    /**
     * Connect button signals to slots (Signal-Slot Mechanism)
     */

    // When Login button is clicked, call on_submit_clicked()
    connect(ui->pushButton_Login, SIGNAL(clicked()), this, SLOT(on_submit_clicked()));

    // When SignUp button is clicked, call on_toggle_mode_clicked()
    connect(ui->pushButton_SignUp, SIGNAL(clicked()), this, SLOT(on_toggle_mode_clicked()));
}

/**
 * MainWindow Destructor
 *
 * Cleans up dynamically allocated memory.
 * The UserRegistry destructor will automatically free all nodes.
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

    // Delete the hash table (this will free all nodes)
    if (registeredUsers) {
        delete registeredUsers;
    }
}

/**
 * Login Submit Handler (Slot Function)
 *
 * This function is called when the Login button is clicked.
 * Validates input and authenticates credentials using the hash table.
 */
void MainWindow::on_submit_clicked()
{
    // Get text from input fields
    QString studentID = ui->lineEdit_StudentID->text();
    QString password = ui->lineEdit_Password->text();

    /**
     * Input Validation - Check for empty fields
     */
    if (studentID.isEmpty() || password.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Input Error");
        msgBox.setText("Please fill in all fields!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        return;
    }

    /**
     * Authentication Process
     * Uses validateLogin() to check credentials against hash table
     */
    if (validateLogin(studentID, password)) {
        // Login successful
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Login Successful");
        msgBox.setText("Welcome " + studentID + "!");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();

        // Navigate to the course management page
        switchToManageCoursesPage();
    } else {
        // Login failed
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Login Failed");
        msgBox.setText("Invalid Student ID or Password!");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();

        // Clear password field for security
        ui->lineEdit_Password->clear();
    }
}

/**
 * Signup Button Handler (Slot Function)
 *
 * Opens the signup window dialog for new user registration.
 */
void MainWindow::on_toggle_mode_clicked()
{
    /**
     * Lazy Initialization Pattern
     */
    if (signupWindow == nullptr) {
        signupWindow = new SignupWindow(this);

        /**
         * Connect Signal from SignupWindow to Slot in MainWindow
         */
        connect(signupWindow, SIGNAL(userRegistered(const QString&, const QString&)),
                this, SLOT(on_userRegistered(const QString&, const QString&)));
    }

    // Open the signup window as a modal dialog
    signupWindow->exec();
}

/**
 * User Registration Handler (Slot Function)
 *
 * This slot is called when SignupWindow emits the userRegistered signal.
 * Uses addUser() to insert into the hash table.
 *
 * @param studentID: New user's student ID
 * @param password: New user's password
 */
void MainWindow::on_userRegistered(const QString &studentID, const QString &password)
{
    /**
     * Use addUser() to insert into hash table
     * addUser() returns false if duplicate is found in the bucket
     */
    if (!addUser(studentID, password)) {
        // Duplicate found
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Registration Failed");
        msgBox.setText("Student ID already exists!");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet("QLabel{font-size: 11px;} QPushButton{font-size: 11px;}");
        msgBox.exec();
        return;
    }

    // Successfully inserted new node
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

    // Clear input fields for security
    ui->lineEdit_StudentID->clear();
    ui->lineEdit_Password->clear();

    // Show the login window again
    this->show();
}
