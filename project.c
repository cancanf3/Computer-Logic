/* Author: Jose Pena
 * NID: 4142431
 * PID: jo676717
 * Project: Mini Processor Simulator
 */

#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{

	// ALU Operations
	switch ((int)ALUControl) 
	{
		case 000 :
			*ALUresult = A + B;
			break;
		case 001:
			*ALUresult = A - B;
			break;
		case 010:
			if ((signed) A < (signed) B)
				*ALUresult = 1;
			else
				*ALUresult = 0;
			break;
		case 011:
			if (A < B)
				*ALUresult = 1;
			else
				*ALUresult = 0;
			break;
		case 100:
			*ALUresult = A & B;
			break;
		case 101:
			*ALUresult = A | B;
			break;
		case 110:
			B = B << 16;
			break;
	}

	if (*ALUControl == 0)
		*Zero = 1;
	else 
		*Zero = 0;
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
	if (PC % 4 == 0) 
	{
		*instruction = Mem[PC];
		return 0;
	}
	else
		return 1;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	// Mask to extract only the required info
	unsigned mask_6bits = 0x3f;		//  6 bits
	unsigned mask_5bits = 0x1f;		//  5 bits
	unsigned mask_16bits = 0xffff;	// 16 bits
	unsigned mask_26bits = 0x3ffffff;	// 26 bits

	// Shift and Mask to extract each one
	*op = (instruction >> 25) & mask_6bits;
	*r1 = (instruction >> 20) & mask_5bits;
	*r2 = (instruction >> 15) & mask_5bits;
	*r3 = (instruction >> 10) & mask_5bits;
	*funct = instruction & mask_6bits;
	*offset = instruction & mask_16bits;
	*jsec = instruction & mask_26bits;


}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
	switch (op) 
	{
		case 0x8:
			//addi
			controls->RegDst = 1;
			controls->Jump = 0;
			controls->Branch = 0;
			controls->MemRead = 0;
			controls->MemtoReg = 0;
			controls->ALUOp = 0;
			controls->MemWrite = 0;
			controls->ALUSrc = 1;
			controls->RegWrite = 1;
			break;
		case 0x23:
			//lw
			controls->RegDst = 1;
			controls->Jump = 0;
			controls->Branch = 0;
			controls->MemRead = 1;
			controls->MemtoReg = 1;
			controls->ALUOp = 0;
			controls->MemWrite = 0;
			controls->ALUSrc = 1;
			controls->RegWrite = 1;
			break;
		case 0x2B:
			//sw
			controls->RegDst = 2;
			controls->Jump = 0;
			controls->Branch = 0;
			controls->MemRead = 0;
			controls->MemtoReg = 2;
			controls->ALUOp = 0;
			controls->MemWrite = 1;
			controls->ALUSrc = 1;
			controls->RegWrite = 0;
			break;
		case 0xF:
			//lui
			controls->RegDst = 0;
			controls->Jump = 0;
			controls->Branch = 0;
			controls->MemRead = 1;
			controls->MemtoReg = 0;
			controls->ALUOp = 6;
			controls->MemWrite = 0;
			controls->ALUSrc = 1;
			controls->RegWrite = 1;
			break;
		case 0x4:
			// beq
			controls->RegDst = 2;
			controls->Jump = 0;
			controls->Branch = 1;
			controls->MemRead = 0;
			controls->MemtoReg = 2;
			controls->ALUOp = 1;
			controls->MemWrite = 0;
			controls->ALUSrc = 0;
			controls->RegWrite = 0;
			break;
		case 0xA:
			//slti
			controls->RegDst = 0;
			controls->Jump = 0;
			controls->Branch = 0;
			controls->MemRead = 0;
			controls->MemtoReg = 0;
			controls->ALUOp = 2;
			controls->MemWrite = 0;
			controls->ALUSrc = 1;
			controls->RegWrite = 1;
			break;
		case 0xB:
			//sltiu
			controls->RegDst = 0;
			controls->Jump = 0;
			controls->Branch = 0;
			controls->MemRead = 0;
			controls->MemtoReg = 0;
			controls->ALUOp = 3;
			controls->MemWrite = 0;
			controls->ALUSrc = 1;
			controls->RegWrite = 1;
			break;
		case 0x2:
			//jump
			controls->RegDst = 0;
			controls->Jump = 1;
			controls->Branch = 0;
			controls->MemRead = 0;
			controls->MemtoReg = 0;
			controls->ALUOp = 0;
			controls->MemWrite = 0;
			controls->ALUSrc = 0;
			controls->RegWrite = 0;
			break;
		case 0x0:
			// R-type
			controls->RegDst = 1;
			controls->Jump = 0;
			controls->Branch = 0;
			controls->MemRead = 0;
			controls->MemtoReg = 0;
			controls->ALUOp = 7;
			controls->MemWrite = 0;
			controls->ALUSrc = 0;
			controls->RegWrite = 1;
			break;
		default:
			return 1;
	}
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	*data1 = Reg[r1];
	*data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
	// Determining if it is negative or not
	unsigned negative = offset >> 15;
	if (negative == 1)
		*extended_value = offset | 0xFFFF0000;
	else
		*extended_value = offset | 0xFFFF0000;
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
	
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

