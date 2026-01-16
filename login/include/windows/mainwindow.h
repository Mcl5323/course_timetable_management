#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include "datastructures.h"  // Use our custom data structures

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class SignupWindow;
class ManageCoursesPage;
class QPushButton;
class QCheckBox;

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
