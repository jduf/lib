EXEC=test

test_SRCS=test.cpp IOFiles.cpp Header.cpp RST.cpp Linux.cpp Gnuplot.cpp ACiD.cpp Lapack.cpp

JDLIB = ../lib
BUILD = build
#-----------------------------------------------------------------

CXX = g++ -std=c++14

OPTION =
LAPACK = -llapack -lblas
ERRORS = -Wall -Wextra -pedantic

VPATH = $(JDLIB) .
MYLIB = $(addprefix -I,$(VPATH))
-include $(JDLIB)/config.mk

CXXFLAGS = $(MYLIB) $(ERRORS) $(OPTION) $(CONFIG)
LDFLAGS  = $(MYLIB) $(ERRORS) $(OPTION) $(LAPACK)

all: OPTION += -O3 -DNDEBUG
all:$(EXEC)

debug: OPTION += -ggdb
debug:$(EXEC)

.SECONDEXPANSION:
$(EXEC): $$(patsubst %.cpp, $(BUILD)/%.o, $$($$@_SRCS))
	@echo Links $(notdir $^)
	$(CXX) $^ $(LDFLAGS) -o $@

$(BUILD)/%.o:%.cpp
	@echo Creates $(notdir $@)
	$(CXX) $< $(CXXFLAGS) -MMD -c -o $@

-include $(wildcard $(BUILD)/*.d)

clean:
	rm -f $(BUILD)/* test plot* data.dat
