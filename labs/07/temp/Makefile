all:
	yacc -d syntax_calc.y
	lex lexic_analyzer.l 
	gcc -o compiler y.tab.c lex.yy.c -ly -ll
clean:
	rm -rf lex.yy.c
	rm -rf y.tab.c
	rm -rf compiler
	rm -rf *.h
	rm -rf *.ac
	rm -rf *.out
	rm -rf *.output