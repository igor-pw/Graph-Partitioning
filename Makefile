CC = cc
CFLAGS = -ggdb -Wall -Wextra -std=c99 -lm
TARGET = bin/G-Cut
TEST = --output wyjscie.txt --input data/graf6.csrrg --divide 2 --margin
FILES = src/G-Cut.c src/matrix.c src/vector.c src/input.c src/groups.c

compile: src/G-Cut.c src/matrix.c src/vector.c src/input.c
	$(CC) $(FILES) -o $(TARGET) $(CFLAGS)

valgrind:
	valgrind ./$(TARGET) $(TEST)

execute: 
	./$(TARGET) $(TEST)
