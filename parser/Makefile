CFLAGS = -g

all: server db parser lexer expression client_parser

server:
	cd ../network && $(MAKE) server.o

db:
	cd ../db && $(MAKE) sharedds.o

parser: lexer
	bison Parser.y

lexer:
	lex Lexer.l

expression: lexer parser
	g++ -c Expression.cc $(CFLAGS)

client_parser: lexer parser expression server db
	g++ -c client_parser.cc $(CFLAGS)

clean:
	rm -f Lexer.cc Lexer.h Parser.cc Parser.h *.o
