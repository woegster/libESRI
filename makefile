# Declaration of variables
CC = g++
CC_FLAGS = -w -std=c++11

# File names
EXEC = run
SOURCES = $(filter-out src/stdafx.cpp, $(wildcard src/*.cpp))
OBJECTS = $(SOURCES:.cpp=.o)

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)