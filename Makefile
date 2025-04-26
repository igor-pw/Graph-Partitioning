CC = cc
CFLAGS = -ggdb -Wall -Wextra -std=c99 -lm
TARGET = bin/G-Cut
TEST = --output wyjscietxt --input data/graf1.csrrg --divide 5000 --margin 0.2
FILES = src/G-Cut.c src/matrix.c src/vector.c src/input.c src/groups.c src/graph.c

compile: src/G-Cut.c src/matrix.c src/vector.c src/input.c
	$(CC) $(FILES) -o $(TARGET) $(CFLAGS)

valgrind:
	valgrind --track-origins=yes --leak-check=full -s ./$(TARGET) $(TEST)

execute: 
	./$(TARGET) $(TEST)

