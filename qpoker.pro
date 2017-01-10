TEMPLATE = app
TARGET = qpoker

# Build and link the core game static library
LIBS += poker_core/poker.a

poker_core.target = poker_core/poker.a
poker_core.commands = cd poker_core && make

QMAKE_EXTRA_TARGETS += poker_core

qpoker.depends = poker_core

# bye compiler warnings
DEFINES += QT_DEPRECATED_WARNINGS

# copy pasted this one, dunno
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# build the ui
QT += core gui

SOURCES += poker_qt/Main.cc
