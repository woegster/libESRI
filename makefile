# Declaration of variables
CC = g++
CC_FLAGS = -w -std=c++11 -D BUILDING_ESRI -lpthread -fvisibility=hidden -fPIC
LINKER=g++

# File names
EXEC = libESRI.so
SOURCES = $(filter-out src/stdafx.cpp, $(wildcard src/*.cpp))
OBJECTS = $(SOURCES:.cpp=.o)

# Main target
$(EXEC): $(OBJECTS)
	$(LINKER) -shared -Wl,-soname,$(EXEC) -o $(EXEC) $(OBJECTS) -lc

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)