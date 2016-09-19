/*
Team name: Group 30 aka Team Awesome
Patrick Dorton
Andrew Maida
David Almeida
Michael Garro

COP3402-16Fall001
MW 7:30pm-8:45pm
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct instruction
{
	int op; /* opcode*/
	int l;	// L
	int m; 	// M
} Instruction;

//Constants
#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

int stack[100];
int sp;
int pc;
int bp;

Instruction Fetch(int pc);
void Execute(Instruction ir);




int main()
{
	FILE* input, *output;
	
	Instruction Instructions[MAX_CODE_LENGTH];
	Instruction instructionRegister;
	
    sp = 0;
    bp = 1;
    pc = 0;

	int calFlag = 0; 
	int haltFlag = 0; // flag for if halt 
	
    stack[1] = 0;
    stack[2] = 0;
    stack[3] = 0;

	input = fopen(argv[1], "r");
	output = fopen(argv[2], "w");
	
	if(input == null)
	{
		printf("File Not Found\n");
		return -1;//return neg one will let programmer know that it returned from file not found
	}
	// read in the input file and place into the array of instructions
	*instructions = ReadInput(intput, instructions, &lineCount);
	
	WriteInstructions(output, linecount, instructions);
	
	fprintf(output, "STACK");
	
	while(pc < linecount)
	{// go through all the instructions 
		// first do a fetch for the new instruction 
		Fetch(instructions[pc], &instructionRegister);
	}
	





	return 0;
}

void Fetch(instruction currentInstruction, Intsruction* instructionRegister)
{// chose to pass by reference to keep the instructions having less changes throughout 
//pull current instruction and load it into the instruction register 
	instructionRegister->op = currentInstruction.op;
	instructionRegister->l = currentInstruction.l;
	instructionRegister->m = curretnInstruction.m;
}



void Execute(Instruction ir)
{
    switch (ir.op)
    {
        case 01: //LIT
            sp = sp + 1;
            stack[sp] = ir.m;
            break;
        case 02: //OPR
            switch (ir.m)
            {

                case 1: //NEG
                    stack[sp] = -stack[sp];
                    break;

                case 2: //ADD
                    sp = sp - 1;
                    stack[sp] = stack[sp] + stack[sp + 1];
                    break;

                case 3: //SUB
                    sp = sp - 1;
                    stack[sp] = stack[sp] - stack[sp + 1];
                    break;

                case 4: //MUL
                    sp = sp - 1;
                    stack[sp] = stack[sp] * stack[sp + 1];
                    break;

                case 5: //DIV
                    sp = sp - 1;
                    stack[sp] = stack[sp] / stack[sp + 1];
                    break;

                case 6: //ODD
                    stack[sp] = stack[sp] % 2;
                    break;

                case 7: //MOD
                    sp = sp - 1;
                    stack[sp] = stack[sp] % stack[sp + 1];
                    break;

                case 8: //EQL
                    sp = sp - 1;
                    stack[sp] = stack[sp] == stack[sp + 1];
                    break;

                case 9: //NEQ
                    sp = sp - 1;
                    stack[sp] = stack[sp] != stack[sp + 1];
                    break;

                case 10: //LSS
                    sp = sp - 1;
                    stack[sp] = stack[sp] <  stack[sp + 1];
                    break;

                case 11: //LEQ
                    sp = sp - 1;
                    stack[sp] = stack[sp] <= stack[sp + 1];
                    break;

                case 12: //GTR
                    sp = sp - 1;
                    stack[sp] = stack[sp] >  stack[sp + 1];
                    break;

                case 13: //GEQ
                    sp = sp - 1;
                    stack[sp] = stack[sp] >= stack[sp + 1];
                    break;
            }
            break;

        case 03:  //LOD
            sp = sp + 1;
            stack[sp] = stack [base(ir.l, bp) + ir.m];
            break;

        case 04:  //STO
            stack[ base(ir.l, bp) + ir.m] = stack[sp];
            sp = sp - 1;
            break;

        case 05: //CAL
            stack[sp + 1] = 0;                //return value (FV)
            stack[sp + 2] = base(ir.l, bp);   //static link (SL)
            stack[sp + 3] = bp;               //dynamic link (DL)
            stack[sp + 4] = pc;               //return address (RA)
            bp = sp + 1;
            pc = ir.m;
            break;

        case 06:  //INC
            sp = sp + ir.m;
            break;

        case 07:  //JMP
            pc = ir.m;
            break;
    }
}

