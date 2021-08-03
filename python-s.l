%option noyywrap

%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "symtab.h"
	#include "semantic.h"
	#include "python-s.tab.h"
	extern FILE *yyin;
	extern FILE *yyout;
	
	int lineno = 1; // initialize to 1
	void ret_print(char *token_type);
	void yyerror();
%}

%x ML_COMMENT

alpha 		[a-zA-Z]
digit 		[0-9]
alnum 		{alpha}|{digit}
print		[ -~]

ID 			{alpha}+{alnum}*
ICONST		"0"|[0-9]{digit}*
FCONST		"0"|{digit}*"."{digit}+
CCONST		(\'{print}\')|(\'\\[nftrbv]\')
STRING		\"{print}*\"

%%

"#".*					{ printf("Eat up comment at line %d\n", lineno); } 

"'''"					{ printf("Eat up comment from line %d ", lineno); BEGIN(ML_COMMENT); }
<ML_COMMENT>"'''" 		{ printf("to line %d\n", lineno); BEGIN(INITIAL); }
<ML_COMMENT>[^*\n]+		
<ML_COMMENT>"*"			
<ML_COMMENT>"\n"		{ lineno += 1; }


"char"       	{ return CHAR; }
"int"			{ return INT; }
"float"			{ return FLOAT; }
"double"		{ return DOUBLE; }
"bool"			{ return BOOL; }
"if"			{ return IF; }
"else"			{ return ELSE; }
"elif"			{ return ELIF; }
"when"			{ return WHEN; }
"while"			{ return WHILE; }
"for"			{ return FOR; }
"continue"		{ return CONTINUE; }
"break"			{ return BREAK; }
"in"			{ return IN; }
"void"			{ return VOID; }
"def"			{ return DEFINE; }
"return"		{ return RETURN; }


"+"|"-"					{ return ADDOP; }
"++"|"--"				{ return INCR;}
"*"						{ return MULOP; }
"/"						{ return DIVOP; }
"or"					{ return OROP; }
"and"					{ return ANDOP; }
"not"						{ return NOTOP; }
"=="|"!="				{ return EQUOP; }
">"|"<"|">="|"<="		{ return RELOP; }


"("				{ return LPAREN; }
")"				{ return RPAREN; }
"]"				{ return RBRACK; }
"["				{ return LBRACK; }
"{"				{ return LBRACE; }
"}"				{ return RBRACE; }
":"				{ return COLON; }
";"				{ return SEMI; }
"."				{ return DOT; }
","				{ return COMMA; }
"="				{ return ASSIGN; }
"->"			{ return ARROW; }
".."			{ return RANGE; }
"=>"			{ return SHORTHAND; }


{ID} 			{
					// insert identifier into symbol table
					insert(yytext, strlen(yytext), UNDEF, lineno);
					yylval.symtab_item = lookup(yytext);
					return ID;
				}
{ICONST} 		{ yylval.ival = atoi(yytext); return ICONST; }
{FCONST} 		{ yylval.dval = atof(yytext); return FCONST; }
{CCONST} 		{ yylval.cval = yytext[0]; return CCONST; }
{STRING} 		{ 
					yylval.sval = malloc(yyleng*sizeof(char));
					strcpy(yylval.sval, yytext);
					return STRING; 
				}


"\n"			{ lineno += 1;}
[ \t\r\f]+			/* eat up whitespace */

.				{ yyerror("Unrecognized character"); }

%%
