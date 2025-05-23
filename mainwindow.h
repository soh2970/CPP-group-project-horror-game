/**
 * @file mainwindow.h
 * @brief Main application window class with UI and audio controls.
 * @author Evan Van
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "audiosystem.h"

namespace Ui {

    class MainWindow;

}

/**
 * @class MainWindow
 * @brief The main window of the game application
 *
 * Handles the user interface of the main menu window and integrates the background audio system
 */

class MainWindow : public QMainWindow

{

    Q_OBJECT

public:

    // Constructs the main window
    MainWindow(QWidget *parent = nullptr);

    // Destroys the main window
    ~MainWindow();

    /**
     * @brief Getter method for the audio system instance
     * @return Returns a pointer to the AudioSystem object
     */

    AudioSystem* audioSystem() const { return m_audioSystem_main; }


private slots:

    // Function is called when the user clicks the "New Game" button
    void onNewGameButtonClicked();

private:

    // Represents a pointer to the UI components
    Ui::MainWindow *ui;

    // An instance of the main audio system
    AudioSystem *m_audioSystem_main;

    // Sets up the default audio system
    void setupAudio();

};

#endif
