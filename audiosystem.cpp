/**
 * @file audiosystem.cpp
 * @brief Implementation of the AudioSystem class for handling game audio
 * @author Cherie Duong
 */

#include "audiosystem.h"
#include <QDebug>

/**
 * @brief Constructs an AudioSystem object
 * @param parent represents the parent QObject
 *
 * Initializes audio for the player
 */

AudioSystem::AudioSystem(QObject *parent) : QObject(parent)
{

    initializePlayers();

}

/**
 * @brief Destroys the AudioSystem object
 *
 * Stops all audio and cleans up media players
 */

AudioSystem::~AudioSystem()
{
    backgroundPlayer->stop();
    effectsPlayer->stop();

    delete backgroundPlayer;
    delete backgroundOutput;
    delete effectsPlayer;
    delete effectsOutput;
}

/**
 * @brief Initializes audio players and output
 *
 * Sets up the background music and sound effects
 */

void AudioSystem::initializePlayers()
{

    // Sets up the background audio
    backgroundPlayer = new QMediaPlayer(this);
    backgroundOutput = new QAudioOutput(this);
    backgroundPlayer->setAudioOutput(backgroundOutput);
    backgroundOutput->setVolume(0.5f);

    // Connects the playback state change signal
    connect(backgroundPlayer, &QMediaPlayer::playbackStateChanged, this, &AudioSystem::handleBackgroundMusicStateChange);

    // Sets up the sound effects
    effectsPlayer = new QMediaPlayer(this);
    effectsOutput = new QAudioOutput(this);
    effectsPlayer->setAudioOutput(effectsOutput);
    effectsOutput->setVolume(0.7f);

    // Condcuts error handling for both players
    connect(backgroundPlayer, &QMediaPlayer::errorOccurred, this, [this](){
        qDebug() << "Background music error:" << backgroundPlayer->errorString();
    });

    connect(effectsPlayer, &QMediaPlayer::errorOccurred, this, [this](){
        qDebug() << "Sound effect error:" << effectsPlayer->errorString();
    });

}

/**
 * @brief Handles state changes to the background music
 * @param state represents the new playback state
 *
 * Automatically restarts the playback if stopped while in loop mode
 */

void AudioSystem::handleBackgroundMusicStateChange(QMediaPlayer::PlaybackState state)
{

    if (state == QMediaPlayer::StoppedState && shouldLoop && !currentBackgroundMusic.isEmpty()) {

        // Rewinds to start
        backgroundPlayer->setPosition(0);

        // Restarts playback
        backgroundPlayer->play();

    }

}

/**
 * @brief Plays background music
 * @param filePath represents the Path to the audio file
 * @param loop represents whether to loop the music continuously or not
 *
 * Stops any background music that is currently playing before starting a new playback
 */

void AudioSystem::playBackgroundMusic(const QString &filePath, bool loop)
{

    shouldLoop = loop;
    currentBackgroundMusic = filePath;

    backgroundPlayer->stop();
    backgroundPlayer->setSource(QUrl(filePath));
    backgroundPlayer->play();

}

/**
 * @brief Pauses background music playback
 *
 * Saves the current playback position for later resuming
 */

void AudioSystem::pauseBackgroundMusic()
{

    backgroundPlayer->pause();

}

/**
 * @brief Resumes the background music
 *
 * Continues playback from paused position.
 */

void AudioSystem::resumeBackgroundMusic()
{

    backgroundPlayer->play();

}

/**
 * @brief Stops the background music
 *
 * Resets the playback position and disables looping
 */

void AudioSystem::stopBackgroundMusic()
{

    shouldLoop = false;
    backgroundPlayer->stop();

}

/**
 * @brief Sets the volume of the background music
 * @param volume represents the volume level
 */

void AudioSystem::setBackgroundVolume(float volume)
{

    backgroundOutput->setVolume(volume);

}

/**
 * @brief Plays a sound effect
 * @param filePath represents the path to the audio file
 *
 * Stops any currently playing effect before starting a new playback
 */

void AudioSystem::playSoundEffect(const QString &filePath)
{

    effectsPlayer->stop();
    effectsPlayer->setSource(QUrl(filePath));
    effectsPlayer->play();

}

/**
 * @brief Stops all sound effects
 */

void AudioSystem::stopSoundEffects()
{

    effectsPlayer->stop();

}

/**
 * @brief Sets the volume for sound effects
 * @param volume represents the volume level
 */

void AudioSystem::setEffectsVolume(float volume)
{

    effectsOutput->setVolume(volume);

}
