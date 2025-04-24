CC = cc
CFLAGS = -ggdb -Wall -Wextra -std=c99 -lm
TARGET = bin/G-Cut
TEST = --output wyjscie.txt --input data/graf4.csrrg --divide 5 --margin 0.1
FILES = src/G-Cut.c src/matrix.c src/vector.c src/input.c src/groups.c src/graph.c

compile: src/G-Cut.c src/matrix.c src/vector.c src/input.c
	$(CC) $(FILES) -o $(TARGET) $(CFLAGS)

valgrind:
	valgrind ./$(TARGET) $(TEST)

execute: 
	./$(TARGET) $(TEST)
	
