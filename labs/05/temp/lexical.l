%{
#include <stdio.h>
#define FILE_NAME "tokens.out"
FILE *outputFile;
%}

%%
[\n\t' '] {};
^[A-Z] fprintf(outputFile,"<sg,%s>", yytext);
[A-Z] fprintf(outputFile,"<g,%s>", yytext);
[a-z] fprintf(outputFile,"<t,%s>", yytext);
\=    fprintf(outputFile,"<assign,%s>", yytext);
\|    fprintf(outputFile,"<or,%s>", yytext);
%%

int main(int argc, char **argv) {
    FILE *fd;
    outputFile = fopen(FILE_NAME, "w");
    if (argc == 2)
    {
        if (!(fd = fopen(argv[1], "r")))
        {
            perror("Error: ");
            return (-1);
        }
        yyset_in(fd);
        yyset_out(outputFile);
        yylex();
        fclose(fd);
    }
    else
        printf("Usage: a.out filename\n");
        
    fclose(outputFile);
    return (0);
}