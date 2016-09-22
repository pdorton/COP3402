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
Instruction instructionsCalled[MAX_CODE_LENGTH];
int instructionsCalledIndex = 0;
int pcHistory[100];
int bpHistory[100];
int spHistory[100];
int pcIndex = 0;
int bpIndex = 0;
int spIndex = 0;
int stackHistory[100][100];

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

    //initialize stack
    stack[1] = 0;
    stack[2] = 0;
    stack[3] = 0;

    //initialize stack history array
    //we use the first entry of each row as the index counter
    int i;
    for(i = 0; i < 100; i++)
        stackHistory[i][0] = 0;

	input = fopen(argv[1], "r");
	output = fopen(argv[2], "w");

	if(input == NULL)
	{
		printf("File Not Found\n");
		return -1;//return neg one will let programmer know that it returned from file not found
	}
	// read in the input file and place into the array of instructions
    ReadInput(input, instructions, &lineCount);


	WriteInstructions(output, lineCount, instructions);

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

void WriteInstructions(FILE* output, int lineCount,Instruction* instructions)
{
	fprintf(output, "PL/0 code:\n\n");
	int i;

	for(i = 0 ; i < lineCount; i++)
	{// for each instruction print out
		// pc op and m
		switch(instructions[i].op)
		{
			case 1:
				fprintf(output, "\t%d\tLIT\t\t\t\t%d\n", i,  instructions[i].m);
                break;
			case 2:
				switch(instructions[i].m)
				{
					case 0: //RET
					{
						fprintf(output, "\t%d\tRET\t\t\t\t%d\n", i,  instructions[i].m);
						break;
					}


					case 1: //NEG
					{
						fprintf(output, "\t%d\tNEG\t\t\t\t%d\n", i,  instructions[i].m);
						break;
					}


					case 2: //ADD
					{
						fprintf(output, "\t%d\tADD\t\t\t\t%d\n", i,  instructions[i].m);
						break;
					}


					case 3: //SUB
					{
						fprintf(output, "\t%d\tSUB\t\t\t\t%d\n", i,  instructions[i].m);
						break;
					}


					case 4: //MUL
					{
						fprintf(output, "\t%d\tMUL\t\t\t\t%d\n", i,  instructions[i].m);
						break;
					}


					case 5: //DIV
					{
						fprintf(output, "\t%d\tDIV\t\t\t\t%d\n", i,  instructions[i].m);
						break;
					}


					case 6: //ODD
					{
						fprintf(output, "\t%d\tODD\t\t\t\t%d\n", i,  instructions[i].m);
						break;
					}


					case 7: //MOD
					{
						fprintf(output, "\t%d\tMOD\t\t\t\t%d\n", i,  instructions[i].m);
						break;
					}


					case 8: //EQL
					{
						fprintf(output, "\t%d\tEQL\t\t\t\t%d\n", i,  instructions[i].m);
						break;
					}


					case 9: //NEQ
					{
						fprintf(output, "\t%d\tNEQ\t\t\t\t%d\n", i,  instructions[i].m);
						break;
					}


					case 10: //LSS
					{
						fprintf(output, "\t%d\tLSS\t\t\t\t%d\n", i,  instructions[i].m);
						break;
					}


					case 11: //LEQ
					{
						fprintf(output, "\t%d\tLEQ\t\t\t\t%d\n", i,  instructions[i].m);
						break;
					}


					case 12: //GTR
					{
						fprintf(output, "\t%d\tGTR\t\t\t\t%d\n", i,  instructions[i].m);
						break;
					}


					case 13: //GEQ
					{
						fprintf(output, "\t%d\tGEQ\t\t\t\t%d\n", i,  instructions[i].m);
						break;
					}

				}
				break;


			//	fprintf(output, "\t%d\tOPR\t\t\t\t%d\n", i,  instructions[i].m);
                		//break;
			case 3:
				fprintf(output, "\t%d\tLOD\t\t\t\t%d\n", i,  instructions[i].m);
                		break;
                	case 4:
                		fprintf(output, "\t%d\tSTO\t\t%d\t\t%d\n", i, instructions[i].l, instructions[i].m);
                		break;
                	case 5:
                		fprintf(output, "\t%d\tCAL\t\t%d\t\t%d\n", i, instructions[i].l, instructions[i].m);
                		break;
                	case 6:
                		fprintf(output, "\t%d\tINC\t\t\t\t%d\n", i, instructions[i].m);
                		break;
                	case 7:
                		fprintf(output, "\t%d\tJMP\t\t\t\t%d\n", i, instructions[i].m);
                		break;
                	case 8:
                		fprintf(output, "\t%d\tJPC\t\t\t\t%d\n", i,  instructions[i].m);
                		break;
                	case 9:
                		if(instructions[i].m ==2)
                		{
                		fprintf(output, "\t%d\tHLT\t\t\t\t\n", i, instructions[i].m);
                		break;
						}
                		fprintf(output, "\t%d\tSIO\t\t\t\t%d\n", i, instructions[i].m);
                		break;
                	default:
                		fprintf(output, "Invalid Instruction.\n");
      				break;

        }

    }
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
        case 1: //LIT
            sp = sp + 1;
            stack[sp] = ir.m;
            break;
        case 2: //OPR
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

        case 3:  //LOD
            sp = sp + 1;
            stack[sp] = stack [ /*base(ir.l, bp)  +*/ ir.m];
            break;

        case 4:  //STO
            stack[ /*base(ir.l, bp) +*/ ir.m] = stack[sp];
            sp = sp - 1;
            break;

        case 5: //CAL
            stack[sp + 1] = 0;                //return value (FV)
            stack[sp + 2] = /*base(ir.l, bp)*/ 0;   //static link (SL)
            stack[sp + 3] = bp;               //dynamic link (DL)
            stack[sp + 4] = pc;               //return address (RA)
            bp = sp + 1;
            pc = ir.m;
            break;

        case 6:  //INC
            sp = sp + ir.m;
            break;

        case 7:  //JMP
            pc = ir.m;
            break;

        case 8:  //JPC
            if(stack[sp] == 0)
                pc = ir.m;
            sp--;
            break;

        case 9:  //SIO
            switch (ir.m)
            {
                case 0:  //OUT
                    printf("%d\n", stack[sp]);
                    sp--;
                    break;

                case 1:  //INP
                    sp++;
                    scanf("%d", &stack[sp]);
                    break;

                case 2:  //HLT
                    return;
                    break;
            }
            break;
    }

    instructionsCalled[instructionsCalledIndex] = ir;
    instructionsCalledIndex++;
    bpHistory[bpIndex] = bp;
    spHistory[spIndex] = sp;
    pcHistory[pcIndex] = pc;
    bpIndex++;
    spIndex++;
    pcIndex++;
}

