CC = cc
CFLAGS = -ggdb -Wall -Wextra -std=c99 -lm
TARGET = bin/G-Cut
<<<<<<< HEAD
TEST = --output wyjscie.txt --input data/graf.csrrg --divide 10 --margin 0.1
=======
TEST = --output wyjscietxt --input data/graf1.csrrg --divide 5000 --margin 0.2
>>>>>>> 315314f296e2b5be05a0752107a26656db5d18dc
FILES = src/G-Cut.c src/matrix.c src/vector.c src/input.c src/groups.c src/graph.c

compile: src/G-Cut.c src/matrix.c src/vector.c src/input.c
	$(CC) $(FILES) -o $(TARGET) $(CFLAGS)

valgrind:
	valgrind --track-origins=yes --leak-check=full -s ./$(TARGET) $(TEST)

execute: 
	./$(TARGET) $(TEST)

