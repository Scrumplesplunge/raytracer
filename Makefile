# Raytracer Makefile.

# Compiler
CC=g++

# Paths.
SOURCE=src
OBJECT=obj
BINARY=bin
ASSEMBLER=asm

# Source path
SOURCES=$(wildcard $(SOURCE)/*.cpp)
OBJECTS=$(addprefix $(OBJECT)/,$(notdir $(SOURCES:.cpp=.o)))
ASSEMBLERS=$(addprefix $(ASSEMBLER)/,$(notdir $(SOURCES:.cpp=.s)))

# Compiler flags
LFLAGS=-O3 -pthread -msse3 -mmmx -std=c++11
CFLAGS=-O3 -pthread -msse3 -mmmx -std=c++11 -MMD

# Output binary name
EXECUTABLE=raytracer
TEST=test

# Put everything together.
all: $(EXECUTABLE) $(TEST)

$(TEST): test/Test.o $(filter-out $(OBJECT)/Main.o,$(OBJECTS))
	$(CC) $(LFLAGS) $^ -o $(BINARY)/$@

test/Test.o: test/Test.cpp $(filter-out $(OBJECT)/Main.o,$(OBJECTS))
	$(CC) $(CFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) -o $(BINARY)/$@ $^

assembler: $(ASSEMBLERS)

$(ASSEMBLER)/%.s: src/%.cpp
	$(CC) $(CFLAGS) -c $< -fverbose-asm -S -o $@

$(OBJECT)/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build.
clean:
	rm -rf $(OBJECT)/*

# Include the generated dependency graphs from the compilation process.
-include $(OBJECTS:.o=.d)
