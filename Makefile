SRC_DIR := src/core
OBJ_DIR := obj
BIN_DIR := bin

OUT := main.out
OUT_DIR := $(BIN_DIR)/

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CC       := g++
CPPFLAGS := -std=c++11 -Isrc/include -MMD -MP
CFLAGS   := -Wall
LDFLAGS  := -Llib
LDLIBS   := -lm

ARGS     := README.md

.PHONY: all clean run

all: clean $(OUT) run

run: $(OUT)
	@echo
	@echo --OUTPUT--
	@./$(OUT_DIR)./$(OUT) $(ARGS)

$(OUT): $(OBJ) | $(BIN_DIR)
	@echo COMP $^ out: $@
	@$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $(OUT_DIR)$@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo LINK $< out: $@
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	@mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)
	@echo

-include $(OBJ:.o=.d)
