%{
/**********************************
third.y
YACC file
Date: 2022/10/01
zzekun <2014074@mail.nankai.edu.cn>
***********************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#ifndef YYSTYPE
#define YYSTYPE char*
#endif
char idStr[50];
char numStr[50];
int yylex();
extern int yyparse();
FILE* yyin;
void yyerror(const char* s);
%}

%token NUMBER
%token ID
%token ADD
%token SUB
%token MUL
%token DIV
%token LEFT_PRA
%token RIGHT_PRA
%left ADD SUB
%left MUL DIV
%right UMINUS

%%

lines : lines expr ';' { printf("%s\n", $2); }
	| lines ';'
	|
	;

expr : expr ADD expr { $$ = (char *)malloc(50*sizeof (char)); 
                       strcpy($$,$1);
                       strcat($$,$3); 
                       strcat($$,"+ "); }
	| expr SUB expr { $$ = (char *)malloc(50*sizeof (char)); 
                      strcpy($$,$1);
                      strcat($$,$3); 
                      strcat($$,"- "); }
	| expr MUL expr { $$ = (char *)malloc(50*sizeof (char)); 
                      strcpy($$,$1);
                      strcat($$,$3);
                      strcat($$,"* "); }
	| expr DIV expr { $$ = (char *)malloc(50*sizeof (char)); 
                      strcpy($$,$1);
                      strcat($$,$3);
                      strcat($$,"/ "); }
	| LEFT_PRA expr RIGHT_PRA { $$ = (char *)malloc(50*sizeof (char)); 
                                strcpy($$,$2); }
	| SUB expr %prec UMINUS { $$ = (char *)malloc(50*sizeof (char));
                              strcpy($$,"-");
                              strcat($$,$2); }
	| NUMBER { $$ = (char *)malloc(50*sizeof (char));
               strcpy($$, $1); 
               strcat($$," "); }
    | ID { $$ = (char *)malloc(50*sizeof (char));
           strcpy($$, $1); 
           strcat($$," "); }
	;

%%

// programs section

int yylex()
{
    // place your token retrieving code here
    int t;
    while(1)
    {
        t = getchar();
        if(t == ' ' || t == '\t' || t == '\n')
            ;
        else if(( t >= '0' && t <= '9' )){
            int i = 0;
            while(( t >= '0' && t <= '9' )){
                numStr[i] = t;
                t = getchar();
                i++;
            }
            numStr[i] = '\0';
            yylval = numStr;
            ungetc(t, stdin);
            return NUMBER;
        }
        else if(( t >= 'a' && t <= 'z' ) || ( t >= 'A' && t<= 'Z' ) || ( t == '_' )){
            int i = 0;
            while(( t >= 'a' && t <= 'z' ) || ( t >= 'A' && t<= 'Z' ) || ( t == '_' ) || ( t >= '0' && t <= '9' )){
                idStr[i] = t;
                t = getchar();
                i++;
            }
            idStr[i] = '\0';
            yylval = idStr;
            ungetc(t, stdin);
            return ID;
        }
        else{
            switch (t)
            {
            case '+':
                return ADD;
                break;
            case '-':
                return SUB;
                break;
            case '*':
                return MUL;
                break;
            case '/':
                return DIV;
                break;
            case '(':
                return LEFT_PRA;
                break;
            case ')':
                return RIGHT_PRA;
                break;
            default:
                return t;
            }
        }
    }
}

int main(void)
{
    yyin = stdin ;
    do {
        yyparse();
    } 
    while (! feof (yyin));
    return 0;
}
void yyerror(const char* s) {
    fprintf (stderr , "Parse error : %s\n", s );
    exit (1);
}