void PrintStack(FILE* output, int Flag ,Instruction* instructions)
{

        // for each instruction print out
		// pc op and m
		switch(instructionsCalled[i].op)
		{
			case 1:
				fprintf(output, "\t%d\tLIT\t\t\t\t%d", i,  instructionsCalled[i].m);
                break;
			case 2:
				switch(instructionsCalled[i].m)
				{
					case 0: //RET
					{
						fprintf(output, "\t%d\tRET\t\t\t\t%d", i,  instructionsCalled[i].m);
						break;
					}


					case 1: //NEG
					{
						fprintf(output, "\t%d\tNEG\t\t\t\t%d", i,  instructionsCalled[i].m);
						break;
					}


					case 2: //ADD
					{
						fprintf(output, "\t%d\tADD\t\t\t\t%d", i,  instructionsCalled[i].m);
						break;
					}


					case 3: //SUB
					{
						fprintf(output, "\t%d\tSUB\t\t\t\t%d", i,  instructionsCalled[i].m);
						break;
					}


					case 4: //MUL
					{
						fprintf(output, "\t%d\tMUL\t\t\t\t%d", i,  instructionsCalled[i].m);
						break;
					}


					case 5: //DIV
					{
						fprintf(output, "\t%d\tDIV\t\t\t\t%d", i,  instructionsCalled[i].m);
						break;
					}


					case 6: //ODD
					{
						fprintf(output, "\t%d\tODD\t\t\t\t%d", i,  instructionsCalled[i].m);
						break;
					}


					case 7: //MOD
					{
						fprintf(output, "\t%d\tMOD\t\t\t\t%d", i,  instructionsCalled[i].m);
						break;
					}


					case 8: //EQL
					{
						fprintf(output, "\t%d\tEQL\t\t\t\t%d", i,  instructionsCalled[i].m);
						break;
					}


					case 9: //NEQ
					{
						fprintf(output, "\t%d\tNEQ\t\t\t\t%d", i,  instructionsCalled[i].m);
						break;
					}


					case 10: //LSS
					{
						fprintf(output, "\t%d\tLSS\t\t\t\t%d", i,  instructionsCalled[i].m);
						break;
					}


					case 11: //LEQ
					{
						fprintf(output, "\t%d\tLEQ\t\t\t\t%d", i,  instructionsCalled[i].m);
						break;
					}


					case 12: //GTR
					{
						fprintf(output, "\t%d\tGTR\t\t\t\t%d", i,  instructionsCalled[i].m);
						break;
					}


					case 13: //GEQ
					{
						fprintf(output, "\t%d\tGEQ\t\t\t\t%d", i,  instructionsCalled[i].m);
						break;
					}

				}
				break;


			//	fprintf(output, "\t%d\tOPR\t\t\t\t%d\n", i,  instructions[i].m);
                		//break;
			case 3:
				fprintf(output, "\t%d\tLOD\t\t\t\t%d", i,  instructionsCalled[i].m);
                		break;
                	case 4:
                		fprintf(output, "\t%d\tSTO\t\t%d\t\t%d", i, instructionsCalled[i].l, instructionsCalled[i].m);
                		break;
                	case 5:
                		fprintf(output, "\t%d\tCAL\t\t%d\t\t%d", i, instructionsCalled[i].l, instructionsCalled[i].m);
                		break;
                	case 6:
                		fprintf(output, "\t%d\tINC\t\t\t\t%d", i, instructionsCalled[i].m);
                		break;
                	case 7:
                		fprintf(output, "\t%d\tJMP\t\t\t\t%d", i, instructionsCalled[i].m);
                		break;
                	case 8:
                		fprintf(output, "\t%d\tJPC\t\t\t\t%d", i,  instructionsCalled[i].m);
                		break;
                	case 9:
                		if(instructionsCalled[i].m ==2)
                		{
                		fprintf(output, "\t%d\tHLT\t\t\t\t", i, instructionsCalled[i].m);
                		break;
						}
                		fprintf(output, "\t%d\tSIO\t\t\t\t%d", i, instructionsCalled[i].m);
                		break;
                	default:
                		fprintf(output, "Invalid Instruction.");
      				break;

        }
        fprintf(output, "\t%d\t%d\t%d", pcHistory[i], bpHistory[i], spHistory[i]);

}
