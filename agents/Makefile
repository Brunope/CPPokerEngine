CXX = clang++

SHELL = /bin/sh

SRC_DIR = ./src
INC_DIR = ./inc
OBJ_DIR = ../obj
BIN_DIR = ./bin
DEP_DIR = ./dep
CORE_INC_DIR = ../core/inc

CXXFLAGS += -g -std=c++11 -I $(INC_DIR) -I $(CORE_INC_DIR)

############################################################
# Agents
############################################################

UNITS = SlowAgent HumanAgent RandomAgent SimpleAgent TestAgent TightAgent \
 SlowTightAgent SlowRandomAgent Sleeper

SRCS = $(wildcard, $(SRC_DIR)/*)
OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(UNITS)))

############################################################
# Build targets
############################################################

all : $(OBJS)

clean :
	rm -rf $(OBJS) $(DEP_DIR)

############################################################
# Auto generate dependency rules for agent sources
############################################################
$(shell mkdir -p $(DEP_DIR))
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.Td

DEP_COMPILE = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c
DEP_POSTCOMPILE = mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d

$(OBJ_DIR)/%.o : %.cc
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cc $(DEPDIR)/%.d
	$(DEP_COMPILE) $(OUTPUT_OPTION) $<
	$(DEP_POSTCOMPILE)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cc $(DEPDIR)/%.d
	$(DEP_COMPILE) $(OUTPUT_OPTION) $<
	$(DEP_POSTCOMPILE)

############################################################
# Finish the voodoo magic to generate rules to compile the source units
############################################################
$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))
