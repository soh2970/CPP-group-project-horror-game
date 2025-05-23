/**
 * @file mainwindow.cpp
 * @brief Implementation of the MainWindow class
 * @author Evan Van, Cherie Duong
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamewindow.h"
#include <QPixmap>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QApplication>

/**
 * @brief Constructs the MainWindow
 * @param parent which represents the parent widget
 *
 * This constructor sets up the UI, configures the properties of the window screen, initializes audio,
 * and establishes the background image and necessary main menu buttons
 */

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), m_audioSystem_main(nullptr)
{

    ui->setupUi(this);

    // Hides the status bar
    statusBar()->hide();

    // Sets the window size to 1440 x 900
    this->resize(1440, 900);

    // Initializes the audio system
    setupAudio();
    m_audioSystem_main->playBackgroundMusic("qrc:/horror_music/background_main.mp3", true);

    // Creates and configures the background image
    QLabel *background = new QLabel(this);
    QPixmap bgImage(":/images/main_menu.png");
    background->setPixmap(bgImage);
    background->setGeometry(0, 0, 1440, 900);
    background->lower();

    // Creates and configures the "New Game" button
    QPushButton *newGameButton = new QPushButton(this);
    newGameButton->setGeometry(600, 375, 240, 80);
    newGameButton->setStyleSheet(
        "QPushButton {"
        "   background-image: url(:/images/new_game.png);"
        "   background-repeat: no-repeat;"
        "   background-position: center;"
        "   border: none;"
        "}"
        );
    newGameButton->setText("");
    connect(newGameButton, &QPushButton::clicked, this, &MainWindow::onNewGameButtonClicked);

    // Creates and configure the "Exit" button
    QPushButton *exitButton = new QPushButton(this);
    exitButton->setGeometry(600, 625, 240, 80);
    exitButton->setStyleSheet(
        "QPushButton {"
        "   background-image: url(:/images/exit.png);"
        "   background-repeat: no-repeat;"
        "   background-position: center;"
        "   border: none;"
        "}"
        );
    exitButton->setText("");
    connect(exitButton, &QPushButton::clicked, this, &QApplication::quit);

}

/**
 * @brief Destroys the MainWindow and cleans up all necessary resources
 */

MainWindow::~MainWindow()
{

    delete m_audioSystem_main;
    delete ui;

}

/**
 * @brief Handles New Game button click events
 *
 * This function logs the button press, hides the main window, stops background music, and creates
 * a new game window for the actual gameplay itself
 */

void MainWindow::onNewGameButtonClicked()
{

    // Hides the main menu
    this->hide();

    // Stops background music
    m_audioSystem_main->stopBackgroundMusic();

    // Creates the game window that contains the game scene
    GameWindow *gameWindow = new GameWindow();
    gameWindow->show();

}

/**
 * @brief Initializes the audio system
 *
 * This function creates a new instance of AudioSystem
 */

void MainWindow::setupAudio()
{
    m_audioSystem_main = new AudioSystem(this);
}
