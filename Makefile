CC = cc
CFLAGS = -Wall -Wextra -std=c99 -lm
TARGET = bin/G-Cut
TEST = data/graf.csrrg
FILES = src/G-Cut.c src/matrix.c

compile: src/G-Cut.c src/matrix.c
	$(CC) $(FILES) -o $(TARGET) $(CFLAGS)

execute: 
	./$(TARGET) $(TEST)
