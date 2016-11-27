/*
    William Jeremy RiCharde
    COP 3402 - Systems Software
    Assignment 1 - Virtual Machine
    Due June 6, 2014

    Notes:  This is a virtual machine for the PL/O programming language.  Reads
    in a file named input.txt with the assembly code and runs it.  Prints
    details of the registers and activation records to the file output.txt.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3


typedef struct
{
    int op; // opcode
    int r;  // reg
    int l;  // L
    int m;  // M
} instruction;

// function definitions
void printCode( FILE *ofp, int code[][MAX_CODE_LENGTH], int codeLength );

instruction fetch( int programCounter, int code[][MAX_CODE_LENGTH] );

char* getOpCode( int opCode );

int base( int l, int base, int *stack );



// begin main
int main(void)
{
    
    int buffer, counter = 0, codeLen = 0;;
    int i, j;
    int line = 0;
    
    // declare and initialize the stack
    int stack[MAX_STACK_HEIGHT];
    for ( i = 0; i < MAX_STACK_HEIGHT; i++ )
    {
        stack[i] = 0;
    }
    
    
    // declare and reset the array that stores the program code
    int code[4][MAX_CODE_LENGTH];
    
    for ( i = 0; i < MAX_CODE_LENGTH; i++ )
    {
        for ( j = 0; j < 4; j++ )
        {
            code[j][i] = 0;
        }
    }
    
    
    // declare and initialize the CPU register variables
    int stackPtr = 0;
    int basePtr = 1;
    int PC = 0;
    instruction ir;
    ir.op = 0;
    ir.r = 0;
    ir.l = 0;
    ir.m = 0;
    
    int reg[16];
    for ( i = 0; i < 16; i++ )
    {
        reg[i] = 0;
    }
    // keeps track of new activation records and their lengths
    int ActRecLen[20];
    int curActRec = 0;
    
    int halt = 0;   // set halt flag to 0
    
    
    FILE *ifp = fopen("code.txt", "rb");
    
    // exit program if file not found
    if (ifp == NULL) {
        printf("Code for virtual machine not found\n");
        exit(1);
    }
    
	// Read the file and put it into the code memory
	while (fscanf(ifp, "%d", &buffer) != EOF)
    {
        code[counter % 4][counter/4] = buffer;
        
        counter++;
        
    }// end while loop
    
    // keep track of the # of lines of code
    codeLen = counter / 4;
    
    fclose (ifp);
    
    /*      used for debugging:
    // printout code to make sure reads in correctly.
    for ( i = 0; i < codeLen; i++)
    {
        for ( j = 0; j < 4; j++ )
            printf("%d ", code[j][i]);
        printf("\n");
        
    }
     */
    
    FILE *ofp = fopen("stacktrace.txt", "w+");
    
    // print the code to file
    printCode(ofp, code, codeLen);
    
    
    // print headers for the stack info
    fprintf(ofp, "\t\t\t\t\tPC\tBP\tSP\tstack \n");
    fprintf(ofp, "Initial values \t\t\t\t%d\t", PC);
    fprintf(ofp, "%d\t", basePtr);
    fprintf(ofp, "%d\t", stackPtr);
    fprintf(ofp, "%d\n", stack[0]);
    
    
    // fetch and execute cycle
    while ( !halt )
    {
        line = PC;
        // comment out next line - used for debugging:
        //printf("line: %d\n", line);
        
        // fetch the next line of code
        ir = fetch( PC, code );
    
        // increment the program counter
        PC = PC + 1;
        
        // execute cycle
        switch ( ir.op )
        {
        case 1: // LIT
            reg[ir.r] = ir.m;
            break;
        case 2: //  RTN
            stackPtr = basePtr - 1;
            basePtr = stack[stackPtr + 3];
            PC = stack[stackPtr + 4];
            curActRec--;
            break;
        case 3: //  LOD
            reg[ir.r] = stack[ base(ir.l, basePtr, stack) + ir.m ];
            break;
        case 4: // STO
            stack[ base(ir.l, basePtr, stack) + ir.m] = reg[ir.r];
            break;
        case 5: // CAL
            stack[stackPtr + 1] = 0;
            stack[stackPtr + 2] = base(ir.l, basePtr, stack);
            stack[stackPtr + 3] = basePtr;
            stack[stackPtr + 4] = PC;
            basePtr = stackPtr + 1;
            PC = ir.m;
            break;
        case 6: // INC
            stackPtr = stackPtr + ir.m;
            ActRecLen[curActRec+1]=ir.m + ActRecLen[curActRec];
            curActRec++;
            break;
        case 7: // JMP
            PC = ir.m;
            break;
        case 8: //  JPC
            if ( reg[ir.r] == 0 ){
                PC = ir.m;
            }
            break;
        case 9: //  SIO R, 0, 1
            printf("%d\n", reg[ir.r] );
            break;
        case 10: // SIO R, 0, 2
            printf("Enter value: \t");
            scanf("%d", &reg[ir.r] );
            break;
        case 11:    // SIO R, 0, 3
            halt = 1;
            break;
        case 12:    // NEG
            reg[ ir.r ] = - reg[ir.l];
            break;
        case 13:    // ADD
            reg[ir.r] = reg[ir.l] + reg[ir.m];
            break;
        case 14:    // SUB
            reg[ir.r] = reg[ir.l] - reg[ir.m];
            break;
        case 15:    // MUL
            reg[ir.r] = reg[ir.l] * reg[ir.m];
            break;
        case 16:    // DIV
            reg[ir.r] = reg[ir.l] / reg[ir.m];
            break;
        case 17:    // ODD
            reg[ir.r] = reg[ir.r] % 2;
            break;
        case 18:    // MOD
            reg[ir.r] = reg[ir.l] % reg[ir.m];
            break;
        case 19:    // EQL
            reg[ir.r] = reg[ir.l] == reg[ir.m];
            break;
        case 20:    // NEQ
            reg[ir.r] = reg[ir.l] != reg[ir.m];
            break;
        case 21:    // LSS
            reg[ir.r] = reg[ir.l] < reg[ir.m];
            break;
        case 22:    // LEQ
            reg[ir.r] = reg[ir.l] <= reg[ir.m];
            break;
        case 23:    // GTR
            reg[ir.r] = reg[ir.l] > reg[ir.m];
            break;
        case 24:    // GEQ
            reg[ir.r] = reg[ir.l] >= reg[ir.m];
            break;
        default:
            printf("Invalid op code.\n");
            exit(1);
            
        }
        
        
        // print registers
        fprintf(ofp, "%d\t", line );
        fprintf(ofp, "%s\t", getOpCode(ir.op) );
        fprintf(ofp, "%d\t", ir.r);
        
        fprintf(ofp, "%d\t", ir.l);
        fprintf(ofp, "%d\t", ir.m);
        fprintf(ofp, "%d\t", PC);
        fprintf(ofp, "%d\t", basePtr);
        fprintf(ofp, "%d\t", stackPtr);
        
        // print the stack
        i=1;
        for( j = 0; j <= stackPtr; j++)
        {
            fprintf(ofp, "%d ", stack[j] );
            // print out separators btwn the activation records
            if ( j == ActRecLen[i] && j != stackPtr )
            {
                fprintf(ofp, "| ");
                i++;
            }
        }
        fprintf(ofp, "\n");
        
        
        // set halt flag to 1 on the end condition
        if ( stackPtr == 0 && basePtr == 0 && PC == 0)
        {
            halt = 1;
        }
        
    }// end while loop
    
    
    fclose(ofp);
    

    return 0;
    
}// end function main


