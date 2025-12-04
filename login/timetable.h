#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <QDialog>
#include <QVector>
#include <QString>
#include <QMap>
#include <QPair>
#include <QSet>

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
    void setCoursesData(const QVector<Course> &courses);

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
    void generateCombinationsRecursive(const QList<QVector<Course>> &groups,
                                       int groupIndex,
                                       QVector<Course> &currentCombination);
    bool hasConflict(const QVector<Course> &combination);
    void displayCurrentCombination();
    void updatePageLabel();

    Ui::TIMETABLE *ui;
    QVector<Course> coursesData;  // All courses added by user

    // New members for handling multiple timetable combinations
    QVector<QVector<Course>> allCombinations;  // All valid non-conflicting combinations
    int currentCombinationIndex;  // Current page index
};

#endif // TIMETABLE_H
