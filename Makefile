# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall

# Executable name
EXEC = bin/main

# Source directory
SRC_DIR = src

# Include directory
INC_DIR = include

# Object directory
OBJ_DIR = obj

# Library name
LIB_NAME = cr_std
LIB_FILE = lib$(LIB_NAME).a

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Installation directories
LIB_DEST_DIR = /usr/local/lib
INCLUDE_DEST_DIR = /usr/local/include

# Build executable
$(EXEC): $(OBJS)
	mkdir -p bin
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Create the static library
$(LIB_FILE): $(OBJS)
	ar rcs $@ $(OBJS)

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

# Clean
clean:
	rm -rf $(OBJ_DIR) bin $(LIB_FILE)

# Debug build
debug: CFLAGS += -DDEBUG
debug: $(EXEC)

.PHONY: all clean run install debug
