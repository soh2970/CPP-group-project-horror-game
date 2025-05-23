#include "voicechallenge.h"
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QDir>
#include <QRandomGenerator>
#include <QBrush>
#include <QPen>
#include <QTimer>
#include <QFont>
#include <QApplication>
#include <QGraphicsProxyWidget>

VoiceChallenge::VoiceChallenge(QGraphicsScene *scene, Player *player, QObject *parent)
    : QObject(parent),
    m_scene(scene),
    m_player(player),
    m_jumpscareFolder(":/jumpscares"),
    m_challengeInterval(30000),
    m_challengeTime(10000),
    m_inputField(nullptr),
    m_inputFieldProxy(nullptr),
    m_submitButton(nullptr),
    m_buttonProxy(nullptr),
    m_challengeActive(false)
{
    // Initialize timers
    m_challengeIntervalTimer = new QTimer(this);
    m_challengeTimer = new QTimer(this);
    m_countdownTimer = new QTimer(this);
    m_jumpscareTimer = new QTimer(this);
    m_successCheckTimer = new QTimer(this);

    // Connect signals
    connect(m_challengeIntervalTimer, &QTimer::timeout, this, &VoiceChallenge::showChallenge);
    connect(m_challengeTimer, &QTimer::timeout, this, &VoiceChallenge::onChallengeTimeout);
    connect(m_countdownTimer, &QTimer::timeout, this, &VoiceChallenge::updateCountdown);
    connect(m_jumpscareTimer, &QTimer::timeout, this, &VoiceChallenge::hideJumpscare);
    connect(m_successCheckTimer, &QTimer::timeout, this, &VoiceChallenge::hideSuccessCheck);

    // Initialize challenge phrases
    m_challengePhrases << "she sells seashells by the seashore"
                       << "peter piper picked a peck of peppers"
                       << "red lorry yellow lorry red lorry"
                       << "black bug bit a big black bear"
                       << "fred fed ted bread and ted fed fred"
                       << "how can a clam cram in a clean can"
                       << "six slippery snails slid slowly seaward"
                       << "truly rural truly rural truly rural"
                       << "brisk brave brigadiers brandish blades"
                       << "four fine fresh fish for you";


    // Create UI elements
    createChallengeUI();

    // Debug message to show initialization
    qDebug() << "Text challenge system initialized with jumpscare path:" << m_jumpscareFolder;
}

VoiceChallenge::~VoiceChallenge()
{
    stop();

    // Remove UI elements from scene
    if (m_overlay && m_scene) {
        m_scene->removeItem(m_overlay);
        delete m_overlay;
    }

    if (m_challengeText && m_scene) {
        m_scene->removeItem(m_challengeText);
        delete m_challengeText;
    }

    if (m_countdownText && m_scene) {
        m_scene->removeItem(m_countdownText);
        delete m_countdownText;
    }

    if (m_jumpscareImage && m_scene) {
        m_scene->removeItem(m_jumpscareImage);
        delete m_jumpscareImage;
    }

    if (m_successCheck && m_scene) {
        m_scene->removeItem(m_successCheck);
        delete m_successCheck;
    }

    if (m_inputFieldProxy && m_scene) {
        m_scene->removeItem(m_inputFieldProxy);
        // m_inputField will be deleted with its proxy
    }

    if (m_buttonProxy && m_scene) {
        m_scene->removeItem(m_buttonProxy);
        // m_submitButton will be deleted with its proxy
    }
}

void VoiceChallenge::start()
{
    // Start the interval timer to trigger challenges
    m_challengeIntervalTimer->start(m_challengeInterval);
    qDebug() << "Text challenge system started. First challenge in" << m_challengeInterval / 1000 << "seconds";
}

void VoiceChallenge::stop()
{
    // Stop all timers
    m_challengeIntervalTimer->stop();
    m_challengeTimer->stop();
    m_countdownTimer->stop();
    m_jumpscareTimer->stop();
    m_successCheckTimer->stop();

    // Hide UI elements
    if (m_overlay) m_overlay->setVisible(false);
    if (m_challengeText) m_challengeText->setVisible(false);
    if (m_countdownText) m_countdownText->setVisible(false);
    if (m_jumpscareImage) m_jumpscareImage->setVisible(false);
    if (m_successCheck) m_successCheck->setVisible(false);
    if (m_inputFieldProxy) m_inputFieldProxy->setVisible(false);
    if (m_buttonProxy) m_buttonProxy->setVisible(false);

    m_challengeActive = false;
    qDebug() << "Text challenge system stopped";
}

