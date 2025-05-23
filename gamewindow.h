#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include "audiosystem.h"

class QGraphicsScene;
class QGraphicsView;
class InputHandler;
class VoiceChallenge;

class GameWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

    AudioSystem* audioSystem() const { return m_audioSystem; }  // Getter for audio system

private:
    // Helper function to add walls to the scene
    void addWalls();

    // Initialize the voice challenge system
    void initVoiceChallenge();

    // declare audio system
    void setupAudio();

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    InputHandler *m_inputHandler;
    VoiceChallenge *m_voiceChallenge;
    AudioSystem *m_audioSystem;  // Add audio system member
};

#endif // GAMEWINDOW_H
