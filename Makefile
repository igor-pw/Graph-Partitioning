CC = cc
CFLAGS = -ggdb -Wall -Wextra -std=c99 -lm
TARGET = bin/G-Cut
TEST = --output wyjscie.txt --margin 0.4 --divide 3 --input data/graf4.csrrg
FILES = src/G-Cut.c src/matrix.c src/vector.c src/input.c

compile: src/G-Cut.c src/matrix.c src/vector.c src/input.c
	$(CC) $(FILES) -o $(TARGET) $(CFLAGS)

valgrind:
	valgrind ./$(TARGET) $(TEST)

execute: 
	./$(TARGET) $(TEST)
