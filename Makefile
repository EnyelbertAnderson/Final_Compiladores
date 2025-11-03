CC = gcc
CFLAGS = -Wall -g
LEX = flex

all: mini0_lex

mini0_lex: lex.yy.o token.o
	$(CC) $(CFLAGS) -o mini0_lex lex.yy.o token.o -lfl

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c

lex.yy.c: mini0_lex.l
	$(LEX) mini0_lex.l

token.o: token.c token.h
	$(CC) $(CFLAGS) -c token.c

clean:
	rm -f mini0_lex lex.yy.c lex.yy.o token.o

.PHONY: all clean