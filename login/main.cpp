// Include Qt framework for creating GUI applications
#include <QApplication>
// Include the main window class definition
#include "mainwindow.h"

/**
 * Main entry point of the Course Timetable Management System
 *
 * This function initializes the Qt application framework and creates
 * the main login window for the system.
 */
int main(int argc, char *argv[])
{
    // Create the Qt application object - manages application-wide resources
    // argc and argv are command-line arguments passed to the program
    QApplication app(argc, argv);

    // Create the main login window instance
    MainWindow window;

    // Display the window on screen - makes it visible to the user
    window.show();

    // Start the event loop - keeps the application running until user closes it
    // This function blocks until the application exits
    return app.exec();
}
