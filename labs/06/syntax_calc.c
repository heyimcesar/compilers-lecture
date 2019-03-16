#include <stdio.h>
#include <string.h>
#define bufSize 1024
#define PLUS "plus"
#define MINUS "minus"
#define MULT "mult"
#define DIV "div"
#define PRINT "print"
#define ASSIGN "assign"

char *source;
int cursor;

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
    //char ch;
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
    }

    /*while ((ch = fgetc(fp)) != EOF)
        printf("%c", ch);*/
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
    return IntDcl() && Id();
}
int FFloatDcl()
{
    return FloatDcl() && Id();
}
int FPrint()
{
    return Print() && Id();
}
int FAssign()
{
    return Id() && Assign() && S();
}
int S()
{
    int res = A() && Sl();
    if(res) printf("S");
    return res;
}
int Sl()
{
    int savedCursor = cursor;
    int res = (cursor = savedCursor, Plus() && A() && Sl());
    if(res) printf("Sl");
    return res || (cursor = savedCursor, 1);
}
int A()
{
    int res = B() && Al();
    if(res) printf("A");
    return res;
}
int Al()
{
    int savedCursor = cursor;
    int res = (cursor = savedCursor, Minus() && B() && Al());
    if(res) printf("Al");
    return res || (cursor = savedCursor, 1);
}
int B()
{
    int res = C() && Bl();
    if(res) printf("B");
    return res;
}
int Bl()
{
    int savedCursor = cursor;
    int res = (cursor = savedCursor, Mult() && C() && Bl());
    if(res) printf("Bl");
    return res || (cursor = savedCursor, 1);
}
int C()
{
    int res = D() && Cl();
    if(res) printf("C");
    return res;
}
int Cl()
{
    int savedCursor = cursor;
    int res = (cursor = savedCursor, Div() && D() && Cl());
    if(res) printf("Cl");
    return res || (cursor = savedCursor, 1);
}
int D()
{
    int savedCursor = cursor;
    int res = (cursor = savedCursor, Id()) ||
    (cursor = savedCursor, INum()) ||
    (cursor = savedCursor, FNum());
    if(res) printf("D");
    return res;
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

    return Analyzer() && cursor == strlen(source);
}