CXX = g++-6

SHELL = /bin/sh

# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR = ./gtest

# Where to find my code.
SRC_DIR = ./src

# Where to find my tests
TEST_DIR = ./test

# Hide all the .o and .a files here except I haven't configured it yet
OBJ_DIR = ./obj

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -std=c++11 -I $(SRC_DIR)

# Object files
OBJS = Card.o Deck.o Hand.o HandEvaluator.o GameView.o SimpleActor.o \
       Game.o
TEST_OBJS = card_unittest.o deck_unittest.o hand_unittest.o \
	    handevaluator_unittest.o action_unittest.o gameview_unittest.o

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = card_unittest deck_unittest hand_unittest handevaluator_unittest \
	action_unittest gameview_unittest

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# House-keeping build targets.

all : $(OBJS) $(TEST_OBJS) test_suite

test : test_suite
	./test_suite

clean :
	rm -rf $(TESTS) gtest.a gtest_main.a *.o test_suite *.dSYM .DS_STORE

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

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Build the test suite
test_suite : gtest_main.o gtest_main.a $(TESTS) $(OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread gtest_main.o gtest_main.a \
-o test_suite $(OBJS) $(TEST_OBJS)

# Build Card and tests
Card.o : $(SRC_DIR)/Card.cc $(SRC_DIR)/Card.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/Card.cc

card_unittest.o : Card.o $(TEST_DIR)/card_unittest.cc \
$(SRC_DIR)/Card.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I $(SRC_DIR) \
-c $(TEST_DIR)/card_unittest.cc

card_unittest : Card.o card_unittest.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# Build Deck and tests
Deck.o : Card.o $(SRC_DIR)/Deck.cc $(SRC_DIR)/Deck.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/Deck.cc

deck_unittest.o : Deck.o $(TEST_DIR)/deck_unittest.cc \
$(SRC_DIR)/Deck.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I $(SRC_DIR) \
-c $(TEST_DIR)/deck_unittest.cc

deck_unittest : Deck.o Card.o deck_unittest.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# Hand
Hand.o : Card.o HandEvaluator.o $(SRC_DIR)/Hand.cc $(SRC_DIR)/Hand.h \
 $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/Hand.cc

hand_unittest.o : Hand.o $(TEST_DIR)/hand_unittest.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I $(SRC_DIR) \
-c $(TEST_DIR)/hand_unittest.cc

hand_unittest : Hand.o Card.o HandEvaluator.o hand_unittest.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# HandEvaluator
HandEvaluator.o : $(SRC_DIR)/HandEvaluator.cc $(SRC_DIR)/HandEvaluator.h \
	$(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/HandEvaluator.cc

handevaluator_unittest.o : HandEvaluator.o \
	$(TEST_DIR)/handevaluator_unittest.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I $(SRC_DIR) -c \
	$(TEST_DIR)/handevaluator_unittest.cc

handevaluator_unittest : HandEvaluator.o handevaluator_unittest.o Hand.o \
	Card.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# Player
PLAYER_H = $(SRC_DIR)/Player.h

# Actions
ACTION_H = $(SRC_DIR)/actions/$(wildcard *.h) $(SRC_DIR)/Actions.h

action_unittest.o : $(ACTION_H) $(PLAYER_H) \
 $(TEST_DIR)/action_unittest.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
	-c $(TEST_DIR)/action_unittest.cc

action_unittest : action_unittest.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# GameView
GameView.o : Card.o Hand.o $(PLAYER_H) $(ACTION_H) $(SRC_DIR)/GameView.h \
 $(SRC_DIR)/GameView.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/GameView.cc

gameview_unittest.o : GameView.o $(GTEST_HEADERS) \
 $(TEST_DIR)/gameview_unittest.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TEST_DIR)/gameview_unittest.cc

gameview_unittest : GameView.o gameview_unittest.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# SimpleActor
SimpleActor.o : $(PLAYER_H) $(ACTION_H) HandEvaluator.o Hand.o Card.o GameView.o \
 $(SRC_DIR)/SimpleActor.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
 -c $(SRC_DIR)/SimpleActor.cc

Game.o : Card.o Hand.o HandEvaluator.o Deck.o $(PLAYER_H) $(ACTION_H) \
 GameView.o $(SRC_DIR)/Actor.h $(SRC_DIR)/Observer.h $(SRC_DIR)/Game.h \
 $(SRC_DIR)/Game.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/Game.cc


