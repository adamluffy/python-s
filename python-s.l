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

"#".*					{ /*printf("Eat up comment at line %d\n", lineno);*/ } 

"'''"					{ /*printf("Eat up comment from line %d ", lineno);*/ BEGIN(ML_COMMENT); }
<ML_COMMENT>"'''" 		{ /*printf("to line %d\n", lineno);*/ BEGIN(INITIAL); }
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
"in"			{ yylval.val.ival = IN_OP; return IN; }
"void"			{ return VOID; }
"def"			{ return DEFINE; }
"return"		{ return RETURN; }


"+"				{ yylval.val.ival = ADD; return ADDOP; }
"-"				{ yylval.val.ival = SUB; return ADDOP; }
"++"			{ yylval.val.ival = INC; return INCR; }
"--"			{ yylval.val.ival = DEC; return INCR; }
"*"				{ return MULOP; }
"/"				{ return DIVOP; }
"or"			{ return OROP; }
"and"			{ return ANDOP; }
"not"			{ return NOTOP; }
"=="			{ yylval.val.ival = EQUAL; return EQUOP; }
"!="			{ yylval.val.ival = NOT_EQUAL; return EQUOP; }
">"				{ yylval.val.ival = GREATER; return RELOP; }
"<"				{ yylval.val.ival = LESS; return RELOP; }
">="			{ yylval.val.ival = GREATER_EQUAL; return RELOP; }
"<="			{ yylval.val.ival = LESS_EQUAL; return RELOP; }


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
{ICONST} 		{ yylval.val.ival = atoi(yytext); return ICONST; }
{FCONST} 		{ yylval.val.fval = atof(yytext); return FCONST; }
{CCONST} 		{ yylval.val.cval = yytext[1]; 
					return CCONST; }
{STRING} 		{ 
					yylval.val.sval = malloc(yyleng*sizeof(char));
					strcpy(yylval.val.sval, yytext);
					return STRING; 
				}


"\n"			{ lineno += 1;}
[ \t\r\f]+			/* eat up whitespace */

.				{ yyerror("Unrecognized character"); }

%%
