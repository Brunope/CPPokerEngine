CXX = clang++
AR = ar

BIN_DIR = ./bin
OBJ_DIR = ./obj
CXXFLAGS = -I core/inc -I agents/inc -g -std=c++11

.PHONY : subsystem test clean core agents ui

subsystem : | core agents ui


test :
	cd test && $(MAKE)
	bin/test_suite

clean :
	cd core && $(MAKE) clean
	cd ui && $(MAKE) clean
	cd agents && $(MAKE) clean
	cd test && $(MAKE) clean
	rm -f $(BIN_DIR)/console_main
	rm -rf $(BIN_DIR)/console_main.dSYM

core :
	cd core && $(MAKE)

agents :
	cd agents && $(MAKE)

ui :
	cd ui && qmake && $(MAKE)

CONS_DEPS =  $(addprefix $(OBJ_DIR)/, Card.o Deck.o Action.o \
 HandEvaluator.o HandRanks.o HandHistory.o Hand.o GameView.o \
 LoggerEventListener.o EventManager.o TestAgent.o SimpleAgent.o RandomAgent.o \
 HumanAgent.o Game.o Random.o ConsoleMain.o)

$(OBJ_DIR)/ConsoleMain.o : core core/src/ConsoleMain.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c core/src/ConsoleMain.cc -o $@
$(BIN_DIR)/console_main : core agents $(CONS_DEPS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(CONS_DEPS) -o $@
