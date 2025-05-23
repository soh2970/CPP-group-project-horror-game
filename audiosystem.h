#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>

class AudioSystem : public QObject
{
    Q_OBJECT

public:
    explicit AudioSystem(QObject *parent = nullptr);
    ~AudioSystem();

    // Background music control
    void playBackgroundMusic(const QString &filePath, bool loop = true);
    void pauseBackgroundMusic();
    void resumeBackgroundMusic();
    void stopBackgroundMusic();
    void setBackgroundVolume(float volume);

    // Sound effects control
    void playSoundEffect(const QString &filePath);
    void stopSoundEffects();
    void setEffectsVolume(float volume);

private slots:
    void handleBackgroundMusicStateChange(QMediaPlayer::PlaybackState state);

private:
    QMediaPlayer *backgroundPlayer;
    QAudioOutput *backgroundOutput;

    QMediaPlayer *effectsPlayer;
    QAudioOutput *effectsOutput;
    QString currentBackgroundMusic; // Track current music file

    bool shouldLoop = true;        // Loop control flag
    void initializePlayers();
};

#endif // AUDIOSYSTEM_H
