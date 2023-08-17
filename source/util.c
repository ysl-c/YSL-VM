#include "util.h"

char* DupString(const char* str) {
	char* ret = malloc(strlen(str) + 1);

	if (ret == NULL) {
		return NULL;
	}
	
	strcpy(ret, str);

	return ret;
}
