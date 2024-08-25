# Project name
PROJECT = passli

# Compiler and flags
CXX = clang++
CXXFLAGS = -c -Wall -std=c++23 -MMD -MP

# Linker flags
LFLAGS = -Wall

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = include

# Source files
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp') $(shell find $(SRC_DIR) -name '*.c')

# Object files and dependency files
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES)))
DEPS = $(OBJECTS:.o=.d)

# Executable
EXECUTABLE = $(BIN_DIR)/$(PROJECT)

# Default target
all: $(EXECUTABLE)

# Build executable
$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(LFLAGS) $(OBJECTS) -o $@

# Build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -o $@

# Create obj and bin directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Include dependency files
-include $(DEPS)

.PHONY: all clean run

