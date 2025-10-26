all:
	clang -Wall -Wextra -std=c99 -pedantic main.c -o main -fsanitize=address
