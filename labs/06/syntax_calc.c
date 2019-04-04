#include <stdio.h>
#include <string.h>
#define bufSize 1024
#define PLUS "plus"
#define MINUS "minus"
#define MULT "mult"
#define DIV "div"
#define PRINT "print"
#define ASSIGN "assign"
#define S_COUNTER 0
#define Sl_COUNTER 1
#define A_COUNTER 2
#define Al_COUNTER 3
#define B_COUNTER 4
#define Bl_COUNTER 5
#define C_COUNTER 6
#define Cl_COUNTER 7
#define D_COUNTER 8
#define id_COUNTER 9
#define inum_COUNTER 10
#define fnum_COUNTER 11

char *source;
int cursor;
char stack[50][50];
int stackPos;
int counters[12];

char getNextToken();
int match(char c);
int Id();
int Print();
int Plus();
int Minus();
int Mult();
int Div();
int INum();
int FNum();
int IntDcl();
int FloatDcl();
int Assign();
int FIntDcl();
int FFloatDcl();
int FPrint();
int FAssign();
int S();
int Sl();
int A();
int Al();
int B();
int Bl();
int C();
int Cl();
int D();
int Analyzer();
int Parse(char *s);

int main(int argc, char *argv[])
{
    FILE *fp;
    char buf[bufSize], *tempLine;
    int res, line;
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
    }

    line = 1;
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        size_t lineLength = strlen(buf) - 1;
        buf[lineLength] = '\0';
        if (!Parse(buf))
        {
            printf("Error in line %d:\n%s\n\n", line, buf);
        }
        printf("\n");
        line++;
    }

    fclose(fp);
    return 0;
}
char getNextToken()
{
    // Skip whitespace.
    while (source[cursor] == ' ')
        cursor++;
    char nextToken = source[cursor];
    cursor++;
    return nextToken;
}
int match(char c)
{
    return getNextToken() == c;
}
int Id()
{
    return match('i') && match('d');
}
int Print()
{
    return match('p') && match('r') && match('i') && match('n') &&
           match('t');
}
int Plus()
{
    return match('p') && match('l') && match('u') && match('s');
}
int Minus()
{
    return match('m') && match('i') && match('n') && match('u') && match('s');
}
int Mult()
{
    return match('m') && match('u') && match('l') && match('t');
}
int Div()
{
    return match('d') && match('i') && match('v');
}
int INum()
{
    return match('i') && match('n') && match('u') && match('m');
}
int FNum()
{
    return match('f') && match('n') && match('u') && match('m');
}
int IntDcl()
{
    return match('i') && match('n') && match('t') &&
           match('d') && match('c') && match('l');
}
int FloatDcl()
{
    return match('f') && match('l') && match('o') &&
           match('a') && match('t') && match('d') && match('c') &&
           match('l');
}
int Assign()
{
    return match('a') && match('s') && match('s') &&
           match('i') && match('g') && match('n');
}
int FIntDcl()
{
    int temp = IntDcl() && Id();
    if (temp)
        printf("\tstart -> {intdcl, id}\n");
    return temp;
}
int FFloatDcl()
{
    int temp = FloatDcl() && Id();
    if (temp)
        printf("\tstart -> {floatdcl, id}\n");
    return temp;
}
int FPrint()
{
    int temp = Print() && Id();
    if (temp)
        printf("\tstart -> {print, id}\n");
    return temp;
}
int FAssign()
{
    int temp = Id() && Assign() && S();
    if (temp)
        printf("\tstart -> {id, assign, S_0}\n");
    return temp;
}
int S()
{
    char t[60];
    sprintf(t, "S_%d -> {A_%d, Sl_%d}", counters[S_COUNTER]++, counters[A_COUNTER], counters[Sl_COUNTER]);

    int temp = A() && Sl();
    if (temp)
    {
        strcpy(stack[stackPos], t);
        stackPos++;
    }
    return temp;
}
int Sl()
{
    char t[60];
    sprintf(t, "Sl_%d -> {plus, A_%d, Sl_%d}", counters[Sl_COUNTER], counters[A_COUNTER], counters[Sl_COUNTER] + 1);
    counters[Sl_COUNTER]++;

    int savedCursor = cursor;
    int temp = (cursor = savedCursor, Plus() && A() && Sl());
    if (temp)
    {
        strcpy(stack[stackPos], t);
        stackPos++;
    }
    else
    {
        sprintf(t, "Sl_%d -> {plus, A_%d, Sl_%d}", counters[Sl_COUNTER], counters[A_COUNTER], counters[Sl_COUNTER] + 1);
    }

    return temp || (cursor = savedCursor, 1);
}
int A()
{
    char t[60];
    sprintf(t, "A_%d -> {B_%d, Al_%d}", counters[A_COUNTER]++, counters[B_COUNTER], counters[Al_COUNTER]);

    int temp = B() && Al();
    if (temp)
    {
        strcpy(stack[stackPos], t);
        stackPos++;
    }
    return temp;
}
int Al()
{
    char t[60];
    sprintf(t, "Al_%d -> {minus, B_%d, Al_%d}", counters[Al_COUNTER], counters[B_COUNTER], counters[Al_COUNTER] + 1);
    counters[Al_COUNTER]++;

    int savedCursor = cursor;
    int temp = (cursor = savedCursor, Minus() && B() && Al());
    if (temp)
    {
        strcpy(stack[stackPos], t);
        stackPos++;
    }
    return temp || (cursor = savedCursor, 1);
}
int B()
{
    char t[60];
    sprintf(t, "B_%d -> {C_%d, Bl_%d}", counters[B_COUNTER]++, counters[C_COUNTER], counters[Bl_COUNTER]);

    int temp = C() && Bl();
    if (temp)
    {
        strcpy(stack[stackPos], t);
        stackPos++;
    }
    return temp;
}
int Bl()
{
    char t[60];
    sprintf(t, "Bl_%d -> {mult, C_%d, Bl_%d}", counters[Bl_COUNTER], counters[C_COUNTER], counters[Bl_COUNTER] + 1);
    counters[Bl_COUNTER]++;

    int savedCursor = cursor;
    int temp = (cursor = savedCursor, Mult() && C() && Bl());
    if (temp)
    {
        strcpy(stack[stackPos], t);
        stackPos++;
    }
    return temp || (cursor = savedCursor, 1);
}
int C()
{
    char t[60];
    sprintf(t, "C_%d -> {D_%d, Cl_%d}", counters[C_COUNTER]++, counters[D_COUNTER], counters[Cl_COUNTER]);

    int temp = D() && Cl();
    if (temp)
    {
        strcpy(stack[stackPos], t);
        stackPos++;
    }
    return temp;
}
int Cl()
{
    char t[60];
    sprintf(t, "Cl_%d -> {div, D_%d, Cl_%d}", counters[Cl_COUNTER], counters[D_COUNTER], counters[Cl_COUNTER] + 1);
    counters[Cl_COUNTER]++;

    int savedCursor = cursor;
    int temp = (cursor = savedCursor, Div() && D() && Cl());
    if (temp)
    {
        strcpy(stack[stackPos], t);
        stackPos++;
    }
    return temp || (cursor = savedCursor, 1);
}
int D()
{
    int savedCursor = cursor;
    int selected = 0;
    int temp = (cursor = savedCursor, selected = 0, Id()) ||
               (cursor = savedCursor, selected = 1, INum()) ||
               (cursor = savedCursor, selected = 2, FNum());
    if (temp)
    {
        switch (selected)
        {
        case 0:
            sprintf(stack[stackPos], "D_%d -> id_%d", counters[D_COUNTER]++, counters[id_COUNTER]++);
            break;
        case 1:
            sprintf(stack[stackPos], "D_%d -> inum_%d", counters[D_COUNTER]++, counters[inum_COUNTER]++);
            break;
        case 2:
            sprintf(stack[stackPos], "D_%d -> fnum_%d", counters[D_COUNTER]++, counters[fnum_COUNTER]++);
            break;
        default:
            printf("ERROR");
            break;
        }
        stackPos++;
    }
    return temp;
}
int Analyzer()
{
    int savedCursor = cursor;
    return (cursor = savedCursor, FIntDcl()) ||
           (cursor = savedCursor, FFloatDcl()) ||
           (cursor = savedCursor, FPrint()) ||
           (cursor = savedCursor, FAssign());
}
int Parse(char *s)
{
    source = s;
    cursor = 0;
    stackPos = 0;
    int i;
    for (i = 0; i < 12; i++)
    {
        counters[i] = 0;
    }
    printf("(%s):\n", s);
    printf("digraph D {\n");
    int temp = Analyzer() && cursor == strlen(source);
    if (temp && stackPos > 0)
    {
        for (i = stackPos - 1; i >= 0; i--)
        {
            printf("\t%s\n", stack[i]);
        }
    }
    printf("}\n");
    return temp;
}