void VoiceChallenge::setJumpscareFolder(const QString &path)
{
    m_jumpscareFolder = path;
    qDebug() << "Jumpscare folder set to:" << path;
}

void VoiceChallenge::setChallengeInterval(int ms)
{
    m_challengeInterval = ms;
    if (m_challengeIntervalTimer->isActive()) {
        m_challengeIntervalTimer->setInterval(ms);
    }
    qDebug() << "Challenge interval set to:" << ms / 1000 << "seconds";
}

void VoiceChallenge::setChallengeTime(int ms)
{
    m_challengeTime = ms;
    qDebug() << "Challenge time set to:" << ms / 1000 << "seconds";
}

void VoiceChallenge::showChallenge()
{
    if (m_challengeActive) {
        return; // Don't show a new challenge if one is already active
    }

    // Get a random challenge phrase
    m_currentChallenge = getRandomChallenge();

    // Set the challenge text
    m_challengeText->setPlainText(m_currentChallenge);

    // Make sure overlay is in the right position and size
    QRectF sceneRect = m_scene->sceneRect();
    m_overlay->setRect(sceneRect);

    // Calculate center position for challenge text
    QRectF textRect = m_challengeText->boundingRect();
    qreal centerX = (sceneRect.width() - textRect.width()) / 2;
    m_challengeText->setPos(centerX, sceneRect.height() / 2 - 50);

    // Make UI elements visible
    m_overlay->setVisible(true);
    m_challengeText->setVisible(true);
    m_countdownText->setVisible(true);

    // Show and reset input field
    m_inputField->clear();
    m_inputField->setPlaceholderText("Type the phrase...");
    m_inputFieldProxy->setVisible(true);
    m_buttonProxy->setVisible(true);

    // Set focus to input field
    m_inputField->setFocus();

    // Start challenge timer
    m_challengeTimer->start(m_challengeTime);
    m_countdownTimer->start(1000); // Update countdown every second
    m_elapsedTimer.start();

    m_challengeActive = true;
    qDebug() << "Text challenge started: \"" << m_currentChallenge << "\"";
}

void VoiceChallenge::onChallengeTimeout()
{
    // Challenge failed - show jumpscare
    qDebug() << "Challenge timeout! Showing jumpscare...";
    showJumpscare();

    // Decrease player health by 20%
    if (m_player) {
        int maxHealth = 100; // Assuming max health is 100, adjust if different
        int damage = maxHealth * 0.2; // 20% of max health
        m_player->decreaseHealth(damage);
        qDebug() << "Player health decreased by" << damage << "points";
    }

    // Rest of method remains the same
    m_challengeTimer->stop();
    m_countdownTimer->stop();

    // Hide challenge UI
    m_overlay->setVisible(false);
    m_challengeText->setVisible(false);
    m_countdownText->setVisible(false);
    m_inputFieldProxy->setVisible(false);
    m_buttonProxy->setVisible(false);

    m_challengeActive = false;

    // Restart the interval timer for the next challenge
    m_challengeIntervalTimer->start(m_challengeInterval);
}

void VoiceChallenge::hideJumpscare()
{
    if (m_jumpscareImage) {
        m_jumpscareImage->setVisible(false);
    }
}

void VoiceChallenge::hideSuccessCheck()
{
    if (m_successCheck) {
        m_successCheck->setVisible(false);
    }
}

