# Makefile for Palavras Cruzadas (Windows MinGW/w64devkit)

# Compiler and paths
CC = gcc
RAYLIB_PATH ?= C:/raylib/w64devkit

# Flags
CFLAGS = -g -std=c99 -Wall -Wno-missing-braces -I$(RAYLIB_PATH)/include -IC:/raylib/raylib/src -IC:/raylib/include -Isrc
LDFLAGS = -L$(RAYLIB_PATH)/lib -Lsrc -lraylib -lopengl32 -lgdi32 -lwinmm

# Source files
SRC = src/app/main.c \
      src/ui/interface.c \
      src/ui/ui_components.c \
      src/data/dictionary.c \
      src/data/grid.c \
      src/external/api_client.c \
      src/game/solver.c \
      src/game/validation.c \
      src/game/puzzle_generator.c \
      src/game/game_mode.c

# Output
TARGET = bin/main.exe

# Rules
all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p bin
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f bin/main.exe
