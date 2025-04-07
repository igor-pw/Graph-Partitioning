CC = cc
CFLAGS = -ggdb -Wall -Wextra -std=c99 -lm
TARGET = bin/G-Cut
TEST = data/graf4.csrrg
FILES = src/G-Cut.c src/matrix.c src/vector.c

compile: src/G-Cut.c src/matrix.c src/vector.c
	$(CC) $(FILES) -o $(TARGET) $(CFLAGS)

valgrind:
	valgrind ./$(TARGET) $(TEST)

execute: 
	./$(TARGET) $(TEST)
