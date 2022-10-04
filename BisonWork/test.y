%{
/**********************************
test.y
YACC file
Date: 2022/10/01
zzekun <2014074@mail.nankai.edu.cn>
***********************************/
#include <iostream>
#include <map>
#include <cstring>
using namespace std;

int yylex();
extern int yyparse();
FILE* yyin;
void yyerror(const char* s);
char idStr[50];
map<string, double>character_table;

struct Assemble{
	char addr[50];
	char code[200];
};
%}

%union  {
    double dval;
    char strval[50];
	Assemble Assemval;
}

%token <Assemval> ID
%token <dval> NUMBER
%type <Assemval> expr
%type <Assemval> statement
%type <Assemval> statement_list
%token ASSIGN
%token ADD
%token SUB
%token MUL
%token DIV
%token LEFT_PRA
%token RIGHT_PRA


%right ASSIGN
%left ADD SUB
%left MUL DIV
%right UMINUS

%%

statement_list: statement ';'     { strcpy($$.code, $1.code); 
									cout<<$$.code; }
        |       statement_list statement ';'   { strcpy($$.code, $1.code); 
												 strcat($$.code, '\n');
												 strcat($$.code, $2.code); 
												 cout<<$$.code; }
statement:      ID ASSIGN expr     { strcpy($$.code, $3.code);
									 strcat($$.code, "\nMOV EAX, "); 
									 strcat($$.code, $3.addr); 
									 strcat($$.code, "\nMOV ");
									 strcat($$.code, $1.addr);
									 strcat($$.code, ", EAX"); }
        |       expr              { cout<<$1<<endl; }  
        ;

expr : expr ADD expr { $$ = $1 + $3; }
	| expr SUB expr { $$ = $1 - $3; }
	| expr MUL expr { $$ = $1 * $3; }
	| expr DIV expr { if($3 == 0.0)
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
            strcpy(yylval.strval, idStr);
            cout<<yylval.strval<<endl;
            ungetc(t, stdin);
            //printf("%f\n", character_table[yylval.strval]);
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
