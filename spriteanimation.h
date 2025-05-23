#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QMap>
#include <QVector>
#include <QPixmap>

enum class AnimationState {
    IdleDown, WalkDown,
    IdleLeft, WalkLeft,
    IdleRight, WalkRight,
    IdleUp, WalkUp
};

class SpriteAnimation : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit SpriteAnimation(QObject *parent = nullptr);

    void setAnimation(AnimationState state);

private slots:
    void updateFrame();

private:
    QMap<AnimationState, QVector<QPixmap>> m_animations;
    AnimationState m_currentState;
    int m_currentFrame;
    QTimer m_timer;
};

#endif
