all:
	flex lexical.l lex.yy.c
	gcc lex.yy.c -o lexical -ll

clean:
	rm -rf *.c
	rm -rf lexical
	rm -rf *.out