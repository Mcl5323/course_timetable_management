#include "loadingdialog.h"
#include <QVBoxLayout>
#include <QPalette>
#include <QFont>

LoadingDialog::LoadingDialog(QWidget *parent)
    : QDialog(parent)
    , currentProgress(0)
{
    // Set window properties
    setWindowTitle("Generating Timetable");
    setFixedSize(400, 150);
    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    // Set background color
    setStyleSheet("QDialog { background-color: #415a77; }");

    // Create layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(20);
    layout->setContentsMargins(30, 30, 30, 30);

    // Create loading label
    loadingLabel = new QLabel("LOADING...", this);
    loadingLabel->setAlignment(Qt::AlignCenter);
    loadingLabel->setStyleSheet(
        "QLabel {"
        "   color: white;"
        "   font-size: 20px;"
        "   font-weight: bold;"
        "}"
    );

    // Create progress bar
    progressBar = new QProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setValue(0);
    progressBar->setTextVisible(true);
    progressBar->setStyleSheet(
        "QProgressBar {"
        "   border: 2px solid #1b263b;"
        "   border-radius: 15px;"
        "   background-color: #778da9;"
        "   text-align: center;"
        "   color: white;"
        "   font-weight: bold;"
        "   height: 30px;"
        "}"
        "QProgressBar::chunk {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #e63946, stop:1 #f72585);"
        "   border-radius: 13px;"
        "}"
    );

    // Add widgets to layout
    layout->addWidget(loadingLabel);
    layout->addWidget(progressBar);

    // Create timer for progress updates
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &LoadingDialog::updateProgress);
}

LoadingDialog::~LoadingDialog()
{
    delete timer;
}

void LoadingDialog::startLoading()
{
    currentProgress = 0;
    progressBar->setValue(0);
    timer->start(30); // Update every 30ms for smooth animation
}

void LoadingDialog::updateProgress()
{
    currentProgress += 1;
    progressBar->setValue(currentProgress);

    // Complete loading when progress reaches 100%
    if (currentProgress >= 100) {
        timer->stop();
        progressBar->setValue(100);

        // Emit signal and close dialog after a short delay
        QTimer::singleShot(300, this, [this]() {
            emit loadingComplete();
            accept();
        });
    }
}
