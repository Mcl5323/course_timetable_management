/**
 * SignupWindow Header File
 *
 * This file defines the SignupWindow class, which provides
 * a dialog interface for new user registration.
 */

#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H

#include <QDialog>

namespace Ui {
class SignupWindow;
}

/**
 * SignupWindow Class
 *
 * A modal dialog window for user registration. Inherits from QDialog
 * which provides standard dialog functionality (OK/Cancel behavior).
 *
 * Key Features:
 * - Input fields for student ID, password, and password confirmation
 * - Password matching validation
 * - Signal emission to notify MainWindow of successful registration
 * - Modal behavior (blocks interaction with parent window)
 */
class SignupWindow : public QDialog
{
    Q_OBJECT  // Required macro for signal-slot mechanism

public:
    /**
     * Constructor - creates the signup dialog
     * @param parent: Parent window (MainWindow in this case)
     */
    explicit SignupWindow(QWidget *parent = nullptr);

    /**
     * Destructor - cleans up UI resources
     */
    ~SignupWindow();

signals:
    /**
     * Signal emitted when a user successfully completes registration
     *
     * This signal is connected to MainWindow's on_userRegistered slot,
     * allowing the parent window to save the new user's credentials.
     *
     * @param studentID: The newly registered student ID
     * @param password: The password for the new account
     *
     * Note: signals don't have implementations - Qt's meta-object system
     * handles the signal emission automatically
     */
    void userRegistered(const QString &studentID, const QString &password);

private slots:
    /**
     * Handles Confirm button click
     * Validates input, checks password match, and emits registration signal
     */
    void on_pushButton_Confirm_clicked();

    /**
     * Handles Back button click
     * Clears form and closes the dialog without registering
     */
    void on_pushButton_Back_clicked();

private:
    Ui::SignupWindow *ui;  // Pointer to UI components from Qt Designer
};

#endif // SIGNUPWINDOW_H
