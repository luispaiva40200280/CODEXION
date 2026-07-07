CC = cc -Wall -Wextra -Werror -I .

all: comp run

comp:
	$(CC) *.c -g -o test
run: $(comp)
	./test 78 45 12 75 85 89 75 fifo
