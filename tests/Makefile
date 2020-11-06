CC=gcc
FLAGS=--std=gnu11 -lncurses

positions:
	@$(CC) $(FLAGS) ../src/new_positions.c ./new_positions_test.c -o test
	valgrind --leak-check=yes ./test
	@rm test