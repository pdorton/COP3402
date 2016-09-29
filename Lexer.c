/*
COP 3402 Lexer Project
Patrick Dorton
Michael Garro
David Almeida
Andrew Maida
*/

typedef enum token {
  nulsym = 1, identsym, numbersym, plussym, minussym,
  multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
  gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
  periodsym, becomessym, beginsym, endsym, ifsym, thensym,
  whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
  readsym , elsesym
} token_type;


#include <stdio.h>
#include <string.h>
//#include "tokens.h"


#define MAX_ID_LENGTH 12
#define MAX_INTEGER 65535

char* RESERVED_WORDS[14];
char* tokenCollection[10000];
int numTokens = 0;

void main(int argc, char *argv[])
{
    //Initialize the reserved words into the array
    InitializeReservedWords();

    FILE * ifp = fopen("input.txt", "r");


    return;
}



void ImportSourceCode(FILE * ifp)
{
    char token[100];
    while(fscanf(ifp, "%c", token) != EOF)
    {
        if (strlen(token) < MAX_ID_LENGTH)
        {
            strcpy(tokenCollection[numTokens], token);
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
