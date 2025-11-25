CC = gcc
CFLAGS = -Wall -g
LEX = flex

all: mini0_lex mini0_parser

mini0_lex: lex.yy.o token.o
	$(CC) $(CFLAGS) -o mini0_lex lex.yy.o token.o -lfl

mini0_parser: lex.yy_nomain.o token.o parser.o
	$(CC) $(CFLAGS) -DBUILD_WITH_PARSER_MAIN -o mini0_parser lex.yy_nomain.o token.o parser.o -lfl

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c

# objeto de lexer compilado sin main para construir el parser y evitar duplicidad de main
lex.yy_nomain.o: lex.yy.c
	$(CC) $(CFLAGS) -c -DBUILD_WITH_PARSER_MAIN -o lex.yy_nomain.o lex.yy.c

lex.yy.c: mini0_lex.l
	$(LEX) mini0_lex.l

parser.o: parser.c parser.h token.h
	$(CC) $(CFLAGS) -c parser.c

token.o: token.c token.h
	$(CC) $(CFLAGS) -c token.c

clean:
	rm -f mini0_lex mini0_parser lex.yy.c lex.yy.o lex.yy_nomain.o token.o parser.o

.PHONY: all clean