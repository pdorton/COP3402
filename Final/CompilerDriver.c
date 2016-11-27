/*
Compiler Builder Group 30
Patrick Dorton
David Almeida
Andrew Maida
Michael Garro
 */




#include <stdlib.h>
#include <string.h>
#include <stdio.h>


// GLOBAL variables
int printLexeme;
int printAssemblyCode;
int printVMTrace;

//function definitions
void printLexemeToScreen();
void printAssemblyCodeToScreen();
void printVMTraceToScreen();
void parseDirectives( int argc, char** argv );



int main(int argc, char **argv)
{
    printLexeme = 0;
    printAssemblyCode = 0;
    printVMTrace = 0;
    
    // system call strings
    char callLex[30];
    char callParser[30];
    char callVM[30];
    
    // initialize system call strings
    strcpy( callLex, "./LexicalAnalyzer.exe");
    strcpy( callParser, "./Parser.exe");
    strcpy( callVM, "./VirtualMachine.exe");

    // parses the directives
    parseDirectives( argc, argv );
    
    if ( printAssemblyCode )
        strcat( callParser, " -a" );
    
    // system calls taken from the recitation slides
    int ret = system( callLex );
	if (ret < 0 || WEXITSTATUS(ret) != EXIT_SUCCESS)
    {
		// Something failed. Better stop.
		exit(EXIT_FAILURE);
	}
    
	// Everything is OK. Keep going
	ret = system( callParser );
    
    if (ret < 0 || WEXITSTATUS(ret) != EXIT_SUCCESS)
    {
		// Something failed. Better stop.
		exit(EXIT_FAILURE);
	}
    
	// Everything is OK. Keep going
	ret = system( callVM );
    
    
    // if system calls, call appropriate function to print to screen
    if ( printLexeme )
    {
        printLexemeToScreen();
    }
    
    if ( printAssemblyCode )
    {
        printAssemblyCodeToScreen();
    }
    
    if ( printVMTrace )
    {
        printVMTraceToScreen();
    }
}// end function main


// parses the directives and changes global variables accordingly
void parseDirectives(int argc, char** argv)
{
	int i;
    
    // iterate through each directive checking for valid ones
    for(i = 1; i < argc; i++)
    {
	    if (strcmp( argv[i], "-l" ) == 0)
        {
			printLexeme = 1;
		}
        else if (strcmp( argv[i], "-a" ) == 0)
        {
			printAssemblyCode = 1;
		}
        else if (strcmp( argv[i], "-v" ) == 0)
        {
			printVMTrace = 1;
		}
        else  // if bad directive, print error and ignore
        {
			printf("Bad directive: %s will be ignored\n", argv[i]);
        }
	}// end for loop through argc
}// end function parseDirectives


// prints the lexeme list and symbol table to the screen
void printLexemeToScreen()
{
    FILE *lsfp = fopen( "lexemelist.txt", "r" );
    
    printf("\n\nPrinting out token representation of the input file:\n");
    int c;
    
    // exit program if file not found
    if (lsfp == NULL) {
        printf("\Error finding Lexeme List file output\n\n");
        exit(1);
    }
    
    while ( (c=fgetc(lsfp)) != EOF )
        printf("%c", c );
    
    printf("\n\n");
    fclose(lsfp);
    
    FILE *stfp = fopen( "symboltable.txt", "r" );
    
    printf("\n\nPrinting out the symbol table:\n");
    
    // exit program if file not found
    if (lsfp == NULL) {
        printf("\Error finding symbol table file output\n\n");
        exit(1);
    }
    
    while ( (c=fgetc(stfp)) != EOF )
        printf("%c", c );
    
    printf("\n\n");
    fclose(stfp);
    
}// end function printLexemeToScreen


// prints out the assembly code to the screen
void printAssemblyCodeToScreen()
{
    FILE *parfp = fopen( "code.txt", "r" );
    int c;
    
    printf("\n\nPrinting out Parser / Code Gen output:\n\n");
    
    // exit program if file not found
    if (parfp == NULL) {
        printf("\Error finding Parser output\n");
        exit(1);
    }
    
    while ( (c=fgetc(parfp)) != EOF )
        printf("%c", c );
    printf("\n\n");
    fclose(parfp);
}// end function printAssembyCodeToScreen


// prints out the virtual machine trace to the screen
void printVMTraceToScreen()
{
    FILE *vmfp = fopen( "stacktrace.txt", "r" );
    int c;
    
    printf("\n\nPrinting out Virtual Machine output:\n\n");
    
    // exit program if file not found
    if (vmfp == NULL) {
        printf("\nError finding VM output\n");
        exit(1);
    }
    
    while ( (c=fgetc(vmfp)) != EOF )
        printf("%c", c );
    printf("\n\n");
    fclose(vmfp);
}// end function printVMTraceToScreen

