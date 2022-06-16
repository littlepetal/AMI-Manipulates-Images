CC=gcc
CFLAGS=-g -Wall -std=c99 -Iinclude
LIBS=-lm
TARGET=ami

SRC_DIR = .
OBJ_DIR = .
INC_DIR = .
_DEPS = bitmap.h manipulations.h auxiliary_functions.h
_OBJS = main.o bitmap.o manipulations.o auxiliary_functions.c

DEPS = $(patsubst %,$(INC_DIR)/%,$(_DEPS))
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(_OBJS))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET)

