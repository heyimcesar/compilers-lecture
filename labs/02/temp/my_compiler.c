#include <stdio.h>
#include <string.h>

#define bufSize 1024

void printError(int linePos, char *message, char *line)
{
    printf("\033[1;31m");
    printf("Error: ");
    printf("\033[0m");
    printf("%s ", message);
    printf("\033[1;32m");
    printf("(line %i):\n", linePos);
    printf("\033[0m");
    printf("%s\n", line);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    char buf[bufSize];
    int parenthesis = 0;            // ()
    int brackets = 0;               // []
    int braces = 0;                 // {}
    int activeDoubleQuote = 0;      // ""
    int activeSingleQuote = 0;      // ''
    int activeMultilineComment = 0; // /* */
    char lastActiveCharacter;
    int activeLine = 0;
    int errors = 0;
    int counterPBB = 0;
    char *stackChar;
    int *stackPos;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <soure-file>\n", argv[0]);
        return 1;
    }

    if ((fp = fopen(argv[1], "r")) == NULL)
    { /* Open source file. */
        perror("fopen source-file");
        return 1;
    }

    // Count number of Parenthesis, Brackets and Braces to declare the stack
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        size_t lineLength = strlen(buf) - 1;
        buf[lineLength] = '\0';
        int counter;
        for (counter = 0; counter < lineLength; counter++)
        {
            if (buf[counter] == '(' || buf[counter] == ')' || buf[counter] == '{' || buf[counter] == '}' || buf[counter] == '[' || buf[counter] == ']')
            {
                counterPBB++;
            }
        }
    }
    rewind(fp);
    //stackChar = malloc(counterPBB * sizeof(char));
    //stackPos = malloc(counterPBB * sizeof(int));

    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        activeLine++;
        size_t lineLength = strlen(buf) - 1;
        buf[lineLength] = '\0';
        int counter;
        for (counter = 0; counter < lineLength; counter++)
        {
            // Ignore everything if a multiple line comment is open
            if (activeMultilineComment)
            {
                if (buf[counter] == '*' && counter < lineLength - 1 && buf[counter + 1] == '/')
                    activeMultilineComment = 0;
                continue;
            }

            // Ignore everything if a double quote is open
            if (activeDoubleQuote)
            {
                if (buf[counter] == '\"')
                    activeDoubleQuote = 0;

                continue;
            }

            // Single quote must have max 1 character inside Ex: '' or '
            //if (buf[lineLength] == '\'' && lineLength)

            // Line comment (ignore the rest of the line)
            if (buf[counter] == '/' && counter < lineLength - 1 && buf[counter + 1] == '/')
                break;

            // Balanced cases
            switch (buf[counter])
            {
            case '(':
                parenthesis++;
                lastActiveCharacter = '(';
                break;
            case ')':
                parenthesis--;
                break;
            case '[':
                brackets++;
                lastActiveCharacter = '[';
                break;
            case ']':
                brackets--;
                break;
            case '{':
                braces++;
                break;
            case '}':
                braces--;
                break;
            case '\"':
                activeDoubleQuote = 1;
                break;
            case '/':
                if (counter < lineLength - 1 && buf[counter + 1] == '*')
                {
                    activeMultilineComment = 1;
                    counter++;
                }
                break;
            }
        }
        if (activeDoubleQuote)
        {
            errors++;
            printError(activeLine, "missing \" character", buf);
        }
    }
    fclose(fp);

    printf("Parenthesis (): %i\n", parenthesis);
    printf("Brackets []: %i\n", brackets);
    printf("Braces {}: %i\n", braces);
    printf("Count : %i\n", counterPBB);
    return errors;
}