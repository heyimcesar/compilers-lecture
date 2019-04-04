%{
#include <stdio.h>
#define FILE_NAME "tokens.out"
#define BUFFER_SIZE 1000
FILE *outputFile, *tempFile;
%}

%%
\/\/(.*)                                {}
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

int isEmpty(const char *str)
{
    char ch;

    do
    {
        ch = *(str++);

        // Check non whitespace character
        if(ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r' && ch != '\0')
            return 0;

    } while (ch != '\0');

    return 1;
}

void removeEmptyLines(FILE *srcFile, FILE *tempFile)
{
    char buffer[BUFFER_SIZE];

    while ((fgets(buffer, BUFFER_SIZE, srcFile)) != NULL)
    {
        /* If current line is not empty then write to temporary file */
        if(!isEmpty(buffer))
        {
            fputs(buffer, tempFile);
        }
    }
}

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

    outputFile = fopen(FILE_NAME, "r");
    tempFile = fopen("remove-blanks.tmp", "w");
    removeEmptyLines(outputFile, tempFile);
    fprintf(tempFile,"\n");
    fclose(outputFile);
    fclose(tempFile);
    remove(FILE_NAME);
    rename("remove-blanks.tmp", FILE_NAME);

    return (0);
}