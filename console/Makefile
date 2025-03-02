CC = gcc
BASE_DIR = ./console
BUILD_DIR = ./build

ALL_C = $(wildcard $(BASE_DIR)/*.c)  
ALL_O = $(patsubst $(BASE_DIR)/%.c, $(BUILD_DIR)/%.o, $(ALL_C))

all: $(BUILD_DIR)/main

$(BUILD_DIR)/main: $(ALL_O) | $(BUILD_DIR)
	$(CC) $(ALL_O) -o $@

$(BUILD_DIR)/%.o: $(BASE_DIR)/%.c | $(BUILD_DIR)
	$(CC) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean