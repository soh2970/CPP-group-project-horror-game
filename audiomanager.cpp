#include "audiomanager.h"
#include <QAudioDevice>
#include <QMediaDevices>
#include <QDebug>
#include <cmath>

AudioManager::AudioManager(QObject *parent)
    : QObject(parent),
    threshold(1000),
    audioInput(nullptr),
    initialized(false)
{
    initAudio();
}

AudioManager::~AudioManager()
{
    stopListening();
}

void AudioManager::initAudio()
{
    // Get default audio input device
    const QAudioDevice inputDevice = QMediaDevices::defaultAudioInput();
    if (!inputDevice.isNull()) {
        audioDevice = inputDevice;
        audioDeviceId = inputDevice.id();

        // Create audio input
        audioInput = new QAudioInput(this);
        captureSession.setAudioInput(audioInput);

        // Set up buffer
        audioBuffer.resize(8192); // 4 seconds at 44.1kHz mono

        initialized = true;
        qDebug() << "Audio system initialized with device:" << inputDevice.description();
    } else {
        qWarning() << "No audio input device found";
    }
}

void AudioManager::startListening()
{
    if (!initialized) {
        qWarning() << "Cannot start listening - audio system not initialized";
        return;
    }

    // Create a simple audio probe/monitor here
    // This is simplified as Qt6's audio handling is different from SDL_Audio used in UML

    qDebug() << "Audio manager started listening";
}

void AudioManager::stopListening()
{
    if (!initialized) {
        return;
    }

    qDebug() << "Audio manager stopped listening";
}

bool AudioManager::isLoud()
{
    if (!initialized) {
        return false;
    }

    // Calculate current audio level
    qreal level = calculateAudioLevel(audioBuffer);

    // Check if above threshold
    if (level > threshold) {
        emit noiseDetected(level);
        return true;
    }

    return false;
}

void AudioManager::audioCallback(void* /*userdata*/, uint8_t* stream, int len)
{
    if (!initialized) {
        return;
    }

    // Copy audio data to our buffer for analysis
    // This is simplified as we don't have direct SDL-style callbacks in Qt6

    // Calculate audio level
    qreal level = calculateAudioLevel(audioBuffer);

    // Emit signal for level change
    emit audioLevelChanged(level);
}

void AudioManager::noiseDetected()
{
    qDebug() << "Noise detected above threshold";
}

qreal AudioManager::calculateAudioLevel(const QVector<qint16>& buffer)
{
    if (buffer.isEmpty()) {
        return 0.0;
    }

    // Calculate RMS value
    qreal sum = 0.0;
    for (qint16 sample : buffer) {
        sum += sample * sample;
    }

    return std::sqrt(sum / buffer.size());
}
