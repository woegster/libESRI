# Declaration of variables
CC = g++
CC_FLAGS = -w -std=c++17 -D BUILDING_ESRI -lpthread
LINKER=ar

# File names
EXEC = libESRI.a
SOURCES = $(filter-out src/stdafx.cpp, $(wildcard src/*.cpp))
OBJECTS = $(SOURCES:.cpp=.o)

# Main target
$(EXEC): $(OBJECTS)
	$(LINKER) rcs $(EXEC) $(OBJECTS)

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)