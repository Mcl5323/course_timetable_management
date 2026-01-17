/**
 * ============================================================================
 * Loading Dialog Implementation (loadingdialog.cpp)
 * ============================================================================
 *
 * This file implements a modal loading dialog that displays a progress bar
 * animation while the timetable is being generated.
 *
 * Features:
 * - Animated progress bar (0% to 100%)
 * - Timer-based smooth animation (30ms intervals)
 * - Auto-closes when loading completes
 * - Emits loadingComplete signal to trigger timetable display
 *
 * ============================================================================
 */

#include "loadingdialog.h"
#include <QVBoxLayout>
#include <QPalette>
#include <QFont>

/**
 * Constructor - Initialize loading dialog with progress bar
 * Creates a modal dialog with styled progress bar and loading label
 */
LoadingDialog::LoadingDialog(QWidget *parent)
    : QDialog(parent)
    , currentProgress(0)
{
    /* Set window properties */
    setWindowTitle("Generating Timetable");
    setFixedSize(400, 150);
    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    /* Set solid opaque background - no transparency */
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor("#415a77"));
    setPalette(pal);
    setStyleSheet("QDialog { background-color: #415a77; border: none; }");

    /* Create layout */
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(20);
    layout->setContentsMargins(30, 30, 30, 30);

    /* Create loading label */
    loadingLabel = new QLabel("LOADING...", this);
    loadingLabel->setAlignment(Qt::AlignCenter);
    loadingLabel->setAutoFillBackground(true);
    QPalette labelPal = loadingLabel->palette();
    labelPal.setColor(QPalette::Window, QColor("#415a77"));
    loadingLabel->setPalette(labelPal);
    loadingLabel->setStyleSheet(
        "QLabel {"
        "   color: white;"
        "   font-size: 20px;"
        "   font-weight: bold;"
        "   background-color: #415a77;"
        "}"
    );

    /* Create progress bar */
    progressBar = new QProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setValue(0);
    progressBar->setTextVisible(true);
    progressBar->setStyleSheet(
        "QProgressBar {"
        "   border: 2px solid #1b263b;"
        "   border-radius: 10px;"
        "   background-color: #778da9;"
        "   text-align: center;"
        "   color: white;"
        "   font-weight: bold;"
        "   min-height: 25px;"
        "   max-height: 25px;"
        "}"
        "QProgressBar::chunk {"
        "   background-color: #e63946;"
        "   border-radius: 8px;"
        "   margin: 1px;"
        "}"
    );

    /* Add widgets to layout */
    layout->addWidget(loadingLabel);
    layout->addWidget(progressBar);

    /* Create timer for progress updates */
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &LoadingDialog::updateProgress);
}

/**
 * Destructor - Clean up timer resource
 */
LoadingDialog::~LoadingDialog()
{
    delete timer;
}

/**
 * Start the loading animation
 * Resets progress to 0 and starts the timer for animation
 */
void LoadingDialog::startLoading()
{
    currentProgress = 0;
    progressBar->setValue(0);
    timer->start(30);  /* Update every 30ms for smooth animation */
}

/**
 * Timer callback - Update progress bar value
 * Called every 30ms to increment progress by 1%
 * Emits loadingComplete signal when reaching 100%
 */
void LoadingDialog::updateProgress()
{
    currentProgress += 1;
    progressBar->setValue(currentProgress);

    /* Complete loading when progress reaches 100% */
    if (currentProgress >= 100) {
        timer->stop();
        progressBar->setValue(100);

        /* Emit signal and close dialog after a short delay */
        QTimer::singleShot(300, this, [this]() {
            emit loadingComplete();
            accept();
        });
    }
}
