CC = cc -Wall -Wextra -Werror -pthread -I .

all: comp run

comp:
	$(CC) *.c -g -o test

run: $(comp)
	./test 2 100 50 50 50 10 5 edf
