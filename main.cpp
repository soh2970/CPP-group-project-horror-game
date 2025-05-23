/**
 * @file main.cpp
 * @brief Main entry point for the application
 * @author Evan Van
 */

#include "mainwindow.h"
#include <QApplication>

/**
 * @brief Entry point for the application
 * @param argc represents the argument count
 * @param argv represents the argument values
 * @return Exit status code
 *
 * Initializes the QApplication instance and sets up the main window
 */

int main(int argc, char *argv[])
{

    // Initialize Qt application
    QApplication a(argc, argv);

    // Creates the main window
    MainWindow w;
    w.show();

    // Enters the main event loop
    return a.exec();

}
