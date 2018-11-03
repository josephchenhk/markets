BIN := run_backtest
SRC_DIR := .
SRCS := $(notdir $(wildcard $(SRC_DIR)/*.cpp))
OBJDIR := .o
DEPDIR := .d

# object files, auto generated from source files
OBJS := $(patsubst %,$(OBJDIR)/%.o,$(basename $(SRCS)))

# compilers (at least gcc and clang) don't create the subdirectories automatically
$(shell mkdir -p $(DEPDIR))
$(shell mkdir -p $(dir $(OBJS)) >/dev/null)

# compiler and linker stuff
CXX := g++-8
LD := g++-8
CXXFLAGS := -std=c++17 -g -Wall
CPPFLAGS := -I/home/t/pf/include -I/usr/include/eigen3 -I../include
LDFLAGS := -no-pie "-L../bin" "-L/home/t/pf/bin"
DEPFLAGS = -MT $@ -MD -MP -MF $(DEPDIR)/$*.Td
LDLIBS := -lmarkets -lstdc++fs -lpf

# compile C++ source files
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c -o $@
# link object files to binary
LINK.o = $(LD) $(LDFLAGS) -o $@ 
# precompile step
PRECOMPILE =
# postcompile step
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

all: $(BIN)

.PHONY: clean
clean:
	$(RM) -r $(OBJDIR) $(DEPDIR)

.PHONY: help
help:
	@echo available targets: all clean

$(BIN): $(OBJS)
	$(LINK.o) $^ $(LDLIBS)

$(OBJDIR)/%.o: %.cpp
$(OBJDIR)/%.o: %.cpp $(DEPDIR)/%.d
	$(PRECOMPILE)
	$(COMPILE.cc) $<
	$(POSTCOMPILE)

.PRECIOUS = $(DEPDIR)/%.d
$(DEPDIR)/%.d: ;

-include $(DEPS)
