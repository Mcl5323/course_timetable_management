#ifndef MANAGECOURSESPAGE_H
#define MANAGECOURSESPAGE_H

#include <QDialog>
#include <QString>
#include "datastructures.h"  // Manual data structures implementation

class MainWindow;
class TIMETABLE;
class LoadingDialog;

namespace Ui {
class ManageCoursesPage;
}

// Course data
struct Course {
    QString name;
    QString day;
    QString startTime;
    QString endTime;
    QString classroom;
};

class ManageCoursesPage : public QDialog {
    Q_OBJECT

public:
    explicit ManageCoursesPage(const QString &currentUser, QWidget *parent = nullptr);
    ~ManageCoursesPage();

private slots:
    void onAddCourse();
    void onDeleteCourse(int row);
    void onEditCourse(int row);
    void onGenerateTimetable();
    void onLoadingComplete();
    void onViewTimetable();
    void onSearchCourse();
    void onClearSearch();
    void onSortCourses(int index);

private:
    void refreshTable();
    void clearForm();
    void setupConnections();
    int timeToInt(const QString &time);
    void saveCoursesToFile();
    void loadCoursesFromFile();
    LinkedList<int> linearSearchCourses(const QString &searchText);
    void quickSortCourses(int sortBy);

    Ui::ManageCoursesPage *ui;
    LinkedList<Course> courses;

    // Search and sort UI (not from .ui file)
    class QLineEdit *searchInput;
    class QPushButton *searchBtn;
    class QPushButton *clearSearchBtn;
    class QComboBox *sortCombo;

    QString currentUser;
    int editingRow;  // -1 = add mode, >=0 = edit mode
    TIMETABLE *timetableWindow;
    LoadingDialog *loadingDialog;
};

#endif // MANAGECOURSESPAGE_H
