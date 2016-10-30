#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

//symbol symbol_table[MAX_SYMBOL_TABLE_SIZE]; // currently here cause on slide, not syet used in program

typedef struct 
{
	int op; //opcode
	int l;	// level
	int m;	// modifier
}current_intstuction;

//current_instruction instructions[MAX_CODE_SIZE]; 	// data structure for instructions and code generation








int main()
{

	printf("i am the parser");
	printf("the ai compiler accepts, make it so");





#include "tokens.h"

TokenType tok;

//Function Prototypes
void Statement();
void Condition();
void Expression();


int main()
{
	return 0;
}

void term()
{
	Factor();
	while(tok == "multsym" || tok == "slashsym")
	{
		get(tok);
		factor();
	}
	return;
}


void factor()
{
	if(tok == "identsym")
		get(tok);
	else if(tok == number)
		get(tok);
	else if(tok == "(")
	{
		get(tok);
		expression();
		if(tok != ")")
			return; // error here, need to see how this is handled
		get(tok); // part of nested if? or part of last else if?
	}
	else
		return; //error needs to be handled here
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
    if (tok = oddsym)
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
    if(tok = plussym || tok = minussym)
        Term();
    while(tok = plussym || tok = minussym)
    {
        tok = Get();
        Term();
    }
}
