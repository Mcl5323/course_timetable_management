/**
 * ============================================================================
 * Student Course Registration & Timetable Management System
 * ============================================================================
 *
 * Main Entry Point (main.cpp)
 *
 * This application allows students to:
 * 1. Register/Login to their account
 * 2. Add and manage courses with time slots
 * 3. Generate all possible timetable combinations
 * 4. View and export timetables as images
 *
 * Data Structures Used:
 * - LinkedList: Store courses (dynamic, supports CRUD operations)
 * - HashTable: Group courses by name for combination generation
 *
 * Algorithms Used:
 * - Linear Search: Search courses by keyword
 * - QuickSort: Sort courses by name/day/time/classroom
 * - Recursive Backtracking: Generate all timetable combinations
 *
 * ============================================================================\n */

#include <QApplication>
#include "mainwindow.h"

/**
 * ============================================================================
 * Main Function - Application Entry Point
 * ============================================================================
 * Initializes the Qt application framework and displays the main login window.
 * The event loop (app.exec()) handles all user interactions until the app closes.
 *
 * @param argc - Number of command line arguments
 * @param argv - Array of command line argument strings
 * @return int - Application exit code (0 = success)
 * ============================================================================
 */
int main(int argc, char *argv[])
{
    /* Create Qt Application instance (required for all Qt GUI applications) */
    QApplication app(argc, argv);

    /* Create and show the main window (Login page) */
    MainWindow window;
    window.show();

    /* Start the Qt event loop - processes user input until window is closed */
    return app.exec();
}
