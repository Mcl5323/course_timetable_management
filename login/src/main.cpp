#include <QApplication>
#include "mainwindow.h"

// Main entry point - Initialize Qt application and show login window
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();  // Start event loop
}
