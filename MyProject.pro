QT += core gui widgets
QT += multimedia

# Required for C++11 features
CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    audiosystem.cpp \
    gamewindow.cpp \
    inputhandler.cpp \
    main.cpp \
    mainwindow.cpp \
    movement.cpp \
    player.cpp \
    voicechallenge.cpp

HEADERS += \
    audiosystem.h \
    gamewindow.h \
    inputhandler.h \
    mainwindow.h \
    movement.h \
    player.h \
    voicechallenge.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

# Suppress SDK version warning
CONFIG += sdk_no_version_check
