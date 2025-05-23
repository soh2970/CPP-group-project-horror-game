#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QGraphicsRectItem>

class Movement;

class Player : public QGraphicsPixmapItem
{
public:
    Player();

    void setMovement(Movement *movement);
    Movement* getMovement() const { return m_movement; }

    // Health bar related functions
    int getHealth() const;
    void decreaseHealth(int amount);
    void increaseHealth(int amount);
    void setHealth(int value);
    bool isAlive() const;

    // Health bar
    void updateHealthBar();
    void setHealthBarVisible(bool visible);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Movement *m_movement;
    QPixmap sprite;  // Store the sprite image

    // Health properties
    int maxHealth;
    int currentHealth;

    // Healh bar visuals
    QGraphicsRectItem *healthBarBackground;
    QGraphicsRectItem *healthBar;
    bool healthBarVisible;
};

#endif // PLAYER_H