void VoiceChallenge::createChallengeUI()
{
    if (!m_scene) return;

    // Create a semi-transparent black overlay
    QRectF sceneRect = m_scene->sceneRect();
    m_overlay = new QGraphicsRectItem(sceneRect);
    m_overlay->setBrush(QBrush(QColor(0, 0, 0, 200))); // Semi-transparent black
    m_overlay->setPen(Qt::NoPen);
    m_overlay->setZValue(10); // Ensure it's above everything else
    m_overlay->setVisible(false);
    m_scene->addItem(m_overlay);

    // Create challenge text
    m_challengeText = new QGraphicsTextItem();
    m_challengeText->setDefaultTextColor(Qt::white);
    QFont challengeFont = QApplication::font();
    challengeFont.setPointSize(24);
    challengeFont.setBold(true);
    m_challengeText->setFont(challengeFont);
    // Position will be set dynamically in showChallenge()
    m_challengeText->setZValue(11);
    m_challengeText->setVisible(false);
    m_scene->addItem(m_challengeText);

    // Create countdown text
    m_countdownText = new QGraphicsTextItem();
    m_countdownText->setDefaultTextColor(Qt::white);
    QFont countdownFont = QApplication::font();
    countdownFont.setPointSize(24);
    countdownFont.setBold(true);
    m_countdownText->setFont(countdownFont);
    // Position will be set dynamically in updateCountdown()
    m_countdownText->setZValue(11);
    m_countdownText->setVisible(false);
    m_scene->addItem(m_countdownText);

    // Create jumpscare image placeholder
    m_jumpscareImage = new QGraphicsPixmapItem();
    m_jumpscareImage->setZValue(12); // Above everything
    m_jumpscareImage->setVisible(false);
    m_scene->addItem(m_jumpscareImage);

    // Create text input field
    m_inputField = new QLineEdit();
    m_inputField->setFixedWidth(300);
    m_inputField->setFixedHeight(30);
    m_inputField->setPlaceholderText("Type the phrase...");
    m_inputFieldProxy = m_scene->addWidget(m_inputField);
    m_inputFieldProxy->setPos((sceneRect.width() - 300) / 2, sceneRect.height() / 2 + 80);
    m_inputFieldProxy->setZValue(11);
    m_inputFieldProxy->setVisible(false);

    // Create submit button
    m_submitButton = new QPushButton("Done");
    m_submitButton->setFixedWidth(100);
    m_submitButton->setFixedHeight(30);
    m_buttonProxy = m_scene->addWidget(m_submitButton);
    m_buttonProxy->setPos((sceneRect.width() - 100) / 2, sceneRect.height() / 2 + 120);
    m_buttonProxy->setZValue(11);
    m_buttonProxy->setVisible(false);

    // Connect button click to check input
    connect(m_submitButton, &QPushButton::clicked, this, [this]() {
        checkTextInput(m_inputField->text());
    });

    // Also connect Return/Enter key in input field
    connect(m_inputField, &QLineEdit::returnPressed, this, [this]() {
        checkTextInput(m_inputField->text());
    });

    // Create success checkmark
    m_successCheck = new QGraphicsPixmapItem();
    // Create a green checkmark (you could replace this with an image)
    QPixmap checkmark(100, 100);
    checkmark.fill(Qt::transparent);
    QPainter painter(&checkmark);
    painter.setPen(QPen(Qt::green, 10));
    painter.drawLine(20, 50, 40, 80);
    painter.drawLine(40, 80, 80, 30);
    painter.end();

    m_successCheck->setPixmap(checkmark);
    m_successCheck->setPos(sceneRect.width()/2 - 50, sceneRect.height()/2 - 50);
    m_successCheck->setZValue(12);
    m_successCheck->setVisible(false);
    m_scene->addItem(m_successCheck);
}

void VoiceChallenge::updateCountdown()
{
    if (!m_challengeActive) return;

    // Calculate remaining time
    int elapsed = m_elapsedTimer.elapsed();
    int remaining = (m_challengeTime - elapsed) / 1000;

    if (remaining < 0) remaining = 0;

    // Update the countdown text
    m_countdownText->setPlainText(QString::number(remaining));

    // Calculate center position for countdown text
    QRectF sceneRect = m_scene->sceneRect();
    QRectF textRect = m_countdownText->boundingRect();
    qreal centerX = (sceneRect.width() - textRect.width()) / 2;
    m_countdownText->setPos(centerX, sceneRect.height() / 2 + 30);
}

void VoiceChallenge::showJumpscare()
{
    QString imagePath = getRandomJumpscareImage();

    qDebug() << "Attempting to show jumpscare image:" << imagePath;

    if (!imagePath.isEmpty()) {
        QPixmap jumpscare(imagePath);

        if (!jumpscare.isNull()) {
            // Scale to fit the screen if needed
            QRectF sceneRect = m_scene->sceneRect();
            if (jumpscare.width() > sceneRect.width() || jumpscare.height() > sceneRect.height()) {
                jumpscare = jumpscare.scaled(sceneRect.width(), sceneRect.height(),
                                             Qt::KeepAspectRatio, Qt::SmoothTransformation);
            }

            // Center the jumpscare
            m_jumpscareImage->setPixmap(jumpscare);
            m_jumpscareImage->setPos((sceneRect.width() - jumpscare.width())/2,
                                     (sceneRect.height() - jumpscare.height())/2);
            m_jumpscareImage->setVisible(true);

            // Set timer to hide jumpscare after 3 seconds
            m_jumpscareTimer->start(3000);

            qDebug() << "Showing jumpscare:" << imagePath << ", Size:" << jumpscare.size();
        } else {
            qWarning() << "Failed to load jumpscare image:" << imagePath;
            // Try alternative method to load the image
            tryAlternativeImageLoad(imagePath);
        }
    } else {
        qWarning() << "No jumpscare images found!";
    }
}

