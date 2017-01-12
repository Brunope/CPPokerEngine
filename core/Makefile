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

RESOURCE_DIR = ./resources

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -std=c++11 -I $(INC_DIR)

# Object files
OBJS = $(addprefix $(OBJ_DIR)/, Card.o Deck.o Hand.o HandEvaluator.o \
 GameView.o Action.o HandHistory.o EventManager.o SimpleActor.o \
 HumanActor.o RandomActor.o LoggerEventListener.o HandRanks.o Game.o)
TEST_OBJS = $(addprefix $(OBJ_DIR)/, card_unittest.o deck_unittest.o \
 hand_unittest.o handevaluator_unittest.o action_unittest.o \
 gameview_unittest.o loggereventlistener_unittest.o \
 eventmanager_unittest.o handhistory_unittest.o game_unittest.o TestActor.o)

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = $(addprefix $(BIN_DIR)/, card_unittest deck_unittest hand_unittest \
 handevaluator_unittest action_unittest gameview_unittest \
 loggereventlistener_unittest eventmanager_unittest handhistory_unittest \
 game_unittest)

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
 $(GTEST_DIR)/include/gtest/internal/*.h

# House-keeping build targets.

all : setup $(OBJS) $(TESTS) $(BIN_DIR)/test_suite $(BIN_DIR)/main poker.a

HandRanksDat_Fpath = "resources/HandRanks.dat"
HandRanksDat_Url = "https://www.dropbox.com/s/iw6yuk8naanppn0/HandRanks.dat?dl=0"
HandRanksDat_Bytes = "129951336"

setup :
	if [ ! -f $(HandRanksDat_Fpath) ] || [ `wc -c < $(HandRanksDat_Fpath)` != $(HandRanksDat_Bytes) ]; then wget $(HandRanksDat_Url) -O resources/HandRanks.dat; fi
	mkdir -p bin obj

test : $(BIN_DIR)/test_suite
	./$(BIN_DIR)/test_suite


# build the static lib
poker.a : $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

clean :
	rm -rf $(TESTS) $(OBJ_DIR) $(BIN_DIR) $(TEST_OBJS) *.a *.o *.log \
 .DS_STORE


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

# HandRanks data file
HANDRANKS_DAT = $(RESOURCE_DIR)/HandRanks.dat
HandRanks_o = $(OBJ_DIR)/HandRanks.o
$(HandRanks_o) : $(SRC_DIR)/HandRanks.S $(RESOURCE_DIR)/HandRanks.dat
	$(CXX) -D__HandRanks_file_path='"$(HANDRANKS_DAT)"' \
-c $(SRC_DIR)/HandRanks.S -o $@

# HandEvaluator
HandEvaluator_o = $(OBJ_DIR)/HandEvaluator.o
HandEvaluator_deps = $(HandRanks_o) $(Card_o)
$(HandEvaluator_o) : $(SRC_DIR)/HandEvaluator.cc $(INC_DIR)/HandEvaluator.h \
 $(HandEvaluator_deps) 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/HandEvaluator.cc -o $@

# Hand actually depends on HandEvaluator, not other way around
# but the tests depend on both
Hand_o = $(OBJ_DIR)/Hand.o
Hand_deps = $(HandEvaluator_deps) $(HandEvaluator_o)
$(Hand_o) : $(Hand_deps) $(SRC_DIR)/Hand.cc $(INC_DIR)/Hand.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/Hand.cc -o $@

handevaluator_unittest_o = $(OBJ_DIR)/handevaluator_unittest.o
$(handevaluator_unittest_o) : $(Hand_o) $(GTEST_HEADERS) \
 $(TEST_DIR)/handevaluator_unittest.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
 -c $(TEST_DIR)/handevaluator_unittest.cc -o $@

$(BIN_DIR)/handevaluator_unittest : $(handevaluator_unittest_o) \
 $(Hand_deps) $(Hand_o) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

hand_unittest_o = $(OBJ_DIR)/hand_unittest.o
$(hand_unittest_o) : $(Hand_o) $(TEST_DIR)/hand_unittest.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TEST_DIR)/hand_unittest.cc -o $@

$(BIN_DIR)/hand_unittest : $(Hand_deps) $(Hand_o) $(hand_unittest_o) \
 gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# Player
PLAYER_H = $(INC_DIR)/Player.h

# Action
Action_o = $(OBJ_DIR)/Action.o
$(Action_o) : $(PLAYER_H) $(INC_DIR)/Action.h $(SRC_DIR)/Action.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/Action.cc -o $@

action_unittest_o = $(OBJ_DIR)/action_unittest.o
$(action_unittest_o) : $(Action_o) $(Card_o) \
 $(TEST_DIR)/action_unittest.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
 -c $(TEST_DIR)/action_unittest.cc -o $@

$(BIN_DIR)/action_unittest : $(action_unittest_o) $(Action_o) $(Card_o) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# HandHistory
HandHistory_o = $(OBJ_DIR)/HandHistory.o
HandHistory_deps = $(Hand_deps) $(Hand_o) $(Action_o)
$(HandHistory_o) : $(PLAYER_H) $(INC_DIR)/GameDefs.h $(HandHistory_deps) \
 $(INC_DIR)/HandHistory.h $(SRC_DIR)/HandHistory.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/HandHistory.cc -o $@

handhistory_unittest_o = $(OBJ_DIR)/handhistory_unittest.o
$(handhistory_unittest_o) : $(HandHistory_o) $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
 -c $(TEST_DIR)/handhistory_unittest.cc -o $@

$(BIN_DIR)/handhistory_unittest : $(HandHistory_o) $(HandHistory_deps) \
 gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# GameView
GameView_o = $(OBJ_DIR)/GameView.o
GameView_deps = $(HandHistory_deps) $(HandHistory_o)
$(GameView_o) : $(GameView_deps) \
 $(INC_DIR)/GameDefs.h $(INC_DIR)/GameView.h $(SRC_DIR)/GameView.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/GameView.cc -o $@

gameview_unittest_o = $(OBJ_DIR)/gameview_unittest.o
$(gameview_unittest_o) : $(GameView_o) $(GTEST_HEADERS) \
 $(TEST_DIR)/gameview_unittest.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TEST_DIR)/gameview_unittest.cc -o $@

$(BIN_DIR)/gameview_unittest : $(GameView_o) $(GameView_deps) \
 $(gameview_unittest_o) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# Actors
Actor_deps = $(GameView_deps) $(GameView_o)

# SimpleActor
SimpleActor_o = $(OBJ_DIR)/SimpleActor.o
$(SimpleActor_o) : $(PLAYER_H) $(Actor_deps) $(INC_DIR)/SimpleActor.h \
 $(SRC_DIR)/SimpleActor.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
 -c $(SRC_DIR)/SimpleActor.cc -o $@

# TestActor
TestActor_o = $(OBJ_DIR)/TestActor.o
$(TestActor_o) : $(PLAYER_H) $(Actor_deps) $(INC_DIR)/TestActor.h \
 $(SRC_DIR)/TestActor.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/TestActor.cc -o $@

# RandomActor
RandomActor_o = $(OBJ_DIR)/RandomActor.o
RandomActor_deps = RandomActor Action Card GameView HandHistory
$(RandomActor_o) : $(addprefix $(INC_DIR)/, \
 $(addsuffix .h, $(RandomActor_deps) Player)) $(SRC_DIR)/RandomActor.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/RandomActor.cc -o $@

# HumanActor
HumanActor_o = $(OBJ_DIR)/HumanActor.o
HumanActor_deps = HumanActor Action Card GameView HandHistory
$(HumanActor_o) : $(addprefix $(INC_DIR)/, \
 $(addsuffix .h, $(HumanActor_deps) Player)) $(SRC_DIR)/HumanActor.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/HumanActor.cc -o $@


# Event listeners
EventListener_deps = $(GameView_deps) $(GameView_o)

# LoggerEventListener
LoggerEventListener_o = $(OBJ_DIR)/LoggerEventListener.o
$(LoggerEventListener_o) : $(PLAYER_H) $(EventListener_deps) \
 $(SRC_DIR)/LoggerEventListener.cc $(INC_DIR)/IEventListener.h \
 $(INC_DIR)/LoggerEventListener.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
 -c $(SRC_DIR)/LoggerEventListener.cc -o $@

loggereventlistener_unittest_o = $(OBJ_DIR)/loggereventlistener_unittest.o
$(loggereventlistener_unittest_o) : $(LoggerEventListener_o) \
 $(GTEST_HEADERS) $(TEST_DIR)/loggereventlistener_unittest.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
 -c $(TEST_DIR)/loggereventlistener_unittest.cc -o $@

$(BIN_DIR)/loggereventlistener_unittest : $(loggereventlistener_unittest_o) \
 $(LoggerEventListener_o) $(EventListener_deps) $(GameView_o) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# EventManager - its tests need LoggerEventListener (or a mock class)
# to verify anything.
EventManager_o = $(OBJ_DIR)/EventManager.o
$(EventManager_o) : $(INC_DIR)/EventManager.h $(SRC_DIR)/EventManager.cc \
 $(INC_DIR)/Action.h $(INC_DIR)/GameView.h $(INC_DIR)/HandHistory.h \
 $(PLAYER_H) $(Hand_o) $(INC_DIR)/IEventListener.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/EventManager.cc -o $@

eventmanager_unittest_o = $(OBJ_DIR)/eventmanager_unittest.o
$(eventmanager_unittest_o) : $(EventManager_o) $(LoggerEventListener_o) \
 $(PLAYER_H) $(Action_o) $(Hand_o) $(GameView_o) $(HandEvaluator_o) \
 $(TEST_DIR)/eventmanager_unittest.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
 -c $(TEST_DIR)/eventmanager_unittest.cc -o $@

$(BIN_DIR)/eventmanager_unittest : $(eventmanager_unittest_o) \
 $(EventManager_o) $(EventListener_deps) $(LoggerEventListener_o) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# Game
Game_o = $(OBJ_DIR)/Game.o
$(Game_o) : $(Card_o) $(Hand_o) $(HandEvaluator_o) $(Deck_o) $(PLAYER_H) \
 $(Action_o) $(GameView_o) $(INC_DIR)/Actor.h $(INC_DIR)/Observer.h \
 $(EventManager_o) $(HandHistory_o) $(INC_DIR)/GameDefs.h \
 $(INC_DIR)/Game.h $(INC_DIR)/log.h $(SRC_DIR)/Game.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/Game.cc -o $@

game_unittest_o = $(OBJ_DIR)/game_unittest.o
$(game_unittest_o) : $(Game_o) $(INC_DIR)/TestActor.h $(GTEST_HEADERS) \
 $(TEST_DIR)/game_unittest.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TEST_DIR)/game_unittest.cc -o $@

$(BIN_DIR)/game_unittest : $(Card_o) $(Hand_o) $(HandEvaluator_o) $(Deck_o) \
 $(Action_o) $(GameView_o) $(EventManager_o) $(HandHistory_o) $(Game_o) \
 $(SimpleActor_o) $(Actor_o) $(game_unittest_o) $(TestActor_o) $(HandRanks_o) \
 $(RandomActor_o) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# Main
Main_o = $(OBJ_DIR)/Main.o
$(Main_o) : $(Game_o) $(HumanActor_o) $(RandomActor_o) $(SRC_DIR)/Main.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/Main.cc -o $@

$(BIN_DIR)/main : $(Card_o) $(Hand_o) $(HandEvaluator_o) $(Deck_o) \
 $(Action_o) $(GameView_o) $(EventManager_o) $(HandHistory_o) $(Game_o) \
 $(Actor_o) $(HumanActor_o) $(RandomActor_o) $(HumanActor_o) $(HandRanks_o) \
 $(LoggerEventListener_o) $(Main_o)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@
