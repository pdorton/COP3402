/*
 Team name: Group 30 aka Team Awesome
 Patrick Dorton
 Andrew Maida
 Michael Garro
 COP 3402-16Fall001
 MW 7:30-8:45
*/

#include <stdio.h>
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

    fprintf(ofp, "PL/0 Code:\n");

    while(fscanf(ifp, "%d %d %d", &input1, &input2, &input3) != EOF)
    {
        instructionSet[counter].op = input1;
        if (input1 == 2)
        {
            fprintf(ofp, "%d\t%s\t", counter, OPRcodes[input3]);
        }
        else if (input1 == 9)
        {
            fprintf(ofp, "%d\t%s\t", counter, SIOCODES[input3]);
        }
        else
            fprintf(ofp, "%d\t%s\t", counter, OPCODES[input1 - 1]);

        instructionSet[counter].l = input2;
        fprintf(ofp, "%d\t", input2);

        instructionSet[counter].m = input3;
        fprintf(ofp, "%d\n", input3);

        counter++;
    }

    instructionCount = counter - 1;
    counter = 0;
    int instructionsRead = 0;

    while (instructionsRead <= instructionCount)
    {
        int userInput;
        int l = instructionSet[pc].l;
        int m = instructionSet[pc].m;
        int op = instructionSet[pc].op - 1;

        fprintf(ofp, "%d\t%s\t%d\t%d\t", pc, OPCODES[op], l, m);

        if (pc == instructionCount)
        {
            lastInstruction = 1;
        }

        pc++;

        switch(op)
        {
            case 1:  //LIT
                sp++;
                stack[sp] = m;
                break;

            case 2:  //OPR
                switch (m)
                {
                    case 0:
                        sp = bp - 1;
                        pc = stack[sp + 4];
                        bp = stack[sp + 3];
                        /*int k = 0;
                        while(1)
                        {
                            if (activationRecords[k] == sp)
                            {
                                activationRecords[k] = 0;
                                break;
                            }
                            k++;
                        }*/
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

                case 3: // "LOD"
                    sp++;
                    stack[sp] = stack[base(l, bp) + m];
                    break;


                case 4: // "STO"
                    stack[base(l, bp) + m] = stack[sp];
                    sp--;
                    break;


                case 5: // "CAL"
                    stack[sp + 1] = 0;
                    stack[sp + 2] = base(l, bp);
                    stack[sp + 3] = bp;
                    stack[sp + 4] = pc;
                    bp = sp + 1;
                    pc = m;
                    break;


                case 6: // "INC"
                    /*if(sp > 0)
                    {
                        activationRecords[activationRecordsIndex] = sp; // so the '|' can be outputted correctly
                        activationRecordsIndex++;
                    }*/
                    sp = sp + m;
                    break;


                case 7: // "JMP"
                    pc = m;
                    break;


                case 8: // "JPC"
                    if(stack[sp] == 0)
                        pc = m;
                    sp--;
                    break;


                case 9: // "SIO"
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
            int i;
            int c = 0;
            for(i = 1; i <= sp; i++)
            {
                if(i == activationRecords[c] + 1 && sp > activationRecords[c] + 1 && c != 0)
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
        b = stack[b];
        l--;
    }
    return b;
}
