#ifndef MANAGECOURSESPAGE_H
#define MANAGECOURSESPAGE_H

#include <QDialog>
#include <QVector>
#include <QString>

class MainWindow;

namespace Ui {
class ManageCoursesPage;
}

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
    explicit ManageCoursesPage(QWidget *parent = nullptr);
    ~ManageCoursesPage();

private slots:
    void onAddCourse();
    void onDeleteCourse(int row);
    void onEditCourse(int row);
    void onGenerateTimetable();
    void onViewTimetable();

private:
    void refreshTable();
    void clearForm();
    void setupConnections();

    Ui::ManageCoursesPage *ui;
    QVector<Course> courses;
    int editingRow;  // -1 if not editing, otherwise the row index being edited
};

#endif // MANAGECOURSESPAGE_H
