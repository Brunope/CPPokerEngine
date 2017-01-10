TEMPLATE = app
TARGET = qpoker


LIBS += poker_core/poker.a

poker_core.target = poker_core/poker.a
poker_core.commands = cd poker_core && make

QMAKE_EXTRA_TARGETS += poker_core

qpoker.depends = poker_core
