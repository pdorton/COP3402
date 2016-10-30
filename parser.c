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