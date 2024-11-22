/* $Id$ */

%{
#include "y.tab.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>

int have_program(const char* str);
int yylex(void);
int yyerror(const char*);

#define DEFPATH "/bin:/sbin:/usr/bin:/usr/sbin"
%}

%union {
	struct {
		char* value;
	} scalar;
}

%start statement_list
%token NEWLINE STRING IDENT SPACES OPTION TARGET

%%

statement_list	:	statement
		|	statement_list statement;

statement	:	option NEWLINE
		|	target NEWLINE
		|	NEWLINE;
	
option		:	OPTION SPACES STRING {
	printf("option: %s\n", $<scalar.value>3);
};

target		:	TARGET SPACES STRING {
	struct utsname uts;
	uname(&uts);
	printf("target: %s\n", $<scalar.value>3);
	printf("host: %s\n", uts.machine);
	if(strcmp($<scalar.value>3, uts.machine) == 0){
		printf("target == host, using cc/gcc\n");
	}else if((strcmp("amd64", uts.machine) == 0 || strcmp("x86_64", uts.machine) == 0) && strcmp("i386", $<scalar.value>3) == 0){
		printf("target == 32 bit version of host, using cc/gcc with -m32\n");
	}else{
		printf("using cross compiler\n");
	}
};

%%

int have_program(const char* str){
	char* path = getenv("PATH");
	char* buf;
	char* p;
	char* old;
	int st = 0;
	if(path == NULL){
		buf = malloc(strlen(DEFPATH) + 1);
		strcpy(buf, DEFPATH);
	}else{
		buf = malloc(strlen(path) + 1);
		strcpy(buf, path);
	}
	old = buf;
	while((p = strtok(old, ":")) != NULL){
		char* name = malloc(strlen(p) + 1 + strlen(str) + 1);
		strcpy(name, p);
		name[strlen(p)] = '/';
		strcpy(name + strlen(p) + 1, str);
		old = NULL;
		st = access(name, F_OK) == 0;
		free(name);
		if(st) break;
	}
	free(buf);
	return st;
}
