#include "builder.h"

BuilderProgram* Builder_NewProgram(void) {
	BuilderProgram* ret = (BuilderProgram*) malloc(sizeof(BuilderProgram));

	if (ret == NULL) {
		return NULL;
	}

	ret->program = NULL;
	ret->size    = 0;

	return ret;
}

void Builder_FreeProgram(BuilderProgram* program) {
	if (ret->program != NULL) {
		free(ret->program);
	}

	free(program);
}

static void ExtendProgram(BuilderProgram* program, size_t by) {
	if (program->program == NULL) {
		program->program = (uint8_t*) malloc(by);
		program->size    = by;
	}
	else {
		program->size    += by;
		program->program  = (uint8_t*) realloc(program->program, program->size);
	}
}

void Builder_AddInstruction(BuilderProgram* program, uint8_t inst) {
	ExtendProgram(program, 1);
	program->program[program->size - 1] = inst;
}

void Builder_AddByteParameter(BuilderProgram* program, uint8_t param) {
	ExtendProgram(program, 1);
	program->program[program->size - 1] = param;
}

void Builder_AddWordParameter(BuilderProgram* program, uint16_t param) {
	ExtendProgram(program, 2);
	program->program[program->size - 2] = param & 0xFF;
	program->program[program->size - 1] = (param & 0xFF00) >> 8;
}
