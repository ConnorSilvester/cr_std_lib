# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall

# Change this as needed
# Installation directories
LIB_DEST_DIR = /usr/local/lib
INCLUDE_DEST_DIR = /usr/local/include

# Executable name
EXEC = bin/main
TEST_EXEC = bin/tests

# Source directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
TEST_DIR = tests

# Library name
LIB_NAME = cr_std
LIB_FILE = lib$(LIB_NAME).a

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c, $(OBJ_DIR)/test_%.o, $(TEST_SRCS))


# Build executable
$(EXEC): $(OBJS)
	mkdir -p bin
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Compile test source files
$(OBJ_DIR)/test_%.o: $(TEST_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Create the static library, excluding main.o
$(LIB_FILE): $(filter-out $(OBJ_DIR)/main.o, $(OBJS))
	ar rcs $@ $(filter-out $(OBJ_DIR)/main.o, $(OBJS))

# Install the library and headers
install: $(LIB_FILE)
	@echo "Installing library to $(LIB_DEST_DIR)..."
	sudo cp $(LIB_FILE) $(LIB_DEST_DIR)
	@echo "Installing header files to $(INCLUDE_DEST_DIR)..."
	for header in $(wildcard $(INC_DIR)/*.h); do \
		sudo cp $$header $(INCLUDE_DEST_DIR); \
	done

# Run the program
run: $(EXEC)
	$(EXEC)

# Build tests with CR_STD_TESTING_MODE define to block output from logger
$(TEST_EXEC): $(TEST_OBJS) $(LIB_FILE)
	mkdir -p bin
	$(CC) $(CFLAGS) $(TEST_OBJS) -L. -l$(LIB_NAME) -o $(TEST_EXEC)

run_tests: clean $(TEST_EXEC)
	$(TEST_EXEC)

# Clean
clean:
	rm -rf $(OBJ_DIR) bin $(LIB_FILE)

# Debug build
debug: CFLAGS += -g
debug: $(EXEC)

.PHONY: all clean run install debug run_tests

