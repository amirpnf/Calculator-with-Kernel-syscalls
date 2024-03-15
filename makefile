CC=gcc 
CFLAGS= -Wall
BUILD_DIR=build
SRC_DIR=src

SRC=$(wildcard $(SRC_DIR)/*.c)

PROGS=$(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%)

all: $(PROGS)

$(BUILD_DIR)/%: $(SRC_DIR)/%.c $(SRC_DIR)/try.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<

clean:
	rm -rf $(BUILD_DIR)	