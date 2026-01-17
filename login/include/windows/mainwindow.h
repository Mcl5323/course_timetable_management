/**
 * ============================================================================
 * Login Page (mainwindow.h)
 * ============================================================================
 *
 * This is the first page users see - the login/authentication page.
 *
 * Features:
 * 1. User login with Student ID and Password
 * 2. Password hashing for security
 * 3. Remember Me functionality
 * 4. Brute force protection (max 5 attempts)
 * 5. Link to signup page for new users
 *
 * Data Structures Used:
 * - HashTable<QString, QString>: Store registered users (ID -> hashed password)
 *
 * Security Features:
 * - Password hashing using Qt's cryptographic hash
 * - Login attempt limiting to prevent brute force attacks
 * - Encrypted password storage
 *
 * ============================================================================
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include "datastructures.h"  // Custom HashTable for user storage

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// Forward declarations
class SignupWindow;
class ManageCoursesPage;
class QPushButton;
class QCheckBox;

/**
 * MainWindow Class - Login/Authentication Page
 *
 * Handles user authentication and navigation to other pages.
 * Uses HashTable to store and lookup user credentials.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void switchToLoginPage();

private slots:
    void on_submit_clicked();
    void on_toggle_mode_clicked();
    void on_userRegistered(const QString &studentID, const QString &password);
    void togglePasswordVisibility();
    void onResetPassword();

private:
    Ui::MainWindow *ui;
    SignupWindow *signupWindow;
    ManageCoursesPage *manageCoursesPage;
    HashTable<QString, QString> registeredUsers;  // key=studentID, value=hashedPassword (using datastructures.h)
    QString currentUser;

    // Login attempt limit (brute force protection)
    int failedLoginAttempts;
    static const int MAX_LOGIN_ATTEMPTS = 5;
    QString lastAttemptedUser;

    // Remember Me functionality
    QCheckBox *rememberMeCheckbox;
    QPushButton *togglePasswordBtn;
    QPushButton *resetPasswordBtn;  // Shows when account is locked

    bool addUser(const QString& studentID, const QString& hashedPassword);
    bool validateLogin(const QString &studentID, const QString &password);
    void saveUsersToFile();
    void loadUsersFromFile();
    void switchToManageCoursesPage();

    // Password hashing
    QString hashPassword(const QString &password);

    // Remember Me functions
    void saveRememberedUser();
    void loadRememberedUser();
};

#endif // MAINWINDOW_H
