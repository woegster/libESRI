# Declaration of variables
CC = g++
ARM_CC = arm-linux-gnueabihf-g++-4.7
CC_FLAGS = -w -std=c++11 -D BUILDING_ESRI -lpthread -fvisibility=hidden -fPIC
LINKER=g++
ARM_LINKER = arm-linux-gnueabihf-g++-4.7

# File names
EXEC = libESRI.so
SOURCES = $(filter-out src/stdafx.cpp src/OsTools_Windows.cpp, $(wildcard src/*.cpp))
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS_ARM = $(SOURCES:.cpp=.oarm)

# Main target
$(EXEC): $(OBJECTS)
	$(LINKER) -shared -Wl,-soname,$(EXEC) -o $(EXEC) $(OBJECTS) -lc

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS) $(OBJECTS_ARM)

arm: $(OBJECTS_ARM)
	$(ARM_LINKER) -shared -Wl,-soname,$(EXEC) -o $(EXEC) $(OBJECTS_ARM) -lc

# To obtain object files
%.oarm: %.cpp
	$(ARM_CC) -c $(CC_FLAGS) $< -o $@

