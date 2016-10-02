/*
COP 3402 Lexer Project
Patrick Dorton
Michael Garro
David Almeida
Andrew Maida
*/

typedef enum token {
  nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,
  multsym = 6, slashsym = 7, oddsym = 8, eqsym = 9, neqsym = 10, lessym = 11, leqsym = 12,
  gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,
  periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24,
  whilesym = 25, dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym = 31,
  readsym =  32, elsesym = 33 
} TokenType;

typedef struct myToken
{
    TokenType type;
    char string[14];
}Token



#include <stdio.h>
#include <string.h>
//#include "tokens.h"

//PROTOTYPES
void InitializeReservedWords();
void InitializeReservedOperators();
void ImportSourceCode(FILE * ifp);


#define MAX_ID_LENGTH 12
#define MAX_INTEGER 65535

char* RESERVED_WORDS[14];
char* RESERVED_OPERATORS[16];
char* tokenCollection[10000];
int numTokens = 0;

int main(int argc, char *argv[])
{
    //Initialize the reserved words into the array
    InitializeReservedWords();
    InitializeReservedOperators();

    FILE * ifp = fopen("input.txt", "r");

    ImportSourceCode(ifp);
    return 0;
}



void ImportSourceCode(FILE * ifp)
{
    char token[100];
    while(fscanf(ifp, "%s", token) != EOF)
    {
        if (strlen(token) < MAX_ID_LENGTH)
        {
            tokenCollection[numTokens] = token;
            numTokens++;
        }
    }
}


//Instantiates the array of reserved words of the PL/0 Language
void InitializeReservedWords()
{
    //Initialize the reserved words into the array
    RESERVED_WORDS[0] = "const";
    RESERVED_WORDS[1] = "var";
    RESERVED_WORDS[2] = "procedure";
    RESERVED_WORDS[3] = "call";
    RESERVED_WORDS[4] = "begin";
    RESERVED_WORDS[5] = "end";
    RESERVED_WORDS[6] = "if";
    RESERVED_WORDS[7] = "then";
    RESERVED_WORDS[8] = "else";
    RESERVED_WORDS[9] = "while";
    RESERVED_WORDS[10] = "do";
    RESERVED_WORDS[11] = "read";
    RESERVED_WORDS[12] = "write";
    RESERVED_WORDS[13] = "odd";
}

//Instantiates the array of reserved operators of the PL/0 Language
void InitializeReservedOperators()
{
    //Initialize the reserved words into the array
    RESERVED_OPERATORS[0] = "+";
    RESERVED_OPERATORS[1] = "-";
    RESERVED_OPERATORS[2] = "*";
    RESERVED_OPERATORS[3] = "/";
    RESERVED_OPERATORS[4] = "=";
    RESERVED_OPERATORS[5] = "<>";
    RESERVED_OPERATORS[6] = "<=";
    RESERVED_OPERATORS[7] = "<";
    RESERVED_OPERATORS[8] = ">=";
    RESERVED_OPERATORS[9] = ">";
    RESERVED_OPERATORS[10] = ":=";
    RESERVED_OPERATORS[11] = ",";
    RESERVED_OPERATORS[12] = ";";
    RESERVED_OPERATORS[13] = ".";
    RESERVED_OPERATORS[14] = "(";
    RESERVED_OPERATORS[15] = ")";
}
