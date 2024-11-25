/* $Id$ */

#include <c/string.h>
#include <c/memory.h>

int strlen(const char* str){
	int i;
	for(i = 0; str[i] != 0; i++);
	return i;
}

bool strequ(const char* str1, const char* str2){
	int i;
	if(strlen(str1) != strlen(str2)) return false;
	for(i = 0; str1[i] != 0; i++){
		if(str1[i] != str2[i]) return false;
	}
	return true;
}

void strcpy(char* dest, const char* src){
	memcpy(dest, src, strlen(src));
	dest[strlen(src)] = 0;
}

void strcat(char* dest, const char* src){
	strcpy(dest + strlen(dest), src);
}

void numstr(char* dest, uint64_t num){
	char buf[513];
	int i;
	buf[512] = 0;
	for(i = 511; i >= 0; i--){
		buf[i] = '0' + (num % 10);
		num /= 10;
		if(num == 0){
			strcpy(dest, buf + i);
			break;
		}
	}
}
