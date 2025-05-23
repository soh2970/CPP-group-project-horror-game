/**
 * @file gamewindow.cpp
 * @brief Implementation of the GameWindow class for the main game interface
 * @author Kiet Tran, Steph Oh, Cherie Duong, and Evan Van
 */

#include "gamewindow.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QPushButton>
#include <QDir>
#include <QFile>
#include "player.h"
#include "movement.h"
#include "inputhandler.h"
#include "voicechallenge.h"

/**
 * @brief Constructs the GameWindow
 * @param parent represents the parent widget
 *
 * Initializes the game scene, background graphics, sprite, movement system, collision walls, audio system, and voice challenges
 *
 */

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent), m_voiceChallenge(nullptr), m_audioSystem(nullptr)
{

    // Creates a scene and sets its size
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 1440, 900);

    // Loads the background image
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(QPixmap(":/images/room1_bg.png"));
    background->setPos(0, 0);
    background->setZValue(-1);
    scene->addItem(background);

    // Creates a view to display the scene
    view = new QGraphicsView(scene, this);
    view->setFixedSize(1440, 900);
    view->setFocusPolicy(Qt::StrongFocus);
    view->setFocus();
    setCentralWidget(view);

    // Initializes the audio system
    setupAudio();
    m_audioSystem->playBackgroundMusic("qrc:/horror_music/background_music1.mp3", true);

    // Creates and configures the player
    Player *player = new Player();
    player->setPos(695, 800);
    player->setZValue(1);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    scene->addItem(player);

    // Creates the movement handler for the player
    Movement *movement = new Movement(player);
    player->setMovement(movement);

    // Set focus to the view first, then to the player
    view->setFocus();
    QTimer::singleShot(100, [player]() {
        player->setFocus();
    });

    // Adds collision walls
    addWalls();

    // Creates an input handler and connects to the player
    m_inputHandler = new InputHandler(this);
    m_inputHandler->setPlayer(player);

    // Initializes a text challenge system with a short delay
    QTimer::singleShot(500, [this]() {
        initVoiceChallenge();
    });

}

/**
 * @brief Destroys the GameWindow and cleans up resources
 *
 * Cleans up audio system resources
 */

GameWindow::~GameWindow()
{

    delete m_audioSystem;

}

/**
 * @brief Adds collision walls to the game scene
 *
 * Creates transparent rectangular collision objects to mimic walls and other objects within the game scene
 */

void GameWindow::addWalls()
{

    // Wall object for collision
    QGraphicsRectItem *wall1 = new QGraphicsRectItem(0, 310, 480, 40);
    wall1->setBrush(Qt::transparent);
    wall1->setPen(Qt::NoPen);
    scene->addItem(wall1);

    // Wall object for collision
    QGraphicsRectItem *wall2 = new QGraphicsRectItem(450, 280, 1000, 40);
    wall2->setBrush(Qt::transparent);
    wall2->setPen(Qt::NoPen);
    scene->addItem(wall2);

    // Wall object for collision
    QGraphicsRectItem *wall3 = new QGraphicsRectItem(0, 348, 40, 600);
    wall3->setBrush(Qt::transparent);
    wall3->setPen(Qt::NoPen);
    scene->addItem(wall3);

    // Wall object for collision
    QGraphicsRectItem *wall4 = new QGraphicsRectItem(0, 860, 590, 40);
    wall4->setBrush(Qt::transparent);
    wall4->setPen(Qt::NoPen);
    scene->addItem(wall4);

    // Wall object for collision
    QGraphicsRectItem *wall5 = new QGraphicsRectItem(850, 860, 600, 40);
    wall5->setBrush(Qt::transparent);
    wall5->setPen(Qt::NoPen);
    scene->addItem(wall5);

    // Wall object for collision
    QGraphicsRectItem *wall6 = new QGraphicsRectItem(1400, 330, 40, 600);
    wall6->setBrush(Qt::transparent);
    wall6->setPen(Qt::NoPen);
    scene->addItem(wall6);

    // Wall object for collision
    QGraphicsRectItem *wall7 = new QGraphicsRectItem(218, 532, 280, 138);
    wall7->setBrush(Qt::transparent);
    wall7->setPen(Qt::NoPen);
    scene->addItem(wall7);

    // Wall object for collision
    QGraphicsRectItem *wall8 = new QGraphicsRectItem(923, 532, 280, 138);
    wall8->setBrush(Qt::transparent);
    wall8->setPen(Qt::NoPen);
    scene->addItem(wall8);

}

/**
 * @brief Initializes the text challenge system
 *
 * Sets up a player reference and the parameters for the challenge
 */

void GameWindow::initVoiceChallenge()
{

    Player* player = nullptr;
    QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem* item : items) {
        Player* p = dynamic_cast<Player*>(item);
        if (p) {
            player = p;
            break;
        }
    }

    // Create text challenge with our scene and player
    m_voiceChallenge = new VoiceChallenge(scene, player, this);
    m_voiceChallenge->setJumpscareFolder(":/jumpscares");

    // Log available images for debugging
    qDebug() << "Checking for jumpscare images:";
    for (int i = 1; i <= 12; i++) {
        QString path = QString(":/jumpscares/image%1.jpg").arg(i);
        QFile file(path);
        if (file.exists()) {
            qDebug() << "  Found image:" << path;
        } else {
            qDebug() << "  Missing image:" << path;
        }
    }

    // Sets the challenge parameters (30 secs in between challenges)
    m_voiceChallenge->setChallengeInterval(30000);

    // 10 secs to complete a challenge
    m_voiceChallenge->setChallengeTime(10000);

    // Starts the challenge
    m_voiceChallenge->start();
    qDebug() << "Text challenge system initialized with resource path.";

}

/**
 * @brief Initializes the audio system for the game
 *
 * Creates a new AudioSystem instance
 */

void GameWindow::setupAudio()
{

    m_audioSystem = new AudioSystem(this);

}
