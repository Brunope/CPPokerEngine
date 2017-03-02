TARGET = qpoker
TEMPLATE = app
CONFIG -= app_bundle
QMAKE_CXX = g++
QMAKE_CXXFLAGS += -std=c++11 -g -O0
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.12

QT += core gui qml quick widgets

# bye compiler warnings
DEFINES += QT_DEPRECATED_WARNINGS

# copy pasted this one, dunno
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# link the core game static library
PRE_TARGETDEPS += "poker.a"

LIBS += "poker.a"
INCLUDEPATH += "../core/inc"

poker_core.target = "poker.a"
poker_core.commands = "cp ../core/poker.a ."
poker_core.depends = FORCE

qpoker.depends = poker_core

# do the thing
SOURCES += "Main.cc" "QEventListener.cc" "QGameDriver.cc" "QPlayer.cc" \
 "QGameView.cc" "QHumanAgent.cc"

HEADERS += "QEventListener.h" "QGameDriver.h" "QPlayer.h" "QGameView.h" \
 "QCard.h" "QHumanAgent.h"

RESOURCES += qml.qrc
QML += Main.qml Player.qml Card.qml Board.qml PlayerRing.qml ChatBox.qml \
ActionInterface.qml

QMAKE_EXTRA_TARGETS += poker_core

# do i need this?
DISTFILES += \
Main.qml Player.qml Card.qml Board.qml PlayerRing.qml ChatBox.qml \
ActionInterface.qml background.png

