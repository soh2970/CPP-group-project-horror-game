#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <QObject>

class Player;

class Movement : public QObject
{
    Q_OBJECT
public:
    explicit Movement(Player *player, QObject *parent = nullptr);

    // Movement actions
    void moveUp(int step);
    void moveDown(int step);
    void moveLeft(int step);
    void moveRight(int step);
    bool hasCollision();

private:
    Player *m_player;  // Pointer to the player we'll move
};

#endif // MOVEMENT_H
