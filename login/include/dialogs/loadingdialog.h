/**
 * ============================================================================
 * Loading Dialog (loadingdialog.h)
 * ============================================================================
 *
 * A modal dialog that shows a loading animation while the timetable
 * is being generated. Provides visual feedback to the user.
 *
 * Features:
 * - Animated progress bar (0% to 100%)
 * - "LOADING..." text display
 * - Auto-closes when loading completes
 * - Emits signal when done to trigger timetable display
 *
 * ============================================================================
 */

#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H

#include <QDialog>
#include <QProgressBar>
#include <QLabel>
#include <QTimer>

/**
 * LoadingDialog Class - Loading Animation Dialog
 *
 * Shows a progress bar animation while the timetable is generated.
 * Uses QTimer to animate the progress bar smoothly.
 */
class LoadingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadingDialog(QWidget *parent = nullptr);
    ~LoadingDialog();

    /**
     * Start the loading animation
     * Progress bar will animate from 0% to 100%
     */
    void startLoading();

signals:
    /**
     * Emitted when loading animation completes (100%)
     * Connected to ManageCoursesPage::onLoadingComplete()
     */
    void loadingComplete();

private slots:
    /**
     * Called by timer to update progress bar
     * Increments progress by 1% each call
     */
    void updateProgress();

private:
    QProgressBar *progressBar;   // Progress bar widget
    QLabel *loadingLabel;        // "LOADING..." text label
    QTimer *timer;               // Timer for animation
    int currentProgress;         // Current progress value (0-100)
};

#endif // LOADINGDIALOG_H
