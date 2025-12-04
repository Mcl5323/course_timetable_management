#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H

#include <QDialog>
#include <QProgressBar>
#include <QLabel>
#include <QTimer>

class LoadingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadingDialog(QWidget *parent = nullptr);
    ~LoadingDialog();

    void startLoading();

signals:
    void loadingComplete();

private slots:
    void updateProgress();

private:
    QProgressBar *progressBar;
    QLabel *loadingLabel;
    QTimer *timer;
    int currentProgress;
};

#endif // LOADINGDIALOG_H
