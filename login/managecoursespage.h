/**
 * ManageCoursesPage Header File
 *
 * This file defines the course management interface where users can
 * add, edit, delete, and view their course schedule.
 */

#ifndef MANAGECOURSESPAGE_H
#define MANAGECOURSESPAGE_H

#include <QDialog>
#include <QVector>
#include <QString>

class MainWindow;
class TIMETABLE;
class LoadingDialog;

namespace Ui {
class ManageCoursesPage;
}

/**
 * Course Structure
 *
 * Stores all information about a single course.
 * This is a simple data structure (no methods, just data).
 */
struct Course {
    QString name;       // Course name (e.g., "Data Structures")
    QString day;        // Day of week (e.g., "Monday")
    QString startTime;  // Start time in 12-hour format (e.g., "9am")
    QString endTime;    // End time in 12-hour format (e.g., "11am")
    QString classroom;  // Classroom location (e.g., "Room 301")
};

/**
 * ManageCoursesPage Class
 *
 * Main interface for course management. Provides full CRUD operations
 * (Create, Read, Update, Delete) for courses.
 *
 * Key Features:
 * - Add new courses with validation
 * - Edit existing courses (inline editing)
 * - Delete courses with confirmation
 * - Display courses in a formatted table
 * - Dynamic UI updates using widgets and layouts
 * - Checkbox-based action enabling (Edit/Delete buttons)
 */
class ManageCoursesPage : public QDialog {
    Q_OBJECT  // Required for signal-slot mechanism

public:
    /**
     * Constructor - initializes the course management interface
     * @param parent: Parent window (MainWindow)
     */
    explicit ManageCoursesPage(QWidget *parent = nullptr);

    /**
     * Destructor - cleans up resources
     */
    ~ManageCoursesPage();

private slots:
    // Slot functions that respond to user actions

    /**
     * Adds a new course or updates an edited course
     * Validates input and prevents duplicates
     */
    void onAddCourse();

    /**
     * Deletes a course from the list
     * @param row: Row index in the table to delete
     */
    void onDeleteCourse(int row);

    /**
     * Loads a course into the input form for editing
     * @param row: Row index in the table to edit
     */
    void onEditCourse(int row);

    /**
     * Generates a timetable from the added courses
     * Shows loading dialog and then displays timetable
     */
    void onGenerateTimetable();

    /**
     * Called when loading is complete
     * Opens the timetable window
     */
    void onLoadingComplete();

    /**
     * Displays the generated timetable
     * (Currently shows a placeholder message)
     */
    void onViewTimetable();

private:
    // Private helper methods

    /**
     * Refreshes the table display with current course data
     * Rebuilds all table rows and widgets
     */
    void refreshTable();

    /**
     * Clears all input fields in the form
     * Resets the form to initial state
     */
    void clearForm();

    /**
     * Sets up signal-slot connections for buttons
     * Called once during initialization
     */
    void setupConnections();

    /**
     * Converts time string to 24-hour integer for comparison
     * e.g., "8am" -> 8, "2pm" -> 14
     */
    int timeToInt(const QString &time);

    // Private member variables

    Ui::ManageCoursesPage *ui;  // Pointer to UI components

    /**
     * Vector (dynamic array) storing all course data
     * Each element is a Course struct containing course details
     */
    QVector<Course> courses;

    /**
     * Edit Mode Tracking Variable
     * -1: Not in edit mode (adding new course)
     * >=0: In edit mode, editing the course at this index
     *
     * This allows the same form and button to handle both
     * adding new courses and editing existing ones
     */
    int editingRow;

    /**
     * Timetable window pointer
     */
    TIMETABLE *timetableWindow;

    /**
     * Loading dialog pointer
     */
    LoadingDialog *loadingDialog;
};

#endif // MANAGECOURSESPAGE_H
