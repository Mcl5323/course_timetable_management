#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

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
    bool validateLogin(const QString &studentID, const QString &password);
    void switchToManageCoursesPage();

    Ui::MainWindow *ui;
    SignupWindow *signupWindow;
    ManageCoursesPage *manageCoursesPage;
    QMap<QString, QString> registeredUsers;
};

#endif // MAINWINDOW_H
