/* $Id$ */

#include <stdio.h>
#include <string.h>

extern FILE* yyin;
extern int yyparse(void);

int main(int argc, char** argv){
	if(argc != 2){
		fprintf(stderr, "usage: %s file\n", argv[0]);
		return 1;
	}
	yyin = fopen(argv[1], "r");
	if(yyin == NULL){
		fprintf(stderr, "could not open the file\n");
		return 1;
	}
	if(yyparse()){
		return 1;
	}
	fclose(yyin);
	return 0;
}
