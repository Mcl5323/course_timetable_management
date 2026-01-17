/**
 * ============================================================================
 * Timetable View Page (timetable.h)
 * ============================================================================
 *
 * This page displays the generated timetable in a weekly view format.
 *
 * Key Features:
 * 1. Display courses in a 7-day x time grid
 * 2. Generate all possible timetable combinations
 * 3. Navigate between different combinations
 * 4. Export timetable as image (PNG/JPG)
 * 5. Show statistics (total courses, hours, conflicts)
 *
 * Algorithm Used:
 * - RECURSIVE BACKTRACKING: Generate all possible timetable combinations
 *   when a course has multiple time options
 *
 * Data Structures Used:
 * - LinkedList<Course>: Store courses for current combination
 * - LinkedList<LinkedList<Course>>: Store all generated combinations
 * - HashTable<QString, int>: Map day names to row indices
 *
 * ============================================================================
 */

#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <QDialog>
#include <QString>
#include "datastructures.h"  // Custom LinkedList and HashTable implementations

namespace Ui {
class TIMETABLE;
}

struct Course;  // Forward declaration

/**
 * TIMETABLE Class - Timetable Display and Generation
 *
 * Displays courses in a weekly grid format and handles
 * generation of all possible timetable combinations using
 * recursive backtracking algorithm.
 */
class TIMETABLE : public QDialog
{
    Q_OBJECT

public:
    explicit TIMETABLE(QWidget *parent = nullptr);
    ~TIMETABLE();

    /**
     * Set course data and generate all timetable combinations
     * @param courses - LinkedList of all courses to display
     */
    void setCoursesData(const LinkedList<Course> &courses);

private slots:
    // ==================== User Actions ====================
    void onSaveAs();       // Export timetable as image file
    void onBack();         // Hide window (can view again)
    void onPrevPage();     // Navigate to previous combination
    void onNextPage();     // Navigate to next combination
    void onTogglePage();   // Toggle between combinations
    void onDelete();       // Close window (need to regenerate)

private:
    // ==================== Display Functions ====================
    void populateTimetable();    // Fill table with course data
    void updateStatistics();     // Update course/hours/conflict counts
    void displayCurrentCombination();  // Show current combination
    void updatePageLabel();      // Update page number display

    // ==================== Calculation Functions ====================
    int calculateTotalHours();   // Sum up all course hours
    int detectConflicts();       // Count time conflicts
    int timeToColumn(const QString &time);  // Convert "8am" -> column index
    int dayToRow(const QString &day);       // Convert "Monday" -> row index

    // ==================== Combination Generation (RECURSIVE BACKTRACKING) ====================
    /**
     * Generate all possible timetable combinations
     * Groups courses by name, then generates all permutations
     */
    void generateAllCombinations();

    /**
     * RECURSIVE BACKTRACKING ALGORITHM
     *
     * Generates all combinations by:
     * 1. For each course group, try each time option
     * 2. Recursively process next group
     * 3. Backtrack and try next option
     *
     * Time Complexity: O(n^m) where n = options per course, m = number of courses
     */
    void generateCombinationsRecursive(const LinkedList<LinkedList<Course>> &groups,
                                       int groupIndex,
                                       LinkedList<Course> &currentCombination);

    /**
     * Check if a combination has time conflicts
     * @return true if any two courses overlap in time on same day
     */
    bool hasConflict(const LinkedList<Course> &combination);

    // ==================== Member Variables ====================
    Ui::TIMETABLE *ui;           // UI components from .ui file

    LinkedList<Course> coursesData;  // Original course data

    // Combination storage
    LinkedList<LinkedList<Course>> allCombinations;  // All generated combinations
    int currentCombinationIndex;  // Current displayed combination (0-based)
};

#endif // TIMETABLE_H
