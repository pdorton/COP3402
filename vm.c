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
int lineCount = 0;

/*-------------------------User Defined Functions-----------------------------------*/

void Fetch(Instruction ir, Instruction* instructionRegister);
void Execute(FILE* output, Instruction ir, int* haltFlag);
void ReadInput(FILE* input, Instruction* intructions, int* lineCount); // function ensured to function. 

/*-------------------------End of Function Prototypes-------------------------------*/

/*-------------------------------Unwritten functions--------------------------------*/
void WriteInstructions(FILE* output, int lineCount,Instruction* instructions); // not yet defined
void PrintStack(FILE* output, int Flag ,Instruction* instructions); // not yet defined
int base(int l, int bp);

/*-------------------------End of unwritten functions-------------------------------*/

int main(int argc, const char* argv[])
{
	FILE* input, *output;
	
	Instruction instructions[MAX_CODE_LENGTH];
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
	
	if(input == NULL)
	{
		printf("File Not Found\n");
		return -1;//return neg one will let programmer know that it returned from file not found
	}
	// read in the input file and place into the array of instructions
ReadInput(input, instructions, &lineCount);
	

	//WriteInstructions(output, lineCount, instructions);
	
	fprintf(output, "STACK");
	//pc = 0 pre initilized earlier
	while(pc < lineCount)
	{// go through all the instructions 
		// first do a fetch for the new instruction 
		Fetch(instructions[pc], &instructionRegister);
		//then increment the pc 
		pc++;
		Execute(output, instructionRegister, &haltFlag);
		
		if(instructionRegister.op == 5)
		{// if a CAL function
			calFlag = 1;
			//PrintStack(output, 0 , instructions);
		}
		else if(instructionRegister.op == 4 && instructionRegister.l > 0)
		{
			calFlag = 0;
		}
		else
		{
			//PrintStack(output, calFlag, instructions);
		}
		
		fprintf(output, "\n");
		if(haltFlag == 1)
		{
			break;
		}
	
	}// end of current instruction 
	// end of all instructions
	
	
	
	
	//close files before exiting 
	fclose(input);
	fclose(output);




	return 0;
}

void ReadInput(FILE* input, Instruction* instructions, int* lineCount)
{
	int i = 0 ; 
	while(!feof(input))
	{// read until end of file
	
		// read in each instruction to a new position in the array
		fscanf(input, "%d %d %d", &instructions[i].op, &instructions[i].l , &instructions[i].m);
		
		printf("op code is: %d L =  %d M =  %d \n", instructions[i].op, instructions[i].l, instructions[i].m);
		
		if(instructions[i].l >= MAX_LEXI_LEVELS)
		{
			printf("Invalid instruction: Level must be no more than 3.\n");
		}
		i++;
		if(i == MAX_CODE_LENGTH)
		{
			printf("Max code length of %d reached.\n", MAX_CODE_LENGTH);
		}
		
	
	}
	// set linecount to total number of instructions 
	*lineCount = i;

	
		printf("%d \n",i );
	
	
}

void Output(FILE* output, int lineCount, Instruction instructions[])
{
	int i = 0 ; 
	for(i = 0 ; i < lineCount; i++)
	{
		if( i < 9 )
		{
			fprintf(output, " ");
			
		}
		switch(instructions[i].op)
		{
			case 1: 
				fprintf(output, "%d    LIT   %d    %d\n", pc, instructions[i].l, instructions[i].m);
        			break;
			case 2:
				fprintf(output, "%d    OPR   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                		break;
			case 3:
				fprintf(output, "%d    LOD   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                		break;
                	case 4:
                		fprintf(output, "%d    STO   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                		break;
                	case 5:
                		fprintf(output, "%d    CAL   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                		break;
                	case 6:
                		fprintf(output, "%d    INC   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                		break;
                	case 7:
                		fprintf(output, "%d    JMP   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                		break;
                	case 8:
                		fprintf(output, "%d    JPC   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                		break;
                	case 9:
                		fprintf(output, "%d    SIO   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                		break;
                	default:
                		fprintf(output, "Invalid Instruction.\n");
      				break;	
		}
	}
	
	fprintf(output, "\n");
}





/*pulls instruction from the current instruction and loads it into the instruction register*/ 
void Fetch(Instruction ir, Instruction* instructionRegister)
{// chose to pass by reference to keep the instructions having less changes throughout 
//pull current instruction and load it into the instruction register 
	instructionRegister->op = ir.op;
	instructionRegister->l = ir.l;
	instructionRegister->m = ir.m;
}



void Execute(FILE* output, Instruction  ir, int* haltFlag)
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
            stack[sp] = stack [ /*base(ir.l, bp)  +*/ ir.m];
            break;

        case 04:  //STO
            stack[ /*base(ir.l, bp) +*/ ir.m] = stack[sp];
            sp = sp - 1;
            break;

        case 05: //CAL
            stack[sp + 1] = 0;                //return value (FV)
            stack[sp + 2] = /*base(ir.l, bp)*/ 0;   //static link (SL)
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

