%{
#include <stdio.h>
#define FILE_NAME "lex.out"
FILE *outputFile;
%}

%%
\/\/(.*)                                fprintf(outputFile,"COMMENT");
f                                       fprintf(outputFile,"floatdcl");
i                                       fprintf(outputFile,"intdcl");
p                                       fprintf(outputFile,"print");
=                                       fprintf(outputFile,"assign");
\+                                      fprintf(outputFile,"plus");
\-                                      fprintf(outputFile,"minus");
\/                                      fprintf(outputFile,"div");
\*                                      fprintf(outputFile,"mult");
[a-eg-hj-oq-z]                          fprintf(outputFile,"id");
(0{1}|[1-9]+[0-9]*)                     fprintf(outputFile,"inum");
(0{1}|[1-9]+[0-9]*)\.([0-9]*[1-9]+)     fprintf(outputFile,"fnum");
%%

int main(int argc, char **argv) {
    FILE    *fd;
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