%{
#include <stdio.h>
typedef struct node
{
        struct node *left;
        struct node *right;
        char *token;
} node;
node *mknode(node *left, node *right, char *token);
void printtree(node *tree);
%}

%start lines
%token INUM FNUM FLOATDCL INTDCL PRINT ID ASSIGN
%token PLUS MINUS TIMES DIVIDE
%token END
%left PLUS MINUS
%left TIMES DIVIDE

%%
lines:  /* empty */
 |      lines line /* do nothing */

line:   FLOATDCL ID END { printf("Declared float variable %c\n", $2); }
 |      INTDCL ID END { printf("Declared int variable %c\n", $2); }
 |      PRINT ID END { printf("%d\n", $2); }
 |      ID ASSIGN S END { $$ = $3; printf("%d\n", $3);}
;

S:      S PLUS A {$$ = $1 + $3;}
 |      A {$$ = $1;}
 ;
A:      A MINUS B {$$ = $1 - $3;}
 |      B {$$ = $1;}
 ;
B:      B TIMES C {$$ = $1 * $3;}
 |      C {$$ = $1;}
 ;
C:      C DIVIDE D {$$ = $1 / $3;}
 |      D {$$ = $1;}
 ;
D:      ID {$$ = $1;}
 |      INUM {$$ = $1;}
 |      FNUM {$$ = $1;}
;
%%

int main(void)
{
        return yyparse();
}
node *mknode(node *left, node *right, char *token)
{
        node *newnode = (node *)malloc(sizeof(node));
        char *newstr = (char *)malloc(strlen(token)+1);
        strcpy(newstr, token);
        newnode->left = left;
        newnode->right = right;
        newnode->token = newstr;
        return(newnode);
}
void printtree(node *tree)
{
        int i;
        if (tree->left || tree->right)
                printf("(");
        printf(" %s ", tree->token);
        if (tree->left)
                printtree(tree->left);
        if (tree->right)
                printtree(tree->right);
        if (tree->left || tree->right)
                printf(")");
}
int yyerror(char *s)
{
        fprintf(stderr, "%s\n", s);
}