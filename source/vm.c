#include "util.h"
#include "vm.h"

#define CHECK_ADDRESS(op, addr)                                         \
	do {                                                                \
		if (addr >= vm->memSize) {                                      \
			fprintf(                                                    \
				stderr, "[FATAL] Out of bounds " op " (0x%04X)\n", addr \
			);                                                          \
			return;                                                     \
		}                                                               \
	} while (0)

#define VM_NEXT() \
	do {                                                    \
		++ vm->ip;                                          \
		if (vm->ip >= vm->programSize) {                    \
			fprintf(stderr, "[FATAL] IP > program size\n"); \
			return;                                         \
		}                                                   \
	} while(0)

static uint16_t VM_GetWordOperand(VM* vm) {
	uint16_t ret = 0;
	ret |= vm->program[vm->ip];

	++ vm->ip;
	ret |= vm->program[vm->ip] << 8;

	return ret;
}

static uint16_t VM_ReadWord(VM* vm, uint16_t addr) {
	uint16_t ret = 0;
	ret |= vm->memory[addr];
	ret |= vm->memory[addr + 1] << 8;

	return ret;
}

static void VM_WriteWord(VM* vm, uint16_t addr, uint16_t value) {
	vm->memory[addr]     = value & 0x00FF;
	vm->memory[addr + 1] = (value & 0xFF00) >> 8;
}

static void VM_WriteDevice(VM* vm) {
	switch (vm->device) {
		case 0x00: {
			putchar(vm->acc);
			break;
		}
		default: {
			fprintf(
				stderr, "[ERROR] Wrote to unknown device 0x%02X\n", vm->device
			);
		}
	}
}

static uint16_t VM_ReadDevice(VM* vm) {
	switch (vm->device) {
		case 0x00: {
			return getchar();
		}
		default: {
			fprintf(
				stderr, "[ERROR] Read to unknown device 0x%02X\n", vm->device
			);
			return 0;
		}
	}
}

static char* VM_InstructionToString(VM* vm) {
	switch (vm->program[vm->ip]) {
		case INSTRUCTION_NOP: return DupString("NOP");
		case INSTRUCTION_PSH: return DupString("PSH");
		case INSTRUCTION_POP: return DupString("POP");
		case INSTRUCTION_LSP: return DupString("LSP");
		case INSTRUCTION_SSP: return DupString("SSP");
		case INSTRUCTION_JMP: return DupString("JMP");
		case INSTRUCTION_JNZ: return DupString("JNZ");
		case INSTRUCTION_LCA: return DupString("LCA");
		case INSTRUCTION_SCA: return DupString("SCA");
		case INSTRUCTION_CMP: return DupString("CMP");
		case INSTRUCTION_ADD: return DupString("ADD");
		case INSTRUCTION_SUB: return DupString("SUB");
		case INSTRUCTION_MUL: return DupString("MUL");
		case INSTRUCTION_DIV: return DupString("DIV");
		case INSTRUCTION_MOD: return DupString("MOD");
		case INSTRUCTION_WAW: return DupString("WAW");
		case INSTRUCTION_RAW: return DupString("RAW");
		case INSTRUCTION_SEL: return DupString("SEL");
		case INSTRUCTION_LDC: return DupString("LDC");
		case INSTRUCTION_INC: return DupString("INC");
		case INSTRUCTION_DEC: return DupString("DEC");
		case INSTRUCTION_NOT: return DupString("NOT");
		case INSTRUCTION_AND: return DupString("AND");
		case INSTRUCTION_BOR: return DupString("BOR");
		case INSTRUCTION_XOR: return DupString("XOR");
		case INSTRUCTION_PAC: return DupString("PAC");
		case INSTRUCTION_CMA: return DupString("CMA");
		case INSTRUCTION_HLT: return DupString("HLT");
		default:              return DupString("UNKNOWN");
	}
}

void VM_Init(VM* vm, size_t memSize) {
	vm->memory  = malloc(memSize);
	vm->memSize = memSize;
}

void VM_Free(VM* vm) {
	free(vm->memory);
}

