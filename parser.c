/*Contributors:
David Almeida
Micheal Garro
Andrew Maida
Patrick Dorton
*/

// parser assignment due nov 10



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokens.h"

#define MAX_SYMBOL_TABLE_SIZE 100
#define MAX_CODE_SIZE 500

/*Symbol Data Structure table*/
typedef struct symbol 
{
	int kind;			// const = 1, var = 2, proc = 3
	char name[1];		// name up to 11 characters
	int val;			// number (ACSII Value)
	int level;			// L level
	int adr;			// M address
}	symbol;

//

typedef struct 
{
	int op; //opcode
	int l;	// level
	int m;	// modifier
}current_intstuction;

//

TokenType tok;
current_instruction instructions[MAX_CODE_SIZE];    // data structure for instructions and code generation
symbol symbol_table[MAX_SYMBOL_TABLE_SIZE]; // currently here cause on slide, not syet used in program
int pc = 0;
int var = 0;
int currentLevel = 0;
int totalSymbol = 0;
int error = 0; // gets changed to 1 if error encountered


//Function Prototypes
void Statement();
void Condition();
void Expression();
void Term();
void Factor();
void emit();
void error();



int main()
{
	return 0;
}

	



void Program()
{

 	tok = Get();
 	Block();
 	if (tok != periodsym)
 	{
 		printf("Period expected");
 		return;
 	}
 		

} 

void Block()
{
 	if (tok == constsym)
 	{
 			/*	
				Const x = 9, y = x;	
				while not a semicolon 
				check for ident then = then numbers
 						*/
			tok = get(tok);// get the first token after the const 
 			while(tok != semicolomsym)
 			{
 				
 				if (tok != identsym)
 				{// if first token is not ident then throw error 
 					printf("const, var, procedure must be followed by identifier");
 					return;
 				}
 				else
 				{// bring in the next token
 					tok = get(tok);
 				}
 				
 				
 				if (tok != eqsym)
 				{// if token isn't an "="
 					printf("Identifier must be followed by =");
 					return;
 				}
 				else
 				{

 				}
 				tok = get(tok);
 				else if (tok != numsym ) ERROR;
 				tok = get(tok);

 				//until tok != "commasym";
 				if tok != semicolomsym then ERROR;
 				tok = get(tok);
 			
 			
 		}
 	if tok == varsym then 
 		{

 			repeat
 			tok = get(tok);
 			if tok != identsym then ERROR;
 			tok = get(tok);
 			until tok != commasym;
 			if tok != semicolomsym then ERROR;
 			tok = get(tok);
 		}
 	while tok == procsym do 
 		{
 			tok = get(tok);
 			if tok != identsym then ERROR;
 			tok = get(tok);
 			if tok != semicolomsym then ERROR;
 			tok = get(tok);
 			BLOCK;
 			if tok != semicolomsym then ERROR;
 			tok = get(tok);
 		}
 		STATEMENT
 		
}





void Statement()
{
    if (tok == identsym)
    {
        //tok = get();
        if (tok != becomessym)
        {
            //error: printf("Assignment operator expected");
            return;
        }
        //tok = get();
        //Expression();
    }
    else if (tok == callsym)
    {
        //tok = get();
        if (tok != identsym)
        {
            //error: printf("call must be followed by an identifier.");
            return;
        }
        //tok = get();
    }
    else if (tok == beginsym)
    {
        //tok = get();
        Statement();
        while (tok == semicolonsym)
        {
            //tok = get();
            Statement();
        }
        if (tok != endsym)
        {
            //error: printf("Period expected");
            return;
        }
        //tok = get();
    }
    else if (tok == ifsym)
    {
        //tok = get();
        //Condition();
        if (tok != thensym)
        {
            //error: printf("then expected.");
            return;
        }
        //tok = get();
        Statement();
    }
    else if (tok == whilesym)
    {
        //tok = get();
        //Condition();
        if (tok != dosym)
        {
            //error: printf("do expected.");
            return;
        }
        //tok = get();
        Statement();
    }
}

void Condition()
{
    if (tok == oddsym)
    {
        tok = Get();
        Expression();
    }
    else
    {
        Expression();

        //is it <, >, =, etc.
        //add this later
        if(tok != Relation)
            Error();

        tok = get();
        Expression();
    }
}

void Expression()
{
    if(tok == plussym || tok == minussym)
        Term();
    while(tok == plussym || tok == minussym)
    {
        tok = Get();
        Term();
    }
}

// david is working on error handling for his two functions

// function desgined to catch term errors
void Term()
{
	Factor();
	while(strcmp(tok, "6") == 0 || strcmp(tok, "7") == 0)  //(tok == "multsym" || tok == "slashsym")
	{
        char temp[12];
        strcpy(temp, tok);
		tok = get();
		Factor();

        if(strcmp(temp, "6") == 0)
            emit(2, 0, 4);
        else
            emit(2, 0, 5)

	}
}// function should be mostly complete

// functinn desgined to catch factor errors
void Factor()
{
    int sym_index = 0:
	if(strcmp(tok, "2") == 0)  //(tok == "identsym")
    { 
		tok = get();
        sym_index = searchSymbolTable(tok);

        if(sym_index == -1)
            error("undeclared identifier.\n");

        if(symbol_table[sym_index] == 1)
            emit(1, 0, symbol_table[sym_index].val);

        else if (symbol_table[sym_index] == 2)
            emit(1,0,symbol_table[sym_index].val);
        else
            error("Expression mus not contain a procedure identifier.\n");
        tok = get();
    }
	else if(strcmp(tok, "3") == 0) //(tok == number)
	{
        tok = get();
        int num = atoi(tok);
        emit(1, 0, num);
        tok = get();
    }

	else if(strcmp(tok, "15") == 0)//(tok == "(")
	{
		tok = get();
		Expression();
		if(strcmp(tok, "16") != 0)//(tok != ")")
            error("Right Parenthesis Missing.\n");
		tok = get(); // part of nested if? or part of last else if?
	}
	else
		error("An exppression cannot begin with this symbol.\n"); //error needs to be handled here
}

void emit(int op, int level, int modifier)
{
    if(pc >= MAX_CODE_SIZE)
        error("Generated code exceeds max code size.\n");

    instructions[pc].op = op;
    instructions[pc].l = level;
    instructions[pc].m = modifier;
    pc++;
}

void error(char* s) // prints error and exit parser
{
    errorDetect = 1; // an error was encountered
    fprintf(output, "\nError: %s\n", s);
    printf("\nError: %s\n", s);
}