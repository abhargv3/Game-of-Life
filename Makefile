#Compiler
CC = gcc
CFLAGS = -g -Wall -std=c99 -lncursesw

#Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TST_DIR = tests
GRD_DIR = grade
OUT_DIR = output

#Files
SOURCES  := $(wildcard $(SRC_DIR)/*.c)
INCLUDES := $(wildcard $(SRC_DIR)/.*h)
OBJECTS  := $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TESTS    := $(wildcard $(TST_DIR)/*_test.c)

#Binaries
EXEC = game_of_life
TARGET := $(BIN_DIR)/$(EXEC)

.PHONY: all
all: $(TARGET)

$(TARGET): dirs $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

$(OBJECTS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: dirs
dirs:
	@ mkdir -p $(OBJ_DIR)
	@ mkdir -p $(BIN_DIR)
	@ mkdir -p $(OUT_DIR)

.PHONY: test
test:
	@ bash ./$(TST_DIR)/runtests.sh

.PHONY: grade
grade:
	@ bash ./$(GRD_DIR)/grade.sh

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(OUT_DIR)
