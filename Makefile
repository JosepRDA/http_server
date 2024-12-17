# Source directories for server and client
SRC_DIR_SERVER := src/server
SRC_DIR_CLIENT := src/client

# Output directory for object files and executables
BIN_DIR := bin

# Server and client source files
SERVER_SRC := $(wildcard $(SRC_DIR_SERVER)/*.c)
CLIENT_SRC := $(wildcard $(SRC_DIR_CLIENT)/*.c)

# Object files in the bin directory
SERVER_OBJ := $(SERVER_SRC:$(SRC_DIR_SERVER)/%.c=$(BIN_DIR)/%.o)
CLIENT_OBJ := $(CLIENT_SRC:$(SRC_DIR_CLIENT)/%.c=$(BIN_DIR)/%.o)

# Output target executables
TARGET1 := $(BIN_DIR)/server
TARGET2 := $(BIN_DIR)/client

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -O2

# Default target
all: $(TARGET1) $(TARGET2)

# Link server and client object files into their respective executables
$(TARGET1): $(SERVER_OBJ)
	$(CC) $(SERVER_OBJ) -o $@

$(TARGET2): $(CLIENT_OBJ)
	$(CC) $(CLIENT_OBJ) -o $@

# Compile server source files into object files
$(BIN_DIR)/%.o: $(SRC_DIR_SERVER)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile client source files into object files
$(BIN_DIR)/%.o: $(SRC_DIR_CLIENT)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean build files
clean:
	rm -f $(BIN_DIR)/*.o $(TARGET1) $(TARGET2)

# Run both server and client executables
run:
	./$(TARGET1) & ./$(TARGET2)
