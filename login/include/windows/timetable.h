#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <QDialog>
#include <QString>
#include "datastructures.h"  // Manual data structures implementation

namespace Ui {
class TIMETABLE;
}

struct Course;  // Forward declaration

class TIMETABLE : public QDialog
{
    Q_OBJECT

public:
    explicit TIMETABLE(QWidget *parent = nullptr);
    ~TIMETABLE();

    // Set course data to populate timetable
    void setCoursesData(const LinkedList<Course> &courses);

private slots:
    void onSaveAs();
    void onBack();
    void onPrevPage();
    void onNextPage();
    void onTogglePage();  // New: Toggle between pages with single button
    void onDelete();

private:
    void populateTimetable();
    void updateStatistics();
    int calculateTotalHours();
    int detectConflicts();
    int timeToColumn(const QString &time);
    int dayToRow(const QString &day);

    // New methods for generating all possible timetable combinations
    void generateAllCombinations();
    void generateCombinationsRecursive(const LinkedList<LinkedList<Course>> &groups,
                                       int groupIndex,
                                       LinkedList<Course> &currentCombination);
    bool hasConflict(const LinkedList<Course> &combination);
    void displayCurrentCombination();
    void updatePageLabel();

    Ui::TIMETABLE *ui;
    LinkedList<Course> coursesData;  // All courses added by user

    // New members for handling multiple timetable combinations
    LinkedList<LinkedList<Course>> allCombinations;  // All valid non-conflicting combinations
    int currentCombinationIndex;  // Current page index
};

#endif // TIMETABLE_H
