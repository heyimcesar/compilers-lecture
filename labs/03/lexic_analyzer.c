#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define bufSize 1024
#define FILE_NAME "lex.out"

int isNumber(char c)
{
    return (c >= '0' && c <= '9');
}

int isVariableName(char c)
{
    return (c >= 'a' && c <= 'z' && c != 'p' && c != 'f' && c != 'i');
}

void printError(int linePos, char *message)
{
    printf("\033[1;31m");
    printf("Error: ");
    printf("\033[0m");
    printf("%s ", message);
    printf("\033[1;33m");
    printf("(line %i)", linePos);
    printf("\033[0m\n");
}

char *getToken(int linePos, char *token)
{
    size_t tokenSize = strlen(token);
    int i, countDecimals = 0, countPoints = 0;
    char tempMessage[100];

    // Token is ID
    if (tokenSize == 1)
    {
        if (isVariableName(token[0]))
        {
            return "id";
        }
        if (isNumber(token[0]))
        {
            return "inum";
        }

        sprintf(tempMessage, "invalid character \"%s\"", token);
        printError(linePos, tempMessage);
        return "<TOKEN-ERROR>";
    }

    // Token starts with a number
    if (tokenSize > 1 && isNumber(token[0]))
    {
        for (i = 1; i < tokenSize; i++)
        {
            if (isNumber(token[i]))
            {
                if (countPoints > 0)
                {
                    if (countDecimals < 5)
                    {
                        countDecimals++;
                    }
                    else
                    {
                        sprintf(tempMessage, "too many decimals in float number \"%s\"", token);
                        printError(linePos, tempMessage);
                        return "<TOKEN-ERROR>";
                    }
                }
            }
            else
            {
                if (token[i] == '.')
                {
                    if (countPoints == 0)
                    {
                        countPoints++;
                    }
                    else
                    {
                        sprintf(tempMessage, "more than one decimal point \"%s\"", token);
                        printError(linePos, tempMessage);
                        return "<TOKEN-ERROR>";
                    }
                }
                else
                {
                    sprintf(tempMessage, "this is not a number \"%s\"", token);
                    printError(linePos, tempMessage);
                    return "<TOKEN-ERROR>";
                }
            }
        }
        if (countPoints == 0)
        {
            return "inum";
        }
        else
        {
            return "fnum";
        }
    }
    sprintf(tempMessage, "invalid variable name \"%s\"", token);
    printError(linePos, tempMessage);
    return "<TOKEN-ERROR>";
}

int main(int argc, char *argv[])
{
    FILE *fp, *outputFile;
    char buf[bufSize];
    char *found, *tempLine;
    int countTokens, linePos;

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
    outputFile = fopen(FILE_NAME, "w");
    linePos = 0;
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        size_t lineLength = strlen(buf) - 1;
        buf[lineLength] = '\0';
        tempLine = strdup(buf);
        countTokens = 0;
        linePos++;
        while ((found = strsep(&tempLine, " ")) != NULL)
        {
            // Line comment (ignore the rest of the line)
            if (found[0] == '/' && found[1] == '/')
                break;

            switch (found[0])
            {
            case 'p':
                fprintf(outputFile, "print ");
                break;
            case 'f':
                fprintf(outputFile, "floatdcl ");
                break;
            case 'i':
                fprintf(outputFile, "intdcl ");
                break;
            case '=':
                fprintf(outputFile, "assign ");
                break;
            case '+':
                fprintf(outputFile, "plus ");
                break;
            case '-':
                fprintf(outputFile, "minus ");
                break;
            case '/':
                fprintf(outputFile, "div ");
                break;
            case '*':
                fprintf(outputFile, "mult ");
                break;
            default:
                fprintf(outputFile, "%s ", getToken(linePos, found));
                break;
            }
            countTokens++;
        }
        if (countTokens > 0)
            fprintf(outputFile, "\n");
    }
    fclose(fp);
    fclose(outputFile);
    return 0;
}