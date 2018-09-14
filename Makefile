BIN := run_backtest
SRC_DIR := ./src
SRCS := $(notdir $(wildcard $(SRC_DIR)/*.cpp))

#intermediate directory for genreated object files
OBJDIR := .o
#intermediate directory for generated dependency files
DEPDIR := .d

# object files, auto generated from source files
OBJS := $(patsubst %, $(OBJDIR)/%.o, $(basename $(SRCS)))

$(shell mkdir -p $(DEPDIR))
$(shell mkdir -p $(dir $(OBJS)))

# compiler and linker
CXX := g++
LD := g++

# c++ flags, linker flags, flags required for dependency generation, and libraries
CXXFLAGS := -std=c++11 -pg -O3
CPPFLAGS := -I./include -I/usr/include/eigen3 #-I~/pf/include -I./include 
LDFLAGS := #-pg #"-L/home/t/pf/bin" 
LDLIBS := #-lpf -lpthread 
DEPFLAGS = -MT $@ -MD -MP -MF $(DEPDIR)/$*.Td

# compile c++ source files and link objects to binary
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c -o $@
LINK.o = $(LD) $(LDFLAGS) -o $@
PRECOMPILE = @echo %.cpp
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

all: $(BIN)

.PHONY: clean
clean:
	$(RM) -r $(OBJDIR) $(DEPDIR)

.PHONY: help
help:
	@echo available targets: all clean

print-%  : ; @echo $* = $($*)

$(BIN): $(OBJS)
	$(LINK.o) $^ $(LDLIBS)

$(OBJDIR)/%.o: $(SRC_DIR)/%.cpp
$(OBJDIR)/%.o: $(SRC_DIR)/%.cpp $(DEPDIR)/%.d
	$(PRECOMPILE)
	$(COMPILE.cc) $<
	$(POSTCOMPILE)

.PRECIOUS = $(DEPDIR)/%.d
$(DEPDIR)/%.d: ;

-include $(DEPS)