// prints the code to the output file
void printCode( FILE *ofp, int code[][MAX_CODE_LENGTH], int codeLength )
{
    int i, j;
    
    fprintf(ofp, "line\tOP\tR\tL\tM\n");
    
    for ( i = 0; i < codeLength; i++)
    {
        fprintf(ofp, "%d\t", i);
        fprintf(ofp, "%s\t", getOpCode(code[0][i]) );
        for ( j = 1; j < 4; j++)
        {
            fprintf(ofp, "%d\t", code[j][i]);
        }
        fprintf(ofp, "\n");
    }
    
    fprintf(ofp, "\n");
}// end function printCode()


// get the new instruction record from the code
instruction fetch( int programCounter, int code[][MAX_CODE_LENGTH] )
{
    instruction currentCode;
    
    currentCode.op = code[0][programCounter];
    currentCode.r = code[1][programCounter];
    currentCode.l = code[2][programCounter];
    currentCode.m = code[3][programCounter];
    
    return currentCode;
}// end function fetch


// function given as part of the assignment specs
int base( int l, int base, int *stack )
{
    int b1;
    b1 = base;
    while ( l > 0 )
    {
        b1 = stack[b1 + 1];
        l--;
    }
    
    return b1;
}// end function base


// returns the opCode String associated with the given opCode int
char* getOpCode( int opCode )
{
    switch ( opCode )
    {

    case 1:
        return "LIT";
    case 2:
        return "RET";
    case 3:
        return "LOD";
    case 4:
        return "STO";
    case 5:
        return "CAL";
    case 6:
        return "INC";
    case 7:
        return "JMP";
    case 8:
        return "JPC";
    case 9:
        return "OUT";
    case 10:
        return "SIO";
    case 11:
        return "HLT";
    case 12:
        return "NEG";
    case 13:
        return "ADD";
    case 14:
        return "SUB";
    case 15:
        return "MUL";
    case 16:
        return "DIV";
    case 17:
        return "ODD";
    case 18:
        return "MOD";
    case 19:
        return "EQL";
    case 20:
        return "NEQ";
    case 21:
        return "LSS";
    case 22:
        return "LEQ";
    case 23:
        return "GTR";
    case 24:
        return "GEQ";
    default:
       return "error";
    }// end switch statement
}// end function getOpCode