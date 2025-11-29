/**
 * MainWindow Header File
 *
 * This is the header file for the main login window of the
 * Course Timetable Management System. It defines the MainWindow
 * class which handles user authentication and navigation.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

// Qt namespace declaration for UI components
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// Forward declarations to avoid circular dependencies
class SignupWindow;
class ManageCoursesPage;

/**
 * MainWindow Class
 *
 * This class manages the login interface and serves as the entry point
 * to the application. It handles user authentication, account registration,
 * and navigation between different pages of the system.
 *
 * Key Features:
 * - User login validation
 * - Integration with signup window for new user registration
 * - Navigation to course management page after successful login
 * - In-memory storage of registered user credentials
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT  // Macro required for Qt's signal-slot mechanism

public:
    /**
     * Constructor - initializes the login window
     * @param parent: Optional parent widget (nullptr for top-level window)
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * Destructor - cleans up allocated resources
     */
    ~MainWindow();

    /**
     * Switches back to the login page from other pages
     * Also clears input fields for security
     */
    void switchToLoginPage();

private slots:
    // Slots are functions that respond to signals (events)

    /**
     * Handles login button click
     * Validates user credentials and navigates to course page if successful
     */
    void on_submit_clicked();

    /**
     * Handles signup button click
     * Opens the signup window dialog for new user registration
     */
    void on_toggle_mode_clicked();

    /**
     * Receives signal from SignupWindow when a new user registers
     * @param studentID: The new user's student ID
     * @param password: The new user's password
     */
    void on_userRegistered(const QString &studentID, const QString &password);

private:
    /**
     * Validates user login credentials
     * @param studentID: Student ID to validate
     * @param password: Password to validate
     * @return true if credentials match a registered user, false otherwise
     */
    bool validateLogin(const QString &studentID, const QString &password);

    /**
     * Switches to the course management page
     * Hides the login window and displays the course management interface
     */
    void switchToManageCoursesPage();

    // Private member variables

    Ui::MainWindow *ui;  // Pointer to the UI components defined in Qt Designer

    SignupWindow *signupWindow;  // Pointer to the signup dialog window

    ManageCoursesPage *manageCoursesPage;  // Pointer to the course management page

    /**
     * In-memory database of registered users
     * Key: Student ID (QString)
     * Value: Password (QString)
     * Note: In a production system, passwords should be hashed and
     * data should be persisted to a database
     */
    QMap<QString, QString> registeredUsers;
};

#endif // MAINWINDOW_H
