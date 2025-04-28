CC = cc
CFLAGS = -ggdb -Wall -Wextra -std=c99 -lm
TARGET = bin/G-Cut
TEST = --output wyjscie.txt --input data/graf.csrrg --divide 15 --margin 0.5 
FILES = src/G-Cut.c src/matrix.c src/vector.c src/input.c src/groups.c src/graph.c

compile: src/G-Cut.c src/matrix.c src/vector.c src/input.c
	$(CC) $(FILES) -o $(TARGET) $(CFLAGS)

valgrind:
	valgrind --track-origins=yes --show-leak-kinds=all --leak-check=full -s ./$(TARGET) $(TEST)

execute: 
	./$(TARGET) $(TEST)

