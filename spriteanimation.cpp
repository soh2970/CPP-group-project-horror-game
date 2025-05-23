#include "spriteanimation.h"
#include <QDebug>

SpriteAnimation::SpriteAnimation(QObject *parent)
    : QObject(parent),
    QGraphicsPixmapItem(),
    m_currentState(AnimationState::IdleDown),
    m_currentFrame(0)
{
    QPixmap idleSheet(":/images/Idle_full.png");
    if (idleSheet.isNull()) {
        qDebug() << "Failed to load idle sprite sheet!";
    } else {
        int idleWidth = idleSheet.width();
        int idleHeight = idleSheet.height();
        int rows = 4;
        int columns = 12;

        int frameW = idleWidth / columns;
        int frameH = idleHeight / rows;

        // IdleDown
        QVector<QPixmap> idleDownFrames;
        for (int col = 0; col < columns; ++col) {
            idleDownFrames.push_back(idleSheet.copy(
                col * frameW,
                0 * frameH,
                frameW, frameH
                ));
        }
        m_animations[AnimationState::IdleDown] = idleDownFrames;
        qDebug() << "IdleDown frames loaded:" << idleDownFrames.size();

        // IdleLeft
        QVector<QPixmap> idleLeftFrames;
        for (int col = 0; col < columns; ++col) {
            idleLeftFrames.push_back(idleSheet.copy(
                col * frameW,
                1 * frameH,
                frameW, frameH
                ));
        }
        m_animations[AnimationState::IdleLeft] = idleLeftFrames;
        qDebug() << "IdleLeft frames loaded:" << idleLeftFrames.size();

        // IdleRight
        QVector<QPixmap> idleRightFrames;
        for (int col = 0; col < columns; ++col) {
            idleRightFrames.push_back(idleSheet.copy(
                col * frameW,
                2 * frameH,
                frameW, frameH
                ));
        }
        m_animations[AnimationState::IdleRight] = idleRightFrames;
        qDebug() << "IdleRight frames loaded:" << idleRightFrames.size();

        int columnsIdleUp = 6;
        int frameWUp = idleWidth / columns;

        QVector<QPixmap> idleUpFrames;
        for (int col = 0; col < columnsIdleUp; ++col) {
            idleUpFrames.push_back(idleSheet.copy(
                col * frameWUp,
                3 * frameH,
                frameWUp, frameH
                ));
        }
        m_animations[AnimationState::IdleUp] = idleUpFrames;
        qDebug() << "IdleUp frames loaded:" << idleUpFrames.size();
    }

    // Load the Walk sheet
    QPixmap walkSheet(":/images/Walk_full.png");
    if (walkSheet.isNull()) {
        qDebug() << "Failed to load walk sprite sheet!";
    } else {
        int walkWidth = walkSheet.width();
        int walkHeight = walkSheet.height();
        int columns = 5;
        int rows = 4;

        int frameW = walkWidth / columns;
        int frameH = walkHeight / rows;

        // WalkDown
        QVector<QPixmap> walkDownFrames;
        for (int col = 0; col < columns; ++col) {
            walkDownFrames.push_back(walkSheet.copy(
                col * frameW,
                0 * frameH,
                frameW, frameH
                ));
        }
        m_animations[AnimationState::WalkDown] = walkDownFrames;
        qDebug() << "WalkDown frames loaded:" << walkDownFrames.size();

        // WalkLeft
        QVector<QPixmap> walkLeftFrames;
        for (int col = 0; col < columns; ++col) {
            walkLeftFrames.push_back(walkSheet.copy(
                col * frameW,
                1 * frameH,
                frameW, frameH
                ));
        }
        m_animations[AnimationState::WalkLeft] = walkLeftFrames;
        qDebug() << "WalkLeft frames loaded:" << walkLeftFrames.size();

        // WalkRight
        QVector<QPixmap> walkRightFrames;
        for (int col = 0; col < columns; ++col) {
            walkRightFrames.push_back(walkSheet.copy(
                col * frameW,
                2 * frameH,
                frameW, frameH
                ));
        }
        m_animations[AnimationState::WalkRight] = walkRightFrames;
        qDebug() << "WalkRight frames loaded:" << walkRightFrames.size();

        // WalkUp
        QVector<QPixmap> walkUpFrames;
        for (int col = 0; col < columns; ++col) {
            walkUpFrames.push_back(walkSheet.copy(
                col * frameW,
                3 * frameH,
                frameW, frameH
                ));
        }
        m_animations[AnimationState::WalkUp] = walkUpFrames;
        qDebug() << "WalkUp frames loaded:" << walkUpFrames.size();
    }

    if (!m_animations[m_currentState].isEmpty()) {
        setPixmap(m_animations[m_currentState][m_currentFrame]);
    }

    connect(&m_timer, &QTimer::timeout, this, &SpriteAnimation::updateFrame);
    m_timer.start(150);
}
