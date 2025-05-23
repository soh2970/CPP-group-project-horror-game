#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QObject>
#include <QAudioInput>
#include <QMediaCaptureSession>
#include <QAudioDevice>
#include <QAudioFormat>
#include <QBuffer>
#include <QVector>

class AudioManager : public QObject
{
    Q_OBJECT

public:
    explicit AudioManager(QObject *parent = nullptr);
    ~AudioManager();

    // Initialize the audio system
    void initAudio();

    // Start and stop listening
    void startListening();
    void stopListening();

    // Check if audio is loud enough (above threshold)
    bool isLoud();

    // Audio callback for processing audio data
    void audioCallback(void* userdata, uint8_t* stream, int len);

    // Noise detection
    void noiseDetected();

signals:
    // Signal emitted when audio level changes
    void audioLevelChanged(qreal level);

    // Signal emitted when noise is detected
    void noiseDetected(qreal level);

private:
    // Audio threshold for detection
    int threshold;

    // Audio specification
    QAudioDevice audioDevice;

    // Audio device ID
    QString audioDeviceId;

    // Audio buffer for processing
    QVector<qint16> audioBuffer;

    // Audio input for capturing
    QAudioInput* audioInput;

    // Media capture session
    QMediaCaptureSession captureSession;

    // Whether audio system is initialized
    bool initialized;

    // Calculate audio level from buffer
    qreal calculateAudioLevel(const QVector<qint16>& buffer);
};

#endif // AUDIOMANAGER_H
