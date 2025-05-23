#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <QObject>
#include <QMap>
#include <QKeyEvent>
#include "player.h"

class InputHandler : public QObject
{
    Q_OBJECT

public:
    explicit InputHandler(QObject *parent = nullptr);

    // Handle key press events
    void handleKeyPress(QKeyEvent *event);

    // Set the player that this input handler controls
    void setPlayer(Player *player);

private slots:
    // Handle voice recognition errors
    void onVoiceError(const QString& error);

private:
    // The player controlled by this input handler
    Player *m_player;

    // Map of key bindings (key to direction)
    QMap<int, QString> m_keyBindings;

    // Step size for movement
    int m_step;
};

#endif // INPUTHANDLER_H