void VoiceChallenge::tryAlternativeImageLoad(const QString& path)
{
    // First, try from resources
    QString resourcePath = path;
    if (!resourcePath.startsWith(":/")) {
        resourcePath = ":/jumpscares/" + QFileInfo(path).fileName();
    }

    QPixmap jumpscare(resourcePath);
    if (!jumpscare.isNull()) {
        QRectF sceneRect = m_scene->sceneRect();
        m_jumpscareImage->setPixmap(jumpscare);
        m_jumpscareImage->setPos((sceneRect.width() - jumpscare.width())/2,
                                 (sceneRect.height() - jumpscare.height())/2);
        m_jumpscareImage->setVisible(true);
        m_jumpscareTimer->start(3000);
        qDebug() << "Loaded jumpscare using alternative method:" << resourcePath;
    } else {
        // If still not working, try a hardcoded default image
        qDebug() << "Trying to show default jumpscare image...";
        QPixmap defaultJumpscare(":/jumpscares/image1.jpg");
        if (!defaultJumpscare.isNull()) {
            QRectF sceneRect = m_scene->sceneRect();
            m_jumpscareImage->setPixmap(defaultJumpscare);
            m_jumpscareImage->setPos((sceneRect.width() - defaultJumpscare.width())/2,
                                     (sceneRect.height() - defaultJumpscare.height())/2);
            m_jumpscareImage->setVisible(true);
            m_jumpscareTimer->start(3000);
            qDebug() << "Loaded default jumpscare image";
        } else {
            qWarning() << "Failed to load ANY jumpscare image!";
        }
    }
}

void VoiceChallenge::showSuccessCheck()
{
    if (m_successCheck) {
        QRectF sceneRect = m_scene->sceneRect();
        m_successCheck->setPos((sceneRect.width() - m_successCheck->boundingRect().width())/2,
                               (sceneRect.height() - m_successCheck->boundingRect().height())/2);
        m_successCheck->setVisible(true);

        // Hide after 1 second
        m_successCheckTimer->start(1000);
    }
}

QString VoiceChallenge::getRandomChallenge()
{
    if (m_challengePhrases.isEmpty()) {
        return "please type this phrase"; // Default if no phrases available
    }

    // Pick a random phrase
    int index = QRandomGenerator::global()->bounded(m_challengePhrases.size());
    return m_challengePhrases.at(index);
}

QString VoiceChallenge::getRandomJumpscareImage()
{
    // Create a list of images
    QStringList images;

    // Try resource path first
    for (int i = 1; i <= 12; i++) {
        QString path = QString(":/jumpscares/image%1.jpg").arg(i);
        if (QFile::exists(path)) {
            images << path;
        }
    }

    // If no resource images found, try file system
    if (images.isEmpty()) {
        QDir directory(m_jumpscareFolder);
        QStringList fileImages = directory.entryList(QStringList() << "*.png" << "*.jpg" << "*.jpeg", QDir::Files);

        if (fileImages.isEmpty()) {
            qWarning() << "No images found in jumpscare folder:" << m_jumpscareFolder;
            return QString();
        }

        // Convert to full paths
        for (const QString& img : fileImages) {
            images << directory.absoluteFilePath(img);
        }
    }

    if (images.isEmpty()) {
        return QString();
    }

    // Use QRandomGenerator to select a random image
    int index = QRandomGenerator::global()->bounded(images.size());
    return images.at(index);
}

void VoiceChallenge::checkTextInput(const QString& input)
{
    if (!m_challengeActive)
        return;

    qDebug() << "Checking input: \"" << input << "\" against challenge: \"" << m_currentChallenge << "\"";

    // Compare input with challenge (case insensitive)
    if (input.toLower().trimmed() == m_currentChallenge.toLower().trimmed()) {
        // Input matches - success!
        qDebug() << "Text challenge completed successfully!";

        // Show success checkmark
        showSuccessCheck();

        // Stop timers
        m_challengeTimer->stop();
        m_countdownTimer->stop();

        // Hide challenge UI
        m_overlay->setVisible(false);
        m_challengeText->setVisible(false);
        m_countdownText->setVisible(false);
        m_inputFieldProxy->setVisible(false);
        m_buttonProxy->setVisible(false);

        m_challengeActive = false;

        // Restart the interval timer for the next challenge
        m_challengeIntervalTimer->start(m_challengeInterval);
    } else {
        // Input doesn't match - provide feedback
        m_inputField->clear();
        m_inputField->setPlaceholderText("Incorrect - try again!");
    }
}
