CXX = clang++
AR = ar

SHELL = /bin/sh

# Points to the root of Google Test, relative to where this file is.
GTEST_DIR = ./gtest

# Where to find my code.
SRC_DIR = ./src
INC_DIR = ./inc

# Where to find my tests
TEST_DIR = ./test

# Hide all the .o here
OBJ_DIR = ../obj

# Put all the binaries here
BIN_DIR = ../bin

# Generated dependency files here
DEP_DIR = ./dep

RESOURCE_DIR = ./resources

# Flags passed to the C++ compiler.
CXXFLAGS += -g -std=c++11 -I $(INC_DIR)

############################################################
# Game library source files
############################################################
UNITS = Card Deck Action Hand HandEvaluator HandRanks HandHistory GameView \
 EventManager LoggerEventListener Game Random

SRCS = $(addprefix $(SRC_DIR)/, $(addsuffix .cc, $(UNITS)))
OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(UNITS)))

############################################################
# House-keeping build targets
############################################################

all : | setup $(OBJS) poker.a

clean :
	rm -rf $(OBJS) $(DEP_DIR) *.log .DS_STORE poker.a

# Poker static library
poker.a : $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

# Setup - get the right HandRanks.dat lookup file
HandRanksDat_Fpath = $(RESOURCE_DIR)/HandRanks.dat
HandRanksDat_Url = "https://www.dropbox.com/s/iw6yuk8naanppn0/HandRanks.dat?dl=0"
HandRanksDat_Bytes = "129951336"

setup :
	mkdir -p $(BIN_DIR) $(OBJ_DIR) $(RESOURCE_DIR)
	if [ ! -f $(HandRanksDat_Fpath) ] || [ `wc -c < $(HandRanksDat_Fpath)` != $(HandRanksDat_Bytes) ]; then wget $(HandRanksDat_Url) -O resources/HandRanks.dat; fi

main : $(OBJ_DIR)/ConsoleMain.o

############################################################
# Auto generate dependency rules for game library sources
############################################################

$(shell mkdir -p $(DEP_DIR))
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.Td

DEP_COMPILE = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c
DEP_POSTCOMPILE = mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d

$(OBJ_DIR)/%.o : %.cc
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cc $(DEPDIR)/%.d
	$(DEP_COMPILE) $(OUTPUT_OPTION) $<
	$(DEP_POSTCOMPILE)

$(OBJ_DIR)/%.o : $(TEST_DIR)/%.cc $(DEPDIR)/%.d
	$(DEP_COMPILE) $(OUTPUT_OPTION) $<
	$(DEP_POSTCOMPILE)

$(OBJ_DIR)/HandRanks.o : $(SRC_DIR)/HandRanks.S $(HandRanksDat_Fpath)
	$(CXX) -D__HandRanks_file_path='"$(HandRanksDat_Fpath)"' \
 -c $(SRC_DIR)/HandRanks.S -o $@


############################################################
# Finish the voodoo magic to generate rules to compile the source units
############################################################
$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))
