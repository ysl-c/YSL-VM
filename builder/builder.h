#ifndef YSLVM_BUILDER_H
#define YSLVM_BUILDER_H

#include "../source/vm.h"

typedef struct BuilderProgram {
	uint8_t* program;
	size_t   size;
} BuilderProgram;

BuilderProgram* Builder_NewProgram(void);
void            Builder_FreeProgram(BuilderProgram* program);

void Builder_AddInstruction(BuilderProgram* program, uint8_t inst);
void Builder_AddByteParameter(BuilderProgram* program, uint8_t param);
void Builder_AddWordParameter(BuilderProgram* program, uint16_t param);

#endif
