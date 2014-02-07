# Raytracer Makefile.

# Compiler
CC=g++

# Paths.
SOURCE=src
OBJECT=obj
BINARY=bin

# Source path
SOURCES=$(wildcard $(SOURCE)/*.cpp)
OBJECTS=$(addprefix $(OBJECT)/,$(notdir $(SOURCES:.cpp=.o)))

# Compiler flags
LFLAGS=
CFLAGS=-O3 -pthread -std=c++11 -MMD

# Output binary name
EXECUTABLE=raytracer

# Put everything together.
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) -o $(BINARY)/$@ $^

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean build.
clean:
	rm -rf $(OBJECT)/*

# Include the generated dependency graphs from the compilation process.
-include $(OBJECTS:.o=.d)
