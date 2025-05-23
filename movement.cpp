/**
 * @file movement.cpp
 * @brief Implementation of the Movement class for player movement and collision
 * @author Kiet Train
 */

#include "movement.h"
#include "player.h"
#include <QGraphicsItem>
#include <QList>
#include <QDebug>


Movement::Movement(Player* player, QObject* parent)
    : QObject(parent)
    , m_player(player)
{

}

/**
 * @brief Moves the player upward
 * @param step represents the distance to move in pixels
 *
 * Handles movement with position validation and collision checking
 */

void Movement::moveUp(int step)
{

    // Moves the player upward
    if (!m_player) return;
    QPointF oldPos = m_player->pos();
    m_player->setPos(m_player->x(), m_player->y() - step);

    // Checks collisions with non-background items
    if (hasCollision()) {
        m_player->setPos(oldPos);
    }

}

/**
 * @brief Moves the player downward
 * @paramstep represents the distance to move in pixels
 *
 * Handles movement with position validation and collision checking
 */

void Movement::moveDown(int step)
{

    // Moves the player downward
    if (!m_player) return;
    QPointF oldPos = m_player->pos();
    m_player->setPos(m_player->x(), m_player->y() + step);

    // Checks collisions with non-background items
    if (hasCollision()) {
        m_player->setPos(oldPos);
    }

}

/**
 * @brief Moves the player leftward
 * @paramstep represents the distance to move in pixels
 *
 * Handles movement with position validation and collision checking
 */

void Movement::moveLeft(int step)
{

    // Moves the player leftward
    if (!m_player) return;
    QPointF oldPos = m_player->pos();
    m_player->setPos(m_player->x() - step, m_player->y());

    // Checks collisions with non-background items
    if (hasCollision()) {
        m_player->setPos(oldPos);
    }
}

/**
 * @brief Moves the player rightward
 * @paramstep represents the distance to move in pixels
 *
 * Handles movement with position validation and collision checking
 */

void Movement::moveRight(int step)
{

    // Moves the player rightward
    if (!m_player) return;
    QPointF oldPos = m_player->pos();
    m_player->setPos(m_player->x() + step, m_player->y());

    // Checks collisions with non-background items
    if (hasCollision()) {
        m_player->setPos(oldPos);
    }

}

/**
 * @brief Checks for collisions with other game objects
 * @return true if a collision is detected and false otherwise
 *
 * Checks for all colliding objects and responds accordingly
 */

bool Movement::hasCollision()
{

    if (!m_player) return false;

    QList<QGraphicsItem*> collisions = m_player->collidingItems();

    // Filters out non-collidable items
    for (QGraphicsItem* item : collisions) {

        // Check if it is not the background
        if (item->zValue() != -1) {
            return true;
        }
    }

    return false;

}
