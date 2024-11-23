/* $Id$ */

%{
#include "y.tab.h"
#include "parse.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char* yytmpbuf;

int yywrap(void){
	return 1;
}

int yyerror(const char* str){
	extern char* yytext;
	fprintf(stderr, "Syntax error near `%s': `%s'\n", yytext, str);
	return 0;
}
%}

%x STR

%%

\" {
	BEGIN(STR);
	yytmpbuf = malloc(1);
	yytmpbuf[0] = 0;
}

<STR>[^\\"]* {
	char* old = yytmpbuf;
	yytmpbuf = malloc(strlen(old) + strlen(yytext) + 1);
	strcpy(yytmpbuf, old);
	strcpy(yytmpbuf + strlen(old), yytext);
	free(old);
}

<STR>\\. {
	char* old = yytmpbuf;
	yytmpbuf = malloc(strlen(old) + 2);
	strcpy(yytmpbuf, old);
	strcpy(yytmpbuf + strlen(old), yytext + 1);
	free(old);
}

<STR>\" {
	BEGIN(0);
	yylval.scalar.value = yytmpbuf;
	return STRING;
}

([ \t]*#[^\n]+) ;

([ \t]*\n[ \t]*) {
	return NEWLINE;
}

option {
	return OPTION;
}

target {
	return TARGET;
}

controller {
	return CONTROLLER;
}

([ \t]+) {
	return SPACES;
}

([A-Za-z0-9_=]+) {
	yylval.scalar.value = malloc(strlen(yytext) + 1);
	strcpy(yylval.scalar.value, yytext);
	return STRING;
}

. {
	return yytext[0];
}

%%
