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
}Token;



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "tokens.h"

//PROTOTYPES
void InitializeReservedWords();
void InitializeReservedOperators();
void ImportSourceCode(FILE * ifp);
void PrintOutInputFile(FILE * ifpFGETS);
void PrintOutInputFileNoComments(FILE * ifpNOCOMMENTS);


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
    FILE * ifpFGETS = fopen("input.txt", "r");
    FILE * ifpNOCOMMENTS = fopen("input.txt", "r");



    ImportSourceCode(ifp);
    PrintOutInputFile(ifpFGETS);
    printf("\n\n");
    PrintOutInputFileNoComments(ifpNOCOMMENTS);

    if(ifp == NULL)
    {
        printf("Could not open file");
    }
    else
    {
         ImportSourceCode(ifp);
    }

    return 0;
}

void PrintOutInputFile(FILE * ifpFGETS)
{
    char line[100];
    while(fgets(line, 100, ifpFGETS)) 
    {
        printf("%s", line);
    }

}

void PrintOutInputFileNoComments(FILE * ifpNOCOMMENTS)
{
    char lineNOCOMMENTS[100];
    while(fgets(lineNOCOMMENTS, 100, ifpNOCOMMENTS)) 
    {
        if (strstr(lineNOCOMMENTS, "/*") != NULL) 
        {
            //char str[] = "ID is a sample string remove to /0.10";
            char *a = strstr(lineNOCOMMENTS, "/*");
            char *b = strstr (lineNOCOMMENTS, "*/");
            if ((a == NULL) || (b == NULL) || (b < a))
            {
                exit(0);
            }

            memmove(a, b+2, strlen(b)+1);
        }       

        printf("%s", lineNOCOMMENTS);

    }
}

void ParseSourceCode()
{
    int i;
    int j;
    int tokenLength;
    for (i = 0; i < numTokens; i++)
    {
        tokenLength = strlen(tokenCollection[i]);
        for (j = 0; j < tokenLength; j++)
        {
            //switch ()
        }
    }
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
