all:
	lex compiler.l
	gcc lex.yy.c -o compiler -ll

clean:
	rm -rf *.c
	rm -rf compiler
	rm -rf *.ac
	rm -rf *.out