#include "file.h"

uint8_t* ReadRawFile(const char* path, size_t* size) {
	FILE* file = fopen(path, "rb");

	if (file == NULL) {
		return NULL;
	}

	fseek(file, 0, SEEK_END);

	size_t fileSize = ftell(file);

	if (size != NULL) {
		*size = fileSize;
	}
	
	uint8_t* ret = (uint8_t*) malloc(fileSize);
	
	fseek(file, 0, SEEK_SET);

	char ch;
	for (int i = 0; (ch = getc(file)) != EOF; ++ i) {
		ret[i] = ch;
	}
	
	fclose(file);
	return ret;
}
