#ifndef YSLVM_VM_H
#define YSLVM_VM_H

#include "components.h"

typedef struct VM {
	uint8_t* program;
	size_t   programSize;
	uint16_t ip; // instruction pointer
	uint8_t* memory;
	size_t   memSize;
	uint16_t sp; // stack pointer
	uint16_t acc; // accumulator
	uint8_t  device;
} VM;

// instructions
enum {
	INSTRUCTION_NOP = 0x00,
	// ()             -> does nothing
	INSTRUCTION_PSH = 0x01,
	// (value)        -> pushes constant to stack
	INSTRUCTION_POP = 0x02,
	// ()             -> pops value on stack to accumulator
	INSTRUCTION_LSP = 0x03,
	// (addr)         -> writes SP to the given address
	INSTRUCTION_SSP = 0x04,
	// (addr)         -> loads SP from the given address
	INSTRUCTION_JMP = 0x05,
	// (addr)         -> jumps to the given address
	INSTRUCTION_JNZ = 0x06,
	// (addr) -> reads accumulator and jumps to addr2 if it is not 0
	INSTRUCTION_LCA = 0x07,
	// (addr)         -> writes the accumulator to the given address
	INSTRUCTION_SCA = 0x08,
	// (addr)         -> loads the accumulator from the given address
	INSTRUCTION_CMP = 0x09,
	// (addr1, addr2) -> reads 2 values from the given addresses and sets the
	//                   accumulator to 65535 if equal, 0 if not
	INSTRUCTION_ADD = 0x0A,
	// (addr)         -> adds the value in the given address to the accumulator
	INSTRUCTION_SUB = 0x0B,
	// (addr)         -> subtracts by the value in the given address to the
	//                   accumulator
	INSTRUCTION_MUL = 0x0C,
	// (addr)         -> multiplies the accumulator by the value in the given
	//                   address
	INSTRUCTION_DIV = 0x0D,
	// (addr)         -> divides the accumulator by the value in the given address
	INSTRUCTION_MOD = 0x0E,
	// (addr)         -> divides the accumulator by the value in the given address
	//                   and stores the remainder
	INSTRUCTION_WAW = 0x0F,
	// ()             -> writes the 16-bit accumulator to the device
	INSTRUCTION_RAW = 0x10,
	// ()             -> reads from the device to the 16-bit accumulator
	INSTRUCTION_SEL = 0x11,
	// (byte)         -> selects the given device
	INSTRUCTION_LDC = 0x12,
	// (word)         -> sets accumulator to the given constant
	INSTRUCTION_INC = 0x13,
	// ()             -> increments the accumulator
	INSTRUCTION_DEC = 0x14,
	// ()             -> decrements the accumulator
	INSTRUCTION_NOT = 0x15,
	// ()             -> does bitwise not on the accumulator
	INSTRUCTION_AND = 0x16,
	// (addr)         -> does bitwise and with the value in the address
	INSTRUCTION_BOR = 0x17,
	// (addr)         -> does bitwise or with the value in the address
	INSTRUCTION_XOR = 0x18,
	// (addr)         -> does bitwise xor with the value in the address
	INSTRUCTION_PAC = 0x19,
	// ()             -> pushes the accumulator onto the stack
	INSTRUCTION_CMA = 0x1A,
	// (addr)         -> compares the accumulator with the value at the given
	//                   address
	INSTRUCTION_HLT = 0xFF
};

void VM_Init(VM* vm, size_t memSize);
void VM_Free(VM* vm);
void VM_Run(VM* vm);
void VM_Dump(VM* vm);

#endif
