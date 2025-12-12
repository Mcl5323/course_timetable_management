#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "datastructures.h"  // Use our custom data structures

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class SignupWindow;
class ManageCoursesPage;

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

private:
    Ui::MainWindow *ui;
    SignupWindow *signupWindow;
    ManageCoursesPage *manageCoursesPage;
    HashTable<QString, QString> registeredUsers;  // key=studentID, value=password (using datastructures.h)
    QString currentUser;

    bool addUser(const QString& studentID, const QString& password);
    bool validateLogin(const QString &studentID, const QString &password);
    void saveUsersToFile();
    void loadUsersFromFile();
    void switchToManageCoursesPage();
};

#endif // MAINWINDOW_H
