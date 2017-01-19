TARGET = qpoker
TEMPLATE = app
CONFIG -= app_bundle
QMAKE_CXX = clang++
QMAKE_CXXFLAGS += -std=c++11 -g
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
SOURCES += "Main.cc" "QtEventListener.cc" "QtGameDriver.cc"

HEADERS += "QtEventListener.h" "QtGameDriver.h"

QMAKE_EXTRA_TARGETS += poker_core

DISTFILES += \
    Gui.qml

QML += Gui.qml
