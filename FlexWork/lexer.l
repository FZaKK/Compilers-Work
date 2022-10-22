%option noyywrap
%{
    #define ONLY_FOR_LEX
    
    #ifdef ONLY_FOR_LEX
    #include <ostream>
    #include <fstream>
    #include <iostream>
    #include <map>
    using namespace std;
    extern FILE *yyin; 
    extern FILE *yyout;

    int columnno = 0;
    string tab = "\t";
    map<string, float> float_table;
    map<string, int> int_table;

    void DEBUG_FOR_LAB4(string s){
        string DEBUG_INFO = "\t" + s + "\t" + to_string(yylineno) + "\t" + to_string(columnno) + "\n";
        fputs(DEBUG_INFO.c_str(), yyout);
    }

    void DEBUG_FOR_INT_LAB4(string s, int num){
        string DEBUG_INFO = "\t" + s + "\t" + to_string(yylineno) + "\t" + to_string(columnno) + "\t" + to_string(num) + "\n";
        fputs(DEBUG_INFO.c_str(), yyout);
    }

    void DEBUG_FOR_FLOAT_LAB4(string s, float num){
        string DEBUG_INFO = "\t" + s + "\t" + to_string(yylineno) + "\t" + to_string(columnno) + "\t" + to_string(num) + "\n";
        fputs(DEBUG_INFO.c_str(), yyout);
    }
    
    #else
    #define YYSTYPE void *
    #include "parser.h"
    
    #endif

    #define YY_NO_UNPUT
    #define YY_NO_INPUT
    #include <string>
%}

DECIMIAL ([1-9][0-9]*|0)
FLOATDECIMAL ([1-9][0-9]*|0)("."[0-9]*)?(("E"|"e")("+"|"-")?[0-9]+)?
OCTAL ([0])([0-7]+)
HEXDECIMIAL ("0x"|"0X")([0-9]|[a-f]|[A-F])+
ID [[:alpha:]_][[:alpha:][:digit:]_]*
EOL (\r\n|\n|\r) 
WHITE [\t|" "]

LINECOMMENT \/\/[^\n]*
BLOCKCOMMENT_BEGIN "/*"
BLOCKCOMMENT_ELEMENT .|EOL
BLOCKCOMMENT_END "*/"
%x BLOCKCOMMENT

%%

"int" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("INT\t int");
        columnno += 3;
    #else
        return INT;
    #endif
}
"float" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("FLOAT\t float");
        columnno += 5;
    #else
        return FLOAT;
    #endif
}
"void" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("VOID\t void");
        columnno += 4;
    #else
        return VOID;
    #endif 
}
"if" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("IF\t if");
        columnno += 2;
    #else
        return IF;
    #endif
};
"else" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("ELSE\t else");
        columnno += 4;
    #else
        return ELSE;
    #endif
};
"return" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("RETURN\t return");
        columnno += 6;
    #else
        return RETURN;
    #endif
}
"while" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("WHILE\t while");
        columnno += 5;
    #else
        return WHILE;
    #endif
}
"for" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("FOR\t for");
        columnno += 3;
    #else
        return FOR;
    #endif
}
"break" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("BREAK\t break");
        columnno += 5;
    #else
        return BREAK;
    #endif
}
"continue" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("CONTINUE\t continue");
        columnno += 8;
    #else
        return CONTINUE;
    #endif
}
"=" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("ASSIGN\t=");
        columnno += 1;
    #else
        return ASSIGN;
    #endif
}
"&&" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("AND\t&&");
        columnno += 2;
    #else
        return AND;
    #endif
}
"||" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("OR\t||");
        columnno += 2;
    #else
        return OR;
    #endif    
}
"!=" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("NOTEQUAL\t!=");
        columnno += 2;
    #else
        return NOTEQUAL;
    #endif
}
"!" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("NON\t!");
        columnno += 1;
    #else
        return NON;
    #endif
}
"==" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("EQUAL\t==");
        columnno += 2;
    #else
        return EQUAL;
    #endif
}
">" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("GREATER\t>");
        columnno += 1;
    #else
        return GREATER;
    #endif
}
"<" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("LESS\t<");
        columnno += 1;
    #else
        return LESS;
    #endif
}
">=" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("GORE\t>=");
        columnno += 2;
    #else
        return GORE;
    #endif
}
"<=" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("LORE\t<=");
        columnno += 2;
    #else
        return LORE;
    #endif
}
"+" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("ADD\t+");
        columnno += 1;
    #else
        return ADD;
    #endif
}
"-" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("SUB\t-");
        columnno += 1;
    #else
        return SUB;
    #endif
}
"*" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("MUL\t*");
        columnno += 1;
    #else
        return MUL;
    #endif
}
"/" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("DIV\t/");
        columnno += 1;
    #else
        return DIV;
    #endif
}
"%" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("MOD\t%");
        columnno += 1;
    #else
        return MOD;
    #endif
}
";" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("SEMICOLON\t;");
        columnno += 1;
    #else
        return SEMICOLON;
    #endif
}
"(" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("LPAREN\t(");
        columnno += 1;
    #else
        return LPAREN;
    #endif
}
")" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("RPAREN\t)");
        columnno += 1;
    #else
    return RPAREN;
    #endif
}
"[" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("LBRACK\t[");
        columnno += 1;
    #else
        return LBRACK;
    #endif
}
"]" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("RBRACK\t]");
        columnno += 1;
    #else
        return RBRACK;
    #endif
}
"{" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("LBRACE\t{");
        columnno += 1;
    #else
        return LBRACE;
    #endif
}
"}" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("RBRACE\t}");
        columnno += 1;
    #else
        return RBRACE;
    #endif
}

