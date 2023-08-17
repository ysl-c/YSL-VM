#include "vm.h"
#include "file.h"
#include "components.h"

int main(int argc, const char** argv) {
	uint8_t defaultProgram[] = {
		/* 0x0000 */ 0x11, 0x00,       // SEL 0x00
		/* 0x0002 */ 0x12, 0x00, 0x00, // LDC 0x0041
		/* 0x0005 */ 0x19,             // PAC
		/* 0x0006 */ 0x12, 0x01, 0x00, // LDC 0x0001
		/* 0x0009 */ 0x07, 0x00, 0x00, // LCA 0x0000
		/* 0x000C */ 0x02,             // POP
		/* 0x000D */ 0x0F,             // WAW
		/* 0x000E */ 0x13,             // INC
		/* 0x000F */ 0x19,             // PAC
		/* 0x0010 */ 0x1A, 0x00, 0x00, // CMA 0x0000
		/* 0x0013 */ 0x15,             // NOT
		/* 0x0014 */ 0x06, 0x0C, 0x00, // JNZ 0x000C
		/* 0x0017 */ 0xFF              // HLT
	};

	VM vm;

	bool freeProgram = false;

	if (argc == 1) {
		vm.program     = defaultProgram;
		vm.programSize = sizeof(defaultProgram);
	}
	else {
		vm.program = ReadRawFile(argv[1], &vm.programSize);

		if (vm.program == NULL) {
			perror("Failed to open program");
			return 1;
		}

		freeProgram = true;
	}

	VM_Init(&vm, 256);
	VM_Run(&vm);
	VM_Dump(&vm);
	VM_Free(&vm);

	if (freeProgram) {
		free(vm.program);
	}

	return 0;
}
