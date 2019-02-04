#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define bufSize 1024

/*
    - This code looks for balanced (), {}, and []
    - Ignores everything inside ""
    - Checks that what's inside '' is a only one character (doesn't allow escaped characters or ASCII) Ex: '\n', only: 'a'
    - Ignores everything after // in that line
    - Ignores everything inside multiline comments / * * /

*/
void printError(int linePos, char *message, char *line)
{
    printf("\033[1;31m");
    printf("Error: ");
    printf("\033[0m");
    printf("%s ", message);
    printf("\033[1;33m");
    printf("(line %i):\n", linePos);
    printf("\033[0m");
    printf("%s\n", line);
}
void printSuccess(char *message)
{
    printf("\033[1;32m");
    printf("Success: ");
    printf("\033[0m");
    printf("%s\n", message);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    char buf[bufSize];
    int activeDoubleQuote = 0;      // ""
    int activeMultilineComment = 0; // /* */
    int activeLine = 0;
    int errors = 0;
    int counterPBB = 0;
    char *stackChar;
    int *stackPos;
    char **stackLines;
    int biggestLineLength = 0;
    int stackPointer = -1;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <soure-file>\n", argv[0]);
        return 1;
    }

    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        perror("fopen source-file");
        return 1;
    }

    // Count number of Parenthesis, Brackets and Braces to declare the stack
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        size_t lineLength = strlen(buf) - 1;
        if (biggestLineLength > lineLength)
        {
            biggestLineLength = lineLength;
        }
        buf[lineLength] = '\0';
        int counter;
        for (counter = 0; counter < lineLength; counter++)
        {
            if (buf[counter] == '(' || buf[counter] == '{' || buf[counter] == '[')
            {
                counterPBB++;
            }
        }
    }
    rewind(fp);
    stackChar = malloc(counterPBB * sizeof(char));
    stackPos = malloc(counterPBB * sizeof(int));
    stackLines = (char **)malloc(counterPBB * sizeof(sizeof(char *)));

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

            // Single quote must have max 1 character inside Ex: '' or 'c'
            if (buf[counter] == '\'')
            {
                if(counter < lineLength - 1 && buf[counter+1] == '\'')
                {
                    counter++;
                }
                if(counter < lineLength - 2 && buf[counter+2] == '\'')
                {
                    counter+=2;
                }
                else
                {
                    errors++;
                    printError(activeLine,"missing ' to close char statement", buf);
                }
                continue;
            }

            // Line comment (ignore the rest of the line)
            if (buf[counter] == '/' && counter < lineLength - 1 && buf[counter + 1] == '/')
                break;

            // Open PBB
            if (buf[counter] == '(' || buf[counter] == '{' || buf[counter] == '[')
            {
                stackPointer++;
                stackChar[stackPointer] = buf[counter];
                stackPos[stackPointer] = activeLine;
                stackLines[stackPointer] = (char *)malloc(lineLength);
                strcpy(stackLines[stackPointer], buf);
            }

            // Balanced cases
            switch (buf[counter])
            {
            case ')':
                if(stackPointer == -1)
                {
                    errors++;
                    printError(activeLine,"trying to close ) without opening ( first", buf);
                }
                else if (stackChar[stackPointer] == '(')
                {
                    stackChar[stackPointer] = '\0';
                    free(stackLines[stackPointer]);
                    stackPos[stackPointer] = 0;
                    stackPointer--;
                }
                else
                {
                    errors++;
                    char m[40];
                    sprintf(m, "attempt to close ) but haven't closed %c", stackChar[stackPointer]);
                    printError(stackPos[stackPointer], m, stackLines[stackPointer]);
                }
                break;
            case ']':
                if(stackPointer == -1)
                {
                    errors++;
                    printError(activeLine,"trying to close ] without opening [ first", buf);
                }
                else if (stackChar[stackPointer] == '[')
                {
                    stackChar[stackPointer] = '\0';
                    free(stackLines[stackPointer]);
                    stackPos[stackPointer] = 0;
                    stackPointer--;
                }
                else
                {
                    errors++;
                    char m[40];
                    sprintf(m, "attempt to close ] but haven't closed %c", stackChar[stackPointer]);
                    printError(stackPos[stackPointer], m, stackLines[stackPointer]);
                }
                break;
            case '}':
                if(stackPointer == -1)
                {
                    errors++;
                    printError(activeLine,"trying to close } without opening { first", buf);
                }
                else if (stackChar[stackPointer] == '{')
                {
                    stackChar[stackPointer] = '\0';
                    free(stackLines[stackPointer]);
                    stackPos[stackPointer] = 0;
                    stackPointer--;
                }
                else
                {
                    errors++;
                    char m[40];
                    sprintf(m, "attempt to close } but haven't closed %c", stackChar[stackPointer]);
                    printError(stackPos[stackPointer], m, stackLines[stackPointer]);
                }
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
    free(stackChar);
    free(stackPos);
    free(stackLines);
    if(errors == 0) printSuccess("There is no errors");
    return errors;
}