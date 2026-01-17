/**
 * ============================================================================
 * Manage Courses Page (managecoursespage.h)
 * ============================================================================
 *
 * This is the main page where users can:
 * 1. Add new courses with name, day, time, and classroom
 * 2. Edit existing courses
 * 3. Delete courses
 * 4. Search courses using LINEAR SEARCH algorithm
 * 5. Sort courses using QUICKSORT algorithm
 * 6. Generate timetable combinations
 * 7. Export/Import courses to/from CSV files
 *
 * Data Structures Used:
 * - LinkedList<Course>: Stores all course data
 * - LinkedList<int>: Stores search result indices
 *
 * Algorithms Used:
 * - Linear Search: Find courses matching search keywords
 * - QuickSort: Sort courses by name/day/time/classroom
 *
 * ============================================================================
 */

#ifndef MANAGECOURSESPAGE_H
#define MANAGECOURSESPAGE_H

#include <QDialog>
#include <QString>
#include "datastructures.h"  // Custom LinkedList and HashTable implementations

// Forward declarations
class MainWindow;
class TIMETABLE;
class LoadingDialog;

namespace Ui {
class ManageCoursesPage;
}

/**
 * Course Structure - Represents a single course entry
 *
 * Each course has:
 * - name: Course name (e.g., "Data Structures")
 * - day: Day of week (e.g., "Monday")
 * - startTime: Start time (e.g., "8am")
 * - endTime: End time (e.g., "10am")
 * - classroom: Room location (e.g., "A101")
 */
struct Course {
    QString name;
    QString day;
    QString startTime;
    QString endTime;
    QString classroom;
};

/**
 * ManageCoursesPage Class - Main Course Management Interface
 *
 * Handles all course CRUD operations (Create, Read, Update, Delete)
 * and provides search/sort functionality using custom algorithms.
 */
class ManageCoursesPage : public QDialog {
    Q_OBJECT

public:
    explicit ManageCoursesPage(const QString &currentUser, QWidget *parent = nullptr);
    ~ManageCoursesPage();

private slots:
    // ==================== CRUD Operations ====================
    void onAddCourse();              // Add new course or save edited course
    void onDeleteCourse(int row);    // Delete course at specified row
    void onEditCourse(int row);      // Load course data into form for editing

    // ==================== Timetable Operations ====================
    void onGenerateTimetable();      // Generate timetable with loading animation
    void onLoadingComplete();        // Called when loading animation finishes
    void onViewTimetable();          // Show generated timetable window

    // ==================== Search & Sort (Algorithms) ====================
    void onSearchCourse();           // Search using LINEAR SEARCH algorithm
    void onClearSearch();            // Clear search and show all courses
    void onSortCourses(int index);   // Sort using QUICKSORT algorithm

    // ==================== Import/Export ====================
    void onExportCourses();          // Export courses to CSV file
    void onImportCourses();          // Import courses from CSV file

private:
    // ==================== Helper Functions ====================
    void refreshTable();             // Update table display from LinkedList
    void clearForm();                // Reset input form fields
    void setupConnections();         // Connect UI signals to slots
    int timeToInt(const QString &time);  // Convert time string to integer for comparison

    // ==================== File Operations ====================
    void saveCoursesToFile();        // Save courses to user's file
    void loadCoursesFromFile();      // Load courses from user's file

    // ==================== Algorithm Implementations ====================
    LinkedList<int> linearSearchCourses(const QString &searchText);  // LINEAR SEARCH
    void quickSortCourses(int sortBy);  // QUICKSORT by different criteria

    // ==================== Backup Mechanism ====================
    void createBackup();             // Create backup before saving
    void restoreFromBackup();        // Restore from backup if needed

    // ==================== Conflict Detection ====================
    bool checkTimeConflict(const Course &newCourse, int excludeRow = -1);
    LinkedList<Course> getConflictingCourses(const Course &newCourse, int excludeRow = -1);

    // ==================== Member Variables ====================
    Ui::ManageCoursesPage *ui;       // UI components from .ui file

    // Course data storage using LinkedList (custom data structure)
    LinkedList<Course> courses;      // Current displayed courses
    LinkedList<Course> allCourses;   // Backup when searching (stores all courses)
    bool isSearching;                // True when showing filtered results

    // Programmatically created UI elements (Search & Sort section)
    class QLineEdit *searchInput;    // Search text input
    class QPushButton *searchBtn;    // Search button
    class QPushButton *clearSearchBtn;  // Clear search button
    class QComboBox *sortCombo;      // Sort dropdown

    // Import/Export buttons
    class QPushButton *exportBtn;    // Export to CSV
    class QPushButton *importBtn;    // Import from CSV

    // State management
    QString currentUser;             // Current logged-in user ID
    int editingRow;                  // -1 = add mode, >=0 = editing row index
    int selectedRow;                 // Currently selected row (-1 = none)
    TIMETABLE *timetableWindow;      // Timetable view window
    LoadingDialog *loadingDialog;    // Loading animation dialog
    bool timetableGenerated;         // True if timetable has been generated
};

#endif // MANAGECOURSESPAGE_H
