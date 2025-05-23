/**
 * @file player.cpp
 * @brief Implementation of the Player class with health management and movement
 * @author Kiet Tran, Steph Oh
 */

#include "player.h"
#include "movement.h"
#include <QPixmap>
#include <QDebug>
#include <QBrush>
#include <QPen>

/**
 * @brief Constructs a Player with default settings
 *
 * Initializes the sprite, health system, movement controls, and visual elements
 */

Player::Player() : m_movement(nullptr), maxHealth(100), currentHealth(100), healthBarVisible(true)
{

    // Loads the sprite image from the resource file
    QPixmap originalSprite(":/images/sprite_back.png");

    // Scales the sprite to 75 x 75 pixels
    QPixmap scaledSprite = originalSprite.scaled(75, 75, Qt::KeepAspectRatio);

    // Sets the scaled pixmap for the player
    setPixmap(scaledSprite);

    // Ensures we can receive key events
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    // Initializes the health bar
    healthBarBackground = new QGraphicsRectItem(this);
    healthBar = new QGraphicsRectItem(this);

    // Sets the health bar appearance
    healthBarBackground->setRect(0,-15,75,10);
    healthBarBackground->setBrush(QBrush(Qt::gray));
    healthBarBackground->setPen(QPen(Qt::NoPen));

    healthBar->setRect(0,-15,75,10);
    healthBar->setBrush(Qt::red);
    healthBar->setPen(Qt::NoPen);

    // Updates the appearance of the health bar
    updateHealthBar();

}

/**
 * @brief Sets the movement controller for this player
 *
 * Takes ownership of the Movement controller and enables player movement
 */

void Player::setMovement(Movement *movement)
{

    m_movement = movement;
    qDebug() << "Movement set for player:" << (m_movement != nullptr);

}

/**
 * @brief Gets the current health value
 *
 * Returns the player's current health points between 0 and maxHealth
 */

int Player::getHealth() const
{

    return currentHealth;

}

/**
 * @brief Reduces the player health by a specified amount
 *
 * Handles health clamping, death state, and visual updates
 */

void Player::decreaseHealth(int amount)
{

    currentHealth = qMax(0, currentHealth - amount);
    updateHealthBar();

    if (currentHealth <= 0) {
        qDebug() << "Player has died!";
    }

}

/**
 * @brief Increases player health by specified amount
 *
 * Handles health clamping and visual updates
 */

void Player::increaseHealth(int amount)
{

    currentHealth = qMin(maxHealth, currentHealth + amount);
    updateHealthBar();

}

/**
 * @brief Sets health to exact value
 *
 * Value is automatically set between 0 and maxHealth
 */

void Player::setHealth(int value)
{

    currentHealth = qBound(0, value, maxHealth);
    updateHealthBar();

}

/**
 * @brief Checks if the player is alive
 *
 * Returns true if health is greater than 0 and false otherwise
 */

bool Player::isAlive() const
{

    return currentHealth > 0;

}

/**
 * @brief Updates the health bar visuals
 *
 * Adjusts the width based on health percentage and changes color at the 60% and 30% thresholds
 */

void Player::updateHealthBar()
{

    if (!healthBarVisible) return;

    // Calculates the health percentage
    float healthPercentage = static_cast<float>(currentHealth) / maxHealth;

    // Updates the health bar width
    healthBar->setRect(0, -15, 75 * healthPercentage, 10);

    // Changes color based on health level
    if (healthPercentage > 0.6) {
        healthBar->setBrush(QBrush(Qt::green));
    } else if (healthPercentage > 0.3) {
        healthBar->setBrush(QBrush(QColor(255, 165, 0)));
    } else {
        healthBar->setBrush(QBrush(Qt::red));
    }

}

/**
 * @brief Toggles health bar visibility
 *
 * Shows or hides both the health bar and its background
 */

void Player::setHealthBarVisible(bool visible)
{

    healthBarVisible = visible;
    healthBarBackground->setVisible(visible);
    healthBar->setVisible(visible);

}

/**
 * @brief Handles keyboard input for player movement
 *
 * Processes WASD keys for movement and updates player sprite accordingly
 */

void Player::keyPressEvent(QKeyEvent *event)
{

    // Sets the step distance to 15px
    int step = 15;
    QPixmap originalSprite;
    QPixmap scaledSprite;

    if (!m_movement) {
        QGraphicsPixmapItem::keyPressEvent(event);
        return;
    }

    // Swtich statement to change sprite direction
    switch (event->key()) {
    case Qt::Key_W:
        m_movement->moveUp(step);
        originalSprite.load(":/images/sprite_forward.png");
        break;
    case Qt::Key_S:
        m_movement->moveDown(step);
        originalSprite.load(":/images/sprite_back.png");
        break;
    case Qt::Key_A:
        m_movement->moveLeft(step);
        originalSprite.load(":/images/sprite_left.png");
        break;
    case Qt::Key_D:
        m_movement->moveRight(step);
        originalSprite.load(":/images/sprite_right.png");
        break;
    default:
        return;
    }

    scaledSprite = originalSprite.scaled(75, 75, Qt::KeepAspectRatio);
    setPixmap(scaledSprite);

}
