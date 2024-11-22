/* $Id$ */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

extern FILE* yyin;
extern int yyparse(void);

int main(int argc, char** argv){
	char buffer[PATH_MAX + 1 + 6 + 5 + 1];
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
	while(1){
		getcwd(buffer, PATH_MAX);
		if(strcmp(buffer, "/") == 0){
			fprintf(stderr, "README.ROOT not found, your distribution might be broken\n");
			return 1;
		}
		strcat(buffer, "/README.ROOT");
		if(access(buffer, F_OK) == 0){
			getcwd(buffer, PATH_MAX);
			break;
		}
		chdir("..");
	}
	printf("root directory: %s\n", buffer);
	return 0;
}
