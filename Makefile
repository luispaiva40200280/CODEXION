CC = cc -Wall -Wextra -Werror -pthread -I .
ARG ?= FIFO

all: comp run

comp:
	$(CC) *.c -g -o test

run: $(comp)
	./test 5 100 50 50 50 10 5 edf

