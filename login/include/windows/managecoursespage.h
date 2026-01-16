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
    void onExportCourses();
    void onImportCourses();

private:
    void refreshTable();
    void clearForm();
    void setupConnections();
    int timeToInt(const QString &time);
    void saveCoursesToFile();
    void loadCoursesFromFile();
    LinkedList<int> linearSearchCourses(const QString &searchText);
    void quickSortCourses(int sortBy);

    // Backup mechanism
    void createBackup();
    void restoreFromBackup();

    // Conflict detection when adding courses
    bool checkTimeConflict(const Course &newCourse, int excludeRow = -1);
    LinkedList<Course> getConflictingCourses(const Course &newCourse, int excludeRow = -1);

    Ui::ManageCoursesPage *ui;
    LinkedList<Course> courses;
    LinkedList<Course> allCourses;  // Backup of all courses when searching
    bool isSearching;  // Track if currently showing search results

    // Search and sort UI (not from .ui file)
    class QLineEdit *searchInput;
    class QPushButton *searchBtn;
    class QPushButton *clearSearchBtn;
    class QComboBox *sortCombo;

    // Import/Export buttons
    class QPushButton *exportBtn;
    class QPushButton *importBtn;

    QString currentUser;
    int editingRow;  // -1 = add mode, >=0 = edit mode
    int selectedRow;  // Track which row's checkbox is selected (-1 = none)
    TIMETABLE *timetableWindow;
    LoadingDialog *loadingDialog;
    bool timetableGenerated;  // Track if timetable has been generated
};

#endif // MANAGECOURSESPAGE_H
