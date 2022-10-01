%{
/**********************************
forth.y
YACC file
Date: 2022/10/01
zzekun <2014074@mail.nankai.edu.cn>
***********************************/
#include <iostream>
#include <map>
#include <string>
using namespace std;

int yylex();
extern int yyparse();
FILE* yyin;
void yyerror(const char* s);
char idStr[50];
map<string, double>character_table;
%}

%union  {
    double  dval;
    char*  string;
}

%token <string> ID
%token <dval> NUMBER
%token ASSIGN
%token ADD
%token SUB
%token MUL
%token DIV
%token LEFT_PRA
%token RIGHT_PRA

%left ADD SUB
%left MUL DIV
%right UMINUS
%right ASSIGN

%type <dval> expr

%%

statement_list: statement ';'
        |       statement_list statement ';'
statement:      ID ASSIGN expr     { character_table[$1] = $3; }
        |       expr              { printf("%f\n",$1); }
        ;

expr : expr ADD expr { $$ = $1 + $3; }
	| expr SUB expr { $$ = $1 - $3; }
	| expr MUL expr { $$ = $1 * $3; }
	| expr DIV expr { if($3==0.0)
                        yyerror("divided by zero.");
                      else
                        $$ = $1 / $3;
                    }
	| LEFT_PRA expr RIGHT_PRA { $$ = $2; }
	| SUB expr %prec UMINUS { $$ = -$2; }
	| NUMBER { $$ = $1; }
    | ID { $$ = character_table[$1]; }
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
        else if(isdigit(t)){
            yylval.dval = 0;
            while(isdigit(t)){
                yylval.dval = yylval.dval * 10 + t -'0';
                t = getchar();
            }
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
            yylval.string = idStr;
            ungetc(t, stdin);
            //printf("%f\n", character_table[yylval.string]);
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
            case '=':
                return ASSIGN;
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