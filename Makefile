# Project name
PROJECT = passli
TEST_PROJECT = test_$(PROJECT)

# Compiler and flags
CXX = clang++
CXXFLAGS = -c -Wall -std=c++23 -MMD -MP -I/usr/include
LFLAGS = -Wall -lgpgme
TEST_LFLAGS = $(LFLAGS) -lgtest -lgtest_main -lpthread

# Directories
SRC_DIR = src
TEST_DIR = tests
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = include
MAIN_OBJECT = $(OBJ_DIR)/main.o

# Source files
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp') $(shell find $(SRC_DIR) -name '*.c')
TEST_SOURCES = $(shell find $(TEST_DIR) -name '*.cpp') $(shell find $(TEST_DIR) -name '*.c')

# Object files and dependency files
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES)))
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.cpp, $(OBJ_DIR)/test_%.o, $(patsubst $(TEST_DIR)/%.c, $(OBJ_DIR)/test_%.o, $(TEST_SOURCES)))
DEPS = $(OBJECTS:.o=.d) $(TEST_OBJECTS:.o=.d)
  
# Executable
EXECUTABLE = $(BIN_DIR)/$(PROJECT)
TEST_EXECUTABLE = $(BIN_DIR)/$(TEST_PROJECT)

# Default target
all: $(EXECUTABLE) $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

# Test
test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Build executable
$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(LFLAGS) $(OBJECTS) -o $@

# Build test executable exluding the main object so the correct gtest main linked in
$(TEST_EXECUTABLE): $(TEST_OBJECTS) $(filter-out $(MAIN_OBJECT), $(OBJECTS)) | $(BIN_DIR)
	$(CXX) $(TEST_LFLAGS) $(TEST_OBJECTS) $(filter-out $(MAIN_OBJECT), $(OBJECTS)) -o $@

# Build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -o $@
	#
# Build test object files
$(OBJ_DIR)/test_%.o: $(TEST_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/test_%.o: $(TEST_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -o $@

# Create obj and bin directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Include dependency files
-include $(DEPS)

.PHONY: all clean
