/**
 * @file voicechallenge.h
 * @brief Handles voice/text-based challenges and jumpscare system
 * @author Steph Oh
 */

#ifndef VOICECHALLENGE_H
#define VOICECHALLENGE_H

#include <QObject>
#include <QTimer>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QStringList>
#include <QElapsedTimer>
#include <QPainter>
#include <QFileInfo>
#include <QLineEdit>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include "player.h"

class QGraphicsPixmapItem;

/**
 * @brief Manages voice recognition challenges and jumpscare effects
 *
 * Handles challenge timing, UI display, input validation,
 * and jumpscare consequences for failed challenges
 */

class VoiceChallenge : public QObject
{
    Q_OBJECT

public:
    explicit VoiceChallenge(QGraphicsScene *scene, Player *player, QObject *parent = nullptr);
    ~VoiceChallenge();

    // Start the challenge system (will trigger first challenge after the interval)
    void start();

    // Stop the challenge system
    void stop();

    // Set the path to the jumpscare images folder
    void setJumpscareFolder(const QString &path);

    // Set the interval between challenges (in milliseconds)
    void setChallengeInterval(int ms);

    // Set the time allowed for a challenge (in milliseconds)
    void setChallengeTime(int ms);

private slots:
    // Show a new challenge
    void showChallenge();

    // Handle challenge timeout
    void onChallengeTimeout();

    // Hide the jumpscare image
    void hideJumpscare();

    // Hide the success checkmark
    void hideSuccessCheck();

private:
    // Create the challenge UI elements
    void createChallengeUI();

    // Update the countdown timer display
    void updateCountdown();

    // Show a jumpscare image
    void showJumpscare();

    // Try alternative methods to load the jumpscare image
    void tryAlternativeImageLoad(const QString& path);

    // Show success checkmark
    void showSuccessCheck();

    // Get a random challenge word/phrase
    QString getRandomChallenge();

    // Get a random jumpscare image path
    QString getRandomJumpscareImage();

    // Check the user's text input
    void checkTextInput(const QString& input);

    // The scene to add UI elements to
    QGraphicsScene *m_scene;

    // Player reference for health management
    Player *m_player;

    // Timer between challenges
    QTimer *m_challengeIntervalTimer;

    // Timer for challenge duration
    QTimer *m_challengeTimer;

    // Timer for countdown display
    QTimer *m_countdownTimer;

    // Timer for hiding jumpscares
    QTimer *m_jumpscareTimer;

    // Timer for hiding success checkmark
    QTimer *m_successCheckTimer;

    // Elapsed timer for tracking challenge time
    QElapsedTimer m_elapsedTimer;

    // Background overlay
    QGraphicsRectItem *m_overlay;

    // Challenge text
    QGraphicsTextItem *m_challengeText;

    // Countdown text
    QGraphicsTextItem *m_countdownText;

    // Jumpscare image
    QGraphicsPixmapItem *m_jumpscareImage;

    // Success checkmark
    QGraphicsPixmapItem *m_successCheck;

    // Text input field
    QLineEdit *m_inputField;
    QGraphicsProxyWidget *m_inputFieldProxy;

    // Submit button
    QPushButton *m_submitButton;
    QGraphicsProxyWidget *m_buttonProxy;

    // List of challenge phrases
    QStringList m_challengePhrases;

    // Current challenge text
    QString m_currentChallenge;

    // Path to jumpscare images folder
    QString m_jumpscareFolder;

    // Challenge interval (milliseconds)
    int m_challengeInterval;

    // Challenge time allowed (milliseconds)
    int m_challengeTime;

    // Whether a challenge is active
    bool m_challengeActive;

};

#endif // VOICECHALLENGE_H
