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


# Game library source files
UNITS = Card Deck Action Hand HandEvaluator HandRanks HandHistory GameView \
 EventManager LoggerEventListener SimpleActor RandomActor TestActor HumanActor \
 Game
SRCS = $(wildcard, $(SRC_DIR)/*)
OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(UNITS)))

# Game library test files
TESTS = card_unittest deck_unittest hand_unittest action_unittest \
 handevaluator_unittest handhistory_unittest gameview_unittest \
 loggereventlistener_unittest eventmanager_unittest game_unittest
TESTS_OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(TESTS)))
TESTS_BINS = $(addprefix $(BIN_DIR)/, $(TESTS))

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_DIR = gtest
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
 $(GTEST_DIR)/include/gtest/internal/*.h

# House-keeping build targets
all : setup $(OBJS) $(TESTS_OBJS) $(TESTS_BINS) $(BIN_DIR)/test_suite \
 $(BIN_DIR)/main poker.a

HandRanksDat_Fpath = resources/HandRanks.dat
HandRanksDat_Url = "https://www.dropbox.com/s/iw6yuk8naanppn0/HandRanks.dat?dl=0"
HandRanksDat_Bytes = "129951336"

setup :
	if [ ! -f $(HandRanksDat_Fpath) ] || [ `wc -c < $(HandRanksDat_Fpath)` != $(HandRanksDat_Bytes) ]; then wget $(HandRanksDat_Url) -O resources/HandRanks.dat; fi
	mkdir -p bin obj

test : $(BIN_DIR)/test_suite
	./$(BIN_DIR)/test_suite

clean :
	rm -rf $(TESTS) $(OBJ_DIR) $(BIN_DIR) $(TESTS_OBJS) $(DEPDIR) \
 *.a *.o *.log .DS_STORE

# Poker static library
poker.a : $(OBJS)
	$(AR) $(ARFLAGS) $@ $^


# gtest suite - build gtest.a, gtest_main.a, and bin/test_suite
$(BIN_DIR)/test_suite : gtest_main.o gtest_main.a $(TESTS_OBJS) $(OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

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

# Auto generate dependency stuff
DEPDIR = .d
$(shell mkdir -p $(DEPDIR))
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

CXXTESTFLAGS = -lpthread

$(OBJ_DIR)/%.o : %.cc
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cc $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(OBJ_DIR)/%.o : $(TEST_DIR)/%.cc $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(OBJ_DIR)/HandRanks.o : $(SRC_DIR)/HandRanks.S $(HandRanksDat_Fpath)
	$(CXX) -D__HandRanks_file_path='"$(HandRanksDat_Fpath)"' \
 -c $(SRC_DIR)/HandRanks.S -o $@

# Unit tests
# todo: figure out how to pattern rule these
#
# the magic incantation that outputs the right dependencies, not sure if
# better because more reliable or worse because it looks like nonsense
# doesn't work for long dependency files
# define unittest_template
# $(BIN_DIR)/$(1) : gtest_main.a $$(shell cat .d/$(1).d | head -1 | sed -E \
#  's/.*: //g; s/test\//obj\//g; s/\.cc/\.o/g; \
#  s/inc\//obj\//g; s/\.h/\.o/g')
# 	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $$@
# endef

# $(foreach unittest,$(TESTS),$(eval $(call unittest_template,$(unittest))))

$(BIN_DIR)/card_unittest : $(addprefix $(OBJ_DIR)/, Card.o card_unittest.o)  \
 gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(CXXTESTFLAGS) $^ -o $@

$(BIN_DIR)/deck_unittest : $(addprefix $(OBJ_DIR)/, Card.o  Deck.o \
 deck_unittest.o) \
 gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(CXXTESTFLAGS) $^ -o $@

$(BIN_DIR)/handevaluator_unittest : $(addprefix $(OBJ_DIR)/, Card.o \
 HandEvaluator.o Hand.o HandRanks.o handevaluator_unittest.o) \
 gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(CXXTESTFLAGS) $^ -o $@

$(BIN_DIR)/hand_unittest : $(addprefix $(OBJ_DIR)/, Card.o Hand.o \
 HandEvaluator.o HandRanks.o hand_unittest.o) \
  gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

$(BIN_DIR)/action_unittest : $(addprefix $(OBJ_DIR)/, Card.o Action.o \
 action_unittest.o) \
 gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

$(BIN_DIR)/handhistory_unittest : $(addprefix $(OBJ_DIR)/, Card.o Action.o \
 HandEvaluator.o HandRanks.o Hand.o HandHistory.o handhistory_unittest.o) \
 gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

$(BIN_DIR)/gameview_unittest : $(addprefix $(OBJ_DIR)/, Card.o Action.o \
 HandEvaluator.o HandRanks.o HandHistory.o Hand.o GameView.o \
 gameview_unittest.o) \
 gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

$(BIN_DIR)/loggereventlistener_unittest : $(addprefix $(OBJ_DIR)/, Card.o \
 Action.o HandEvaluator.o HandRanks.o HandHistory.o Hand.o GameView.o \
 LoggerEventListener.o) \
 gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

$(BIN_DIR)/eventmanager_unittest : $(addprefix $(OBJ_DIR)/, Card.o Action.o \
 HandEvaluator.o HandRanks.o HandHistory.o Hand.o GameView.o \
 LoggerEventListener.o EventManager.o eventmanager_unittest.o) \
 gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

$(BIN_DIR)/game_unittest : $(addprefix $(OBJ_DIR)/, Card.o Deck.o Action.o \
 HandEvaluator.o HandRanks.o HandHistory.o Hand.o GameView.o \
 LoggerEventListener.o EventManager.o TestActor.o SimpleActor.o RandomActor.o \
 Game.o) \
 gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

$(BIN_DIR)/main : $(addprefix $(OBJ_DIR)/, Card.o Deck.o Action.o \
 HandEvaluator.o HandRanks.o HandHistory.o Hand.o GameView.o \
 LoggerEventListener.o EventManager.o TestActor.o SimpleActor.o RandomActor.o \
 HumanActor.o Game.o Main.o)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@


# Finish the voodoo magic to generate rules to compile the source units
$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))
