#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class SignupWindow;
class ManageCoursesPage;

/**
 * User Node Structure
 * Represents a single user account in the linked list
 */
struct UserNode {
    QString studentID;
    QString password;
    UserNode* next;

    // Constructor for easy node creation
    UserNode(const QString& id, const QString& pass)
        : studentID(id), password(pass), next(nullptr) {}
};

/**
 * UserRegistry Structure
 * Manual hash table implementation using chaining for collision resolution
 */
struct UserRegistry {
    UserNode** buckets;  // Array of pointers to linked lists
    int capacity;        // Size of the hash table

    // Constructor
    UserRegistry(int cap = 10) : capacity(cap) {
        buckets = new UserNode*[capacity];
        // Initialize all buckets to nullptr
        for (int i = 0; i < capacity; i++) {
            buckets[i] = nullptr;
        }
    }

    // Destructor - Free all nodes
    ~UserRegistry() {
        for (int i = 0; i < capacity; i++) {
            UserNode* current = buckets[i];
            while (current != nullptr) {
                UserNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] buckets;
    }
};

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

    // Manual hash table for user storage
    UserRegistry* registeredUsers;

    // Current logged-in user
    QString currentUser;

    // Hash table helper functions
    int hashFunction(const QString& key);
    bool addUser(const QString& studentID, const QString& password);
    bool validateLogin(const QString &studentID, const QString &password);

    // File handling functions [NOT from Qt UI]
    void saveUsersToFile();     // Save all users to users.txt
    void loadUsersFromFile();   // Load users from users.txt

    void switchToManageCoursesPage();
};

#endif // MAINWINDOW_H