{LINECOMMENT} {;}
{BLOCKCOMMENT_BEGIN} {BEGIN BLOCKCOMMENT;}
<BLOCKCOMMENT>{BLOCKCOMMENT_ELEMENT} {}
<BLOCKCOMMENT>{EOL} {yylineno++;}
<BLOCKCOMMENT>{BLOCKCOMMENT_END} {BEGIN INITIAL;}

{DECIMIAL} {
            #ifdef ONLY_FOR_LEX
                string temp = yytext;
                int_table[temp] = stoi(temp);
                DEBUG_FOR_INT_LAB4("DECIMIAL\t" + temp, int_table[temp]);
                // cout << int_table[temp] << " " << typeid(int_table[temp]).name() << endl; // 确定输出为int类型
                columnno += strlen(yytext);
            #else
                return DECIMIAL;
            #endif
            }
{FLOATDECIMAL} {
            #ifdef ONLY_FOR_LEX
                string temp = yytext;
                float_table[temp] = stof(temp);
                DEBUG_FOR_FLOAT_LAB4("FLOATDECIMAL\t" + temp, float_table[temp]);
                columnno += strlen(yytext);
            #else
                return FLOATDECIMAL;
            #endif
            }
{OCTAL} {
            #ifdef ONLY_FOR_LEX
                string temp = yytext;
                int_table[temp] = stoi(temp, 0, 8);
                DEBUG_FOR_INT_LAB4("OCTAL\t" + temp, int_table[temp]);
                columnno += strlen(yytext);
            #else
                return OCTAL;
            #endif
    }
{HEXDECIMIAL} {
            #ifdef ONLY_FOR_LEX
                string temp = yytext;
                int_table[temp] = stoi(temp, 0, 16);
                DEBUG_FOR_INT_LAB4("HEXDECIMIAL\t" + temp, int_table[temp]);
                columnno += strlen(yytext);
            #else
                return HEXDECIMIAL;
            #endif
    }
{ID} {columnno += strlen(yytext);}
{EOL} { 
      yylineno++; 
      columnno = 0; 
    }
{WHITE} {columnno++;}



%%

#ifdef ONLY_FOR_LEX
int main(int argc, char **argv){
    if(argc != 5){
        fprintf(stderr, "Argument Not Enough");
        exit(EXIT_FAILURE);
    }

    if(!(yyin = fopen(argv[1], "r"))){
        fprintf(stderr, "No such file or directory: %s", argv[1]);
        exit(EXIT_FAILURE);
    }

    if(!(yyout = fopen(argv[3], "w"))){
        fprintf(stderr, "No such file or directory: %s", argv[3]);
        exit(EXIT_FAILURE);
    }
    string Frame_Info = tab + "1:token" + " " + "2:lexme" + " " + "3:lineno" + " " + "4:columnno" + " " + "5:attribute" + "\n" + "\n" + "\n";
    fputs(Frame_Info.c_str(), yyout);

    yylex();

    /*
    for(auto it : int_table){
	    cout << it.first <<" "<< it.second <<endl;
    }

    for(auto it : float_table){
	    cout << it.first <<" "<< it.second <<endl;
    }
    */

    return 0;
}
#endif
