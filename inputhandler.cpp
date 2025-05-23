/**
 * @file inputhandler.cpp
 * @brief Implementation of the InputHandler class for player control
 * @author Steph Oh
 */

#include "inputhandler.h"
#include "movement.h"
#include <QDebug>

/**
 * @brief Constructs an InputHandler with default key bindings
 * @param parent represents the parent QObject
 *
 * Initializes the default WASD key binds
 */

InputHandler::InputHandler(QObject *parent) : QObject(parent), m_player(nullptr), m_step(15)
{

    // Initializes the key bindings
    m_keyBindings[Qt::Key_W] = "up";
    m_keyBindings[Qt::Key_S] = "down";
    m_keyBindings[Qt::Key_A] = "left";
    m_keyBindings[Qt::Key_D] = "right";

    qDebug() << "InputHandler initialized";
}

/**
 * @brief Handles keyboard input events for player movement
 * @param event represents the QKeyEvent containing key press information
 *
 * Processes WASD keys to move the player in the corresponding direction and to change the sprite's direction
 */

void InputHandler::handleKeyPress(QKeyEvent *event)
{

    if (!m_player) {
        qWarning() << "No player set for input handler";
        return;
    }

    // Handles key press for movement
    int step = 15;
    QPixmap originalSprite;

    Movement* movement = m_player->getMovement();

    // Error handling if no movement component is found
    if (!movement) {
        qWarning() << "No movement component found for player";
        return;
    }

    // Switch statement to handle sprite changes
    switch (event->key()) {
    case Qt::Key_W:
        movement->moveUp(step);
        originalSprite.load(":/images/sprite_forward.png");
        break;
    case Qt::Key_S:
        movement->moveDown(step);
        originalSprite.load(":/images/sprite_back.png");
        break;
    case Qt::Key_A:
        movement->moveLeft(step);
        originalSprite.load(":/images/sprite_left.png");
        break;
    case Qt::Key_D:
        movement->moveRight(step);
        originalSprite.load(":/images/sprite_right.png");
        break;
    default:
        return;
    }

    if (!originalSprite.isNull()) {
        QPixmap scaledSprite = originalSprite.scaled(75, 75, Qt::KeepAspectRatio);
        m_player->setPixmap(scaledSprite);
    }

}

/**
 * @brief Sets the player to be controlled by this input handler
 * @param player represents the pointer to the Player object
 */

void InputHandler::setPlayer(Player *player)
{

    m_player = player;
    qDebug() << "Player set for input handler";

}

/**
 * @brief Handles voice recognition errors
 * @param error represents the error message from the voice recognition system
 */

void InputHandler::onVoiceError(const QString& error)
{

    qWarning() << "Voice recognition error: " << error;

}