void VM_Run(VM* vm) {
	bool running = true;

	vm->ip = 0;
	vm->sp = 0;

	while (running) {
		uint8_t inst = vm->program[vm->ip];

		char* instName = VM_InstructionToString(vm);
		//printf("Executing instruction %s at %02lX\n", instName, vm->ip);
		free(instName);

		switch (inst) {
			case INSTRUCTION_NOP: {
				VM_NEXT();
				break;
			}
			case INSTRUCTION_PSH: {
				VM_NEXT();

				uint16_t value = VM_GetWordOperand(vm);

				++ vm->sp;
				VM_NEXT();

				if (vm->sp >= vm->memSize) {
					fprintf(stderr, "[FATAL] Stack overflow\n");
					return;
				}

				VM_WriteWord(vm, vm->sp, value);
				break;
			}
			case INSTRUCTION_POP: {
				VM_NEXT();
				vm->acc = VM_ReadWord(vm, vm->sp);
				-- vm->sp;
				break;
			}
			case INSTRUCTION_LSP: {
				VM_NEXT();

				uint16_t addr = VM_GetWordOperand(vm);

				VM_NEXT();

				CHECK_ADDRESS("write", addr);

				VM_WriteWord(vm, addr, vm->sp);
				break;
			}
			case INSTRUCTION_SSP: {
				VM_NEXT();

				uint16_t addr = VM_GetWordOperand(vm);
				VM_NEXT();

				CHECK_ADDRESS("read", addr);

				vm->sp = VM_ReadWord(vm, addr);
				break;
			}
			case INSTRUCTION_JMP: {
				VM_NEXT();

				uint16_t addr = VM_GetWordOperand(vm);

				VM_NEXT();

				CHECK_ADDRESS("read", addr);

				vm->ip = addr;
				break;
			}
			case INSTRUCTION_JNZ: {
				VM_NEXT();

				uint16_t addr = VM_GetWordOperand(vm);

				VM_NEXT();

				if (vm->acc != 0) {
					vm->ip = addr;
				}
				break;
			}
			case INSTRUCTION_LCA: {
				VM_NEXT();

				uint16_t addr = VM_GetWordOperand(vm);

				VM_NEXT();

				CHECK_ADDRESS("write", addr);

				VM_WriteWord(vm, addr, vm->acc);
				break;
			}
			case INSTRUCTION_SCA: {
				VM_NEXT();

				uint16_t addr = VM_GetWordOperand(vm);

				VM_NEXT();

				CHECK_ADDRESS("read", addr);

				vm->acc = VM_ReadWord(vm, addr);
				break;
			}
			case INSTRUCTION_CMP: {
				VM_NEXT();
				uint16_t addr1 = VM_GetWordOperand(vm);
				VM_NEXT();
				uint16_t addr2 = VM_GetWordOperand(vm);
				VM_NEXT();

				CHECK_ADDRESS("read", addr1);
				CHECK_ADDRESS("read", addr2);

				vm->acc =
					VM_ReadWord(vm, addr1) == VM_ReadWord(vm, addr2)? 0xFFFF : 0;

				break;
			}
			case INSTRUCTION_ADD: {
				VM_NEXT();
				uint16_t addr = VM_GetWordOperand(vm);
				VM_NEXT();

				CHECK_ADDRESS("read", addr);
				vm->acc += VM_ReadWord(vm, addr);
				break;
			}
			case INSTRUCTION_SUB: {
				VM_NEXT();
				uint16_t addr = VM_GetWordOperand(vm);
				VM_NEXT();

				CHECK_ADDRESS("read", addr);
				vm->acc -= VM_ReadWord(vm, addr);
				break;
			}
			case INSTRUCTION_MUL: {
				VM_NEXT();
				uint16_t addr = VM_GetWordOperand(vm);
				VM_NEXT();

				CHECK_ADDRESS("read", addr);
				vm->acc *= VM_ReadWord(vm, addr);
				break;
			}
			case INSTRUCTION_DIV: {
				VM_NEXT();
				uint16_t addr = VM_GetWordOperand(vm);
				VM_NEXT();

				CHECK_ADDRESS("read", addr);
				vm->acc /= VM_ReadWord(vm, addr);
				break;
			}
			case INSTRUCTION_MOD: {
				VM_NEXT();
				uint16_t addr = VM_GetWordOperand(vm);
				VM_NEXT();

				CHECK_ADDRESS("read", addr);
				vm->acc %= VM_ReadWord(vm, addr);
				break;
			}
			case INSTRUCTION_WAW: {
				VM_WriteDevice(vm);
				VM_NEXT();
				break;
			}
			case INSTRUCTION_RAW: {
				vm->acc = VM_ReadDevice(vm);
				VM_NEXT();
				break;
			}
			case INSTRUCTION_SEL: {
				VM_NEXT();
				vm->device = vm->program[vm->ip];
				VM_NEXT();
				break;
			}
			case INSTRUCTION_LDC: {
				VM_NEXT();

				uint16_t value = VM_GetWordOperand(vm);
				VM_NEXT();

				vm->acc = value;
				break;
			}
			case INSTRUCTION_INC: {
				VM_NEXT();
				++ vm->acc;
				break;
			}
			case INSTRUCTION_DEC: {
				VM_NEXT();
				-- vm->acc;
				break;
			}
			case INSTRUCTION_NOT: {
				VM_NEXT();
				vm->acc = ~vm->acc;
				break;
			}
			case INSTRUCTION_AND: {
				VM_NEXT();
				uint16_t addr = VM_GetWordOperand(vm);
				CHECK_ADDRESS("read", addr);
				VM_NEXT();

				vm->acc &= VM_ReadWord(vm, addr);
				break;
			}
			case INSTRUCTION_BOR: {
				VM_NEXT();
				uint16_t addr = VM_GetWordOperand(vm);
				CHECK_ADDRESS("read", addr);
				VM_NEXT();

				vm->acc |= VM_ReadWord(vm, addr);
				break;
			}
			case INSTRUCTION_XOR: {
				VM_NEXT();
				uint16_t addr = VM_GetWordOperand(vm);
				CHECK_ADDRESS("read", addr);
				VM_NEXT();

				vm->acc ^= VM_ReadWord(vm, addr);
				break;
			}
			case INSTRUCTION_PAC: {
				VM_NEXT();
				++ vm->sp;

				if (vm->sp >= vm->memSize) {
					fprintf(stderr, "[FATAL] Stack overflow\n");
					return;
				}
				
				VM_WriteWord(vm, vm->sp, vm->acc);
				break;
			}
			case INSTRUCTION_CMA: {
				VM_NEXT();
				uint16_t addr = VM_GetWordOperand(vm);
				CHECK_ADDRESS("read", addr);

				vm->acc = VM_ReadWord(vm, addr) == vm->acc? 0xFFFF : 0;
				break;
			}
			case INSTRUCTION_HLT: {
				running = false;
				puts("\n\nProgram halted");
				break;
			}
			default: {
				++ vm->ip;
				fprintf(stderr, "[ERROR] Unknown opcode 0x%02X\n", inst);
				break;
			}
		}
	}
}

void VM_Dump(VM* vm) {
	puts("===VM Info===");
	puts("==Registers==");
	printf("IP:  0x%02X\n", vm->ip);
	printf("SP:  0x%02X\n", vm->sp);
	printf("ACC: 0x%02X\n", vm->acc);
	printf("DEV: 0x%02X\n", vm->device);
}
