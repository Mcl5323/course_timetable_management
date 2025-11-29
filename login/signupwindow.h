#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H

#include <QDialog>

namespace Ui {
class SignupWindow;
}

class SignupWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SignupWindow(QWidget *parent = nullptr);
    ~SignupWindow();

signals:
    void userRegistered(const QString &studentID, const QString &password);

private slots:
    void on_pushButton_Confirm_clicked();
    void on_pushButton_Back_clicked();

private:
    Ui::SignupWindow *ui;
};

#endif // SIGNUPWINDOW_H
