CXX = clang++

SHELL = /bin/sh

# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR = ./gtest

# Where to find my code.
SRC_DIR = ./src
INC_DIR = ./inc

# Where to find my tests
TEST_DIR = ./test

# Hide all the .o and .a files here except I haven't configured it yet
OBJ_DIR = ./obj

# Put all the binaries here
BIN_DIR = ./bin

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -std=c++11 -I $(INC_DIR)

# Object files
OBJS = $(addprefix $(OBJ_DIR)/, Card.o Deck.o Hand.o HandEvaluator.o \
 GameView.o Action.o EventManager.o SimpleActor.o LoggerEventListener.o \
 Game.o)
TEST_OBJS = $(addprefix $(OBJ_DIR)/, card_unittest.o deck_unittest.o \
 hand_unittest.o handevaluator_unittest.o action_unittest.o \
 gameview_unittest.o loggereventlistener_unittest.o \
 eventmanager_unittest.o game_unittest.o TestActor.o)

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = $(addprefix $(BIN_DIR)/, card_unittest deck_unittest hand_unittest \
 handevaluator_unittest action_unittest gameview_unittest \
 loggereventlistener_unittest eventmanager_unittest game_unittest)

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
 $(GTEST_DIR)/include/gtest/internal/*.h

# House-keeping build targets.

all : setup $(OBJS) $(TESTS) $(BIN_DIR)/test_suite

setup :
	if [ `wc -c < resources/HandRanks.dat` != "129951336" ]; then wget "https://github.com/Brunope/CPPokerEngine/blob/master/resources/HandRanks.dat?raw=true" -O resources/HandRanks.dat; fi
	mkdir -p bin obj

test : $(BIN_DIR)/test_suite
	./$(BIN_DIR)/test_suite

clean :
	rm -rf $(TESTS) $(OBJS) $(TEST_OBJS) gtest.a gtest_main.a \
 gtest-all.o gtest_main.o $(BIN_DIR)/test_suite *.dSYM .DS_STORE

$(BIN_DIR)/test_suite : gtest_main.o gtest_main.a $(TEST_OBJS) $(OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
$(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
$(GTEST_DIR)/src/gtest_main.cc

# does anything even use this?
gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Build Card and tests
Card_o = $(OBJ_DIR)/Card.o
$(Card_o) : $(SRC_DIR)/Card.cc $(INC_DIR)/Card.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/Card.cc -o $@

card_unittest_o = $(OBJ_DIR)/card_unittest.o
$(card_unittest_o) : $(Card_o) $(TEST_DIR)/card_unittest.cc \
 $(INC_DIR)/Card.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TEST_DIR)/card_unittest.cc -o $@

$(BIN_DIR)/card_unittest : $(Card_o) $(card_unittest_o) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# Build Deck and tests
Deck_o = $(OBJ_DIR)/Deck.o
$(Deck_o) : $(Card_o) $(SRC_DIR)/Deck.cc $(INC_DIR)/Deck.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/Deck.cc -o $@

deck_unittest_o = $(OBJ_DIR)/deck_unittest.o
$(deck_unittest_o) : $(Deck_o) $(TEST_DIR)/deck_unittest.cc \
$(INC_DIR)/Deck.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TEST_DIR)/deck_unittest.cc -o $@

$(BIN_DIR)/deck_unittest : $(Deck_o) $(Card_o) $(deck_unittest_o) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# HandEvaluator
HandEvaluator_o = $(OBJ_DIR)/HandEvaluator.o
$(HandEvaluator_o) : $(SRC_DIR)/HandEvaluator.cc $(INC_DIR)/HandEvaluator.h \
	$(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/HandEvaluator.cc -o $@

# Hand actually depends on HandEvaluator, not other way around
# but the tests depend on both
Hand_deps = HandEvaluator Card
Hand_o = $(OBJ_DIR)/Hand.o
$(Hand_o) : $(Card_o) $(HandEvaluator_o) $(SRC_DIR)/Hand.cc $(INC_DIR)/Hand.h \
 $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/Hand.cc -o $@

handevaluator_unittest_o = $(OBJ_DIR)/handevaluator_unittest.o
$(handevaluator_unittest_o) : $(HandEvaluator_o) $(Hand_o) \
 $(TEST_DIR)/handevaluator_unittest.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
 -c $(TEST_DIR)/handevaluator_unittest.cc -o $@

$(BIN_DIR)/handevaluator_unittest : $(HandEvaluator_o) \
 $(handevaluator_unittest_o) $(Hand_o) $(Card_o) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

hand_unittest_o = $(OBJ_DIR)/hand_unittest.o
$(hand_unittest_o) : $(Hand_o) $(TEST_DIR)/hand_unittest.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TEST_DIR)/hand_unittest.cc -o $@

$(BIN_DIR)/hand_unittest : $(Hand_o) $(Card_o) $(HandEvaluator_o) \
 $(hand_unittest_o) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# Player
PLAYER_H = $(INC_DIR)/Player.h

# Action
Action_o = $(OBJ_DIR)/Action.o
$(Action_o) : $(INC_DIR)/Action.h $(SRC_DIR)/Action.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/Action.cc -o $@

action_unittest_o = $(OBJ_DIR)/action_unittest.o
$(action_unittest_o) : $(Action_o) $(PLAYER_H) $(Card_o) \
 $(TEST_DIR)/action_unittest.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
 -c $(TEST_DIR)/action_unittest.cc -o $@

$(BIN_DIR)/action_unittest : $(action_unittest_o) $(Action_o) $(Card_o) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# GameView
GameView_o = $(OBJ_DIR)/GameView.o
$(GameView_o) : $(Card_o) $(Hand_o) $(PLAYER_H) $(Action_o) $(Actor_o) \
 $(INC_DIR)/GameDefs.h $(INC_DIR)/GameView.h $(SRC_DIR)/GameView.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/GameView.cc -o $@

gameview_unittest_o = $(OBJ_DIR)/gameview_unittest.o
$(gameview_unittest_o) : $(GameView_o) $(GTEST_HEADERS) \
 $(TEST_DIR)/gameview_unittest.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TEST_DIR)/gameview_unittest.cc -o $@

$(BIN_DIR)/gameview_unittest : $(GameView_o) $(Card_o) $(gameview_unittest_o) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# SimpleActor
SimpleActor_o = $(OBJ_DIR)/SimpleActor.o
$(SimpleActor_o) : $(PLAYER_H) $(Action_o) $(HandEvaluator_o) $(Hand_o) \
 $(Card_o) $(GameView_o) $(INC_DIR)/SimpleActor.h $(SRC_DIR)/SimpleActor.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
 -c $(SRC_DIR)/SimpleActor.cc -o $@

# TestActor
TestActor_o = $(OBJ_DIR)/TestActor.o
TestActor_deps = TestActor Action HandEvaluator Card GameView
$(TestActor_o) : $(addprefix $(INC_DIR)/, $(addsuffix .h, $(TestActor_deps) Player)) \
 $(SRC_DIR)/TestActor.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/TestActor.cc -o $@

# LoggerEventListener
LoggerEventListener_o = $(OBJ_DIR)/LoggerEventListener.o
$(LoggerEventListener_o) : $(PLAYER_H) $(Action_o) $(Hand_o) $(GameView_o) \
 $(HandEvaluator_o) $(SRC_DIR)/LoggerEventListener.cc \
 $(INC_DIR)/IEventListener.h $(INC_DIR)/LoggerEventListener.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
 -c $(SRC_DIR)/LoggerEventListener.cc -o $@

loggereventlistener_unittest_o = $(OBJ_DIR)/loggereventlistener_unittest.o
$(loggereventlistener_unittest_o) : $(LoggerEventListener_o) \
 $(GameView_o) $(Hand_o) $(HandEvaluator_o) $(Card_o) $(Action_o) \
 $(GTEST_HEADERS) $(TEST_DIR)/loggereventlistener_unittest.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
 -c $(TEST_DIR)/loggereventlistener_unittest.cc -o $@

$(BIN_DIR)/loggereventlistener_unittest : $(loggereventlistener_unittest_o) \
 $(LoggerEventListener_o) $(Action_o) $(Hand_o) $(HandEvaluator_o) \
 $(Card_o) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# EventManager - its tests need LoggerEventListener (or a mock class)
# to verify anything.
EventManager_o = $(OBJ_DIR)/EventManager.o
$(EventManager_o) : $(INC_DIR)/EventManager.h $(SRC_DIR)/EventManager.cc \
 $(INC_DIR)/Action.h $(PLAYER_H) $(Hand_o) $(INC_DIR)/IEventListener.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/EventManager.cc -o $@

eventmanager_unittest_o = $(OBJ_DIR)/eventmanager_unittest.o
$(eventmanager_unittest_o) : $(EventManager_o) $(LoggerEventListener_o) \
 $(PLAYER_H) $(Action_o) $(Hand_o) $(GameView_o) $(HandEvaluator_o) \
 $(TEST_DIR)/eventmanager_unittest.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
 -c $(TEST_DIR)/eventmanager_unittest.cc -o $@

$(BIN_DIR)/eventmanager_unittest : $(eventmanager_unittest_o) \
 $(EventManager_o) $(Action_o) $(Card_o) $(Hand_o) $(HandEvaluator_o) \
 $(GameView_o) $(LoggerEventListener_o) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# Game
Game_o = $(OBJ_DIR)/Game.o
$(Game_o) : $(Card_o) $(Hand_o) $(HandEvaluator_o) $(Deck_o) $(PLAYER_H) \
 $(Action_o) $(GameView_o) $(INC_DIR)/Actor.h $(INC_DIR)/Observer.h \
 $(EventManager_o) $(INC_DIR)/GameDefs.h $(INC_DIR)/Game.h $(INC_DIR)/log.h \
 $(SRC_DIR)/Game.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/Game.cc -o $@

game_unittest_o = $(OBJ_DIR)/game_unittest.o
$(game_unittest_o) : $(Game_o) $(INC_DIR)/TestActor.h $(GTEST_HEADERS) \
 $(TEST_DIR)/game_unittest.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TEST_DIR)/game_unittest.cc -o $@

$(BIN_DIR)/game_unittest : $(Card_o) $(Hand_o) $(HandEvaluator_o) $(Deck_o) \
 $(Action_o) $(GameView_o) $(EventManager_o) $(Game_o) $(SimpleActor_o) \
 $(Actor_o) $(game_unittest_o) $(TestActor_o) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
