/*
 Team name: Group 30 aka Team Awesome
 Patrick Dorton
 Andrew Maida
 Michael Garro
 David Almeida
 COP 3402-16Fall001
 MW 7:30-8:45
 version 1.0 (tested with just single input file from wiki)
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEXI_LEVELS 3
#define MAX_CODE_LENGTH 500
#define MAX_STACK_HEIGHT 2000

struct Instruction
{
    int op;
    int m;
    int l;
};

int base(int l, int base);

int pc = 0;
int flag = 0;
int bp = 1;
int sp = 0;
int stack[MAX_STACK_HEIGHT];


int main()
{
    int input1, input2, input3, input4;

    stack[0] = 0;
    stack[1] = 0;
    stack[2] = 0;

    int counter = 0;
    int instructionCount;
    char *OPCODES[] = {"LIT", "OPR", "LOD", "STO", "CAL", "INC", "JMP", "JPC", "SIO", "SIO", "SIO"};
    char *OPRcodes[] = {"RET", "NEG", "ADD", "SUB", "MUL", "DIV", "ODD", "MOD", "EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ"};
    char *SIOCODES[] = {"OUT", "INP", "HLT"};
    struct Instruction instructionSet[MAX_CODE_LENGTH];

    int activationRecords[500];
    int activationRecordsIndex = 0;

    int spLessThanAR = 0;
    int lastInstruction = 0;

    FILE *ifp = fopen("input.txt", "r");
    FILE *ofp = fopen("output.txt", "w");

    fprintf(ofp, "PL/0 Code:\n\n");

    while(fscanf(ifp, "%d %d %d", &input1, &input2, &input3) != EOF)
    {
        instructionSet[counter].op = input1;

        if (input1 == 2)
        {
            fprintf(ofp, "%d\t%s", counter, OPRcodes[input3]);
        }
        else if (input1 == 9)
        {
            fprintf(ofp, "%d\t%s", counter, SIOCODES[input3]);
        }
        else
            fprintf(ofp, "%d\t%s", counter, OPCODES[input1 - 1]);
        if(input1 !=9)
        {
        	if(OPCODES[input1 - 1] == OPCODES[3] || OPCODES[input1 - 1] == OPCODES[4] || OPCODES[input1-1] == OPCODES[2])
        	{
        		instructionSet[counter].l = input2;
        		fprintf(ofp, "\t%d", input2);
        	}
        	else
        	{
        		fprintf(ofp, "\t");
        	}
        	if(OPCODES[input1] != OPCODES[2] && OPRcodes != OPRcodes[0])	
        	{
        		instructionSet[counter].m = input3;
        		fprintf(ofp, "\t%d\n", input3);
        	}
        	else
        	{
        		fprintf(ofp, "\n");
        	}
		}
        counter++;
    }

    instructionCount = counter - 1;
    counter = 0;
    int instructionsRead = 0;
    fprintf(ofp, "\n\nExecution:\n\t\t\t\tpc\tbp\tsp\tstack\n\t\t\t\t%d\t%d\t%d\n", pc, bp, sp);

    while (instructionsRead <= instructionCount)
    {
        int userInput;
        int l = instructionSet[pc].l;
        int m = instructionSet[pc].m;
        int op = instructionSet[pc].op - 1;

        	
        if(OPCODES[op] == OPCODES[1] )
        {
        	OPCODES[op] = OPRcodes[op-1];
        }


        if(OPCODES[op] == OPCODES[10]) 
        {
        	OPCODES[op] = SIOCODES[2];

        }
        if(OPCODES[op] == OPCODES[9]) 
        {
        	OPCODES[op] = SIOCODES[1];

        }
        if(OPCODES[op] == OPCODES[8]) 
        {
        	OPCODES[op] = SIOCODES[0];

        }

        if(OPCODES[op] == OPCODES[3] || OPCODES[op] == OPCODES[4] || OPCODES[op] == OPCODES[2])
        {
        	fprintf(ofp, "%d\t%s\t%d\t%d\t", pc, OPCODES[op], l , m);
        }
        else
        {
        	fprintf(ofp, "%d\t%s\t\t%d\t", pc, OPCODES[op], m);
    	}

        if (pc == instructionCount)
        {
            lastInstruction = 1;
        }

        pc++;

        switch(op)
        {
            case 0:  //LIT
                sp++;
                stack[sp] = m;
                break;

            case 1:  //OPR
                switch (m)
                {
                    case 0:
                        sp = bp - 1;
                        pc = stack[sp + 4];
                        bp = stack[sp + 3];
                        int k = 0;
                        while(1)
                        {
                            if (activationRecords[k] == sp)
                            {
                                activationRecords[k] = 0;
                                break;
                            }
                            k++;
                        }
                        break;

                    case 1:
                        stack[sp] = -stack[sp];
                        break;

                    case 2:
                        sp--;
                        stack[sp] = stack[sp] + stack[sp + 1];
                        break;

                    case 3:
                        sp--;
                        stack[sp] = stack[sp] - stack[sp + 1];
                        break;

                    case 4:
                        sp--;
                        stack[sp] = stack[sp] * stack[sp + 1];
                        break;

                    case 5:
                        sp--;
                        stack[sp] = stack[sp] / stack[sp + 1];
                        break;

                    case 6:
                        stack[sp] = stack[sp] % 2;
                        break;

                    case 7:
                        sp--;
                        stack[sp] = stack[sp] % stack[sp + 1];
                        break;

                    case 8:
                        sp--;
                        stack[sp] = stack[sp] == stack[sp + 1];
                        break;

                    case 9:
                        sp--;
                        stack[sp] = stack[sp] != stack[sp + 1];
                        break;

                    case 10:
                        sp--;
                        stack[sp] = stack[sp] < stack[sp + 1];
                        break;

                    case 11:
                        sp--;
                        stack[sp] = stack[sp] <= stack[sp + 1];
                        break;

                    case 12:
                        sp--;
                        stack[sp] = stack[sp] > stack[sp + 1];
                        break;

                    case 13:
                        sp--;
                        stack[sp] = stack[sp] >= stack[sp + 1];
                        break;
                }
                break;

                case 2: // "LOD"
                    sp++;
                    stack[sp] = stack[base(l, bp) + m];
                    break;


                case 3: // "STO"
                    stack[base(l, bp) + m] = stack[sp];
                    sp--;
                    break;


                case 4: // "CAL"
                    stack[sp + 1] = 0;
                    stack[sp + 2] = base(l, bp);
                    stack[sp + 3] = bp;
                    stack[sp + 4] = pc;
                    bp = sp + 1;
                    pc = m;
                    break;


                case 5: // "INC"
                    if(sp > 0)
                    {
                        activationRecords[activationRecordsIndex] = sp; // so the '|' can be outputted correctly
                        activationRecordsIndex++;
                    }
                    sp = sp + m;
                    break;


                case 6: // "JMP"
                    pc = m;
                    break;


                case 7: // "JPC"
                    if(stack[sp] == 0)
                        pc = m;
                    sp--;
                    break;


                case 8: // "SIO"
                    switch (m)
                    {
                        case 1:  //OUT
                            printf("OUT: %d\n", stack[sp]);
                            sp--;
                            break;

                        case 2: //INP
                            sp++;
                            printf("\nNumber to input: ");
                            scanf("%d", &userInput);
                            stack[sp] = userInput;
                            break;

                        case 3:  //HLT
                            fprintf(ofp, "\nSuccessfully halted.\n");
                            flag = 1;
                            return 0;
                    }
        }

        if(flag == 0)
        {
            fprintf(ofp, "%d\t%d\t%d\t", pc, bp, sp);
            int i = 1;
            int c = 0;
            for(i = 1; i <= sp; i++)
            {
                if(i == activationRecords[c] + 1 && sp > activationRecords[c] + 1 && activationRecords[c] != 0)
                {
                    fprintf(ofp, "| ");
                    c++;
                }
                fprintf(ofp, "%d ", stack[i]);
                
            }
            fprintf(ofp, "\n");
            
        }

        if(lastInstruction == 1)
        {
            break;
        }

        instructionsRead++;
    }
    return 0;
}


int base(int l, int base)
{
    int b = base;
    while(l > 0)
    {
        b = stack[b + 1];
        l--;
    }
    return b;
}
