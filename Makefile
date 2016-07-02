# Raytracer Makefile.

# Compiler
CC=g++

# Paths.
TESTPATH=test
SOURCE=src
OBJECT=obj
BINARY=bin
ASSEMBLER=asm

# Source path
SOURCES=$(wildcard $(SOURCE)/*.cpp)
OBJECTS=$(addprefix $(OBJECT)/$(SOURCE)/,$(notdir $(SOURCES:.cpp=.o)))
TESTSRC=$(wildcard $(TESTPATH)/*.cpp)
TESTOBJ=$(addprefix $(OBJECT)/$(TESTPATH)/,$(notdir $(TESTSRC:.cpp=.o)))
ASSEMBLERS=$(addprefix $(ASSEMBLER)/$(SOURCE)/,$(notdir $(SOURCES:.cpp=.s)))

# Compiler flags
FLAGS=-pthread -msse3 -mmmx -std=c++11 
TLFLAGS=-Og -pg $(FLAGS)
TCFLAGS=$(TLFLAGS) -MMD
LFLAGS=-Ofast -march=native -flto -Wl,--gc-sections -s $(FLAGS)
CFLAGS=$(LFLAGS) -MMD

# Output binary name
EXECUTABLE=raytracer
TEST=test

# Put everything together.
all: $(EXECUTABLE) $(TEST)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) -o $(BINARY)/$@ $^

$(OBJECT)/$(SOURCE)/%.o: $(SOURCE)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST): $(TESTOBJ) $(filter-out $(OBJECT)/$(SOURCE)/Main.o,$(OBJECTS))
	$(CC) $(TLFLAGS) -o $(BINARY)/$@ $^

$(OBJECT)/$(TESTPATH)/%.o: $(TESTPATH)/%.cpp
	$(CC) $(TCFLAGS) -c $< -o $@

# assembler: $(ASSEMBLERS)

# $(ASSEMBLER)/%.s: $(SOURCE)/%.cpp
# 	$(CC) $(CFLAGS) -c $< -fverbose-asm -S -o $@

# Clean build.
clean:
	rm -rf $(BINARY)/*
	rm -rf $(OBJECT)/*
	mkdir -p $(OBJECT)/$(SOURCE)
	mkdir -p $(OBJECT)/$(TESTPATH)
	rm -rf $(ASSEMBLER)/*
	mkdir -p $(ASSEMBLER)/$(SOURCE)

# Include the generated dependency graphs from the compilation process.
-include $(OBJECTS:.o=.d)
-include $(TESTOBJ:.o=.d)
