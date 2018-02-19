CFLAGS = -std=c99 -Wall -pedantic -g
BIN = main
CC = gcc
RM = rm -f

main: main.c
	$(CC) $(CFLAGS) -o $(BIN) scanner.c parser.c str.c main.c errors.c precedence.c symtable.c stack.c queue.c funkce.c -o main


clean:
	$(RM) *.o $(BIN)
