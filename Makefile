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
TESTSRC=$(wildcard $(TESTPATH)/*.cpp)
SOURCES=$(wildcard $(SOURCE)/*.cpp)
TESTOBJ=$(addprefix $(OBJECT)/$(TESTPATH)/,$(notdir $(TESTSRC:.cpp=.o)))
OBJECTS=$(addprefix $(OBJECT)/$(SOURCE)/,$(notdir $(SOURCES:.cpp=.o)))
ASSEMBLERS=$(addprefix $(ASSEMBLER)/$(SOURCE)/,$(notdir $(SOURCES:.cpp=.s)))

# Compiler flags
FLAGS=-pthread -msse3 -mmmx -std=c++11
TLFLAGS=-pg -O3 $(FLAGS)
TCFLAGS=$(TLFLAGS) -MMD
LFLAGS=-O3 $(FLAGS)
CFLAGS=$(LFLAGS) -MMD

# Output binary name
EXECUTABLE=raytracer
TEST=test

# Put everything together.
all: $(EXECUTABLE) $(TEST)

$(TEST): $(TESTOBJ) $(filter-out $(OBJECT)/$(SOURCE)/Main.o,$(OBJECTS))
	$(CC) $(TLFLAGS) $^ -o $(BINARY)/$@

$(OBJECT)/$(TESTPATH)/%.o: $(TESTPATH)/%.cpp
	$(CC) $(TCFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) -o $(BINARY)/$@ $^

assembler: $(ASSEMBLERS)

$(ASSEMBLER)/%.s: $(SOURCE)/%.cpp
	$(CC) $(CFLAGS) -c $< -fverbose-asm -S -o $@

$(OBJECT)/$(SOURCE)/%.o: $(SOURCE)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build.
clean:
	rm -rf $(OBJECT)/*
	mkdir $(OBJECT)/$(SOURCE)
	mkdir $(OBJECT)/$(TESTPATH)
	rm -rf $(ASSEMBLER)/*
	mkdir $(ASSEMBLER)/$(SOURCE)

# Include the generated dependency graphs from the compilation process.
-include $(OBJECTS:.o=.d)
