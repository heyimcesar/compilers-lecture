all:
	flex lexic_analyzer.l
	gcc lex.yy.c -o lexic_analyzer -ll
	gcc syntax_calc.c -o syntax_calc
clean:
	rm -rf lex.yy.c
	rm -rf lexic_analyzer
	rm -rf syntax_calc
	rm -rf *.ac
	rm -rf *.out