TARGET = qpoker
TEMPLATE = app
CONFIG -= app_bundle
QMAKE_CXX = g++
QMAKE_CXXFLAGS += -std=c++11
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.12
QT += core gui

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
SOURCES += "Main.cc"

QMAKE_EXTRA_TARGETS += poker_core
