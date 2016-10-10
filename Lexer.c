/*
COP 3402 Lexer Project
Patrick Dorton
Michael Garro
David Almeida
Andrew Maida
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "tokens.h"

/*
typedef enum token {
  nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,
  multsym = 6, slashsym = 7, oddsym = 8, eqsym = 9, neqsym = 10, lessym = 11, leqsym = 12,
  gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,
  periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24,
  whilesym = 25, dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym = 31,
  readsym =  32, elsesym = 33
} TokenType;
*/
typedef struct myToken
{
    TokenType type;
    char string[14];
}Token;


//PROTOTYPES
void InitializeReservedWords();
void InitializeReservedOperators();
void ImportSourceCode(FILE * ifp);
void PrintOutInputFile(FILE * ifpFGETS);
void PrintOutInputFileNoComments(FILE * ifpNOCOMMENTS);
TokenType FindTokenType( char* token);
char* chop(char* raw);

#define MAX_ID_LENGTH 13
#define MAX_INTEGER 65535

char* RESERVED_WORDS[14];
char* RESERVED_OPERATORS[16];
char* tokenCollection[10000];
int numTokens = 0;

int main(int argc, char *argv[])
{
    int commentFlag = 0;
    int i = 0;
    int showSource = 0;
    int showClean = 0;

    //Initialize the reserved words into the array
    InitializeReservedWords();
    InitializeReservedOperators();

    FILE * ifp = fopen("input.txt", "r");
    FILE * ifpFGETS = fopen("input.txt", "r");
    FILE * ifpNOCOMMENTS = fopen("input.txt", "r");

    for(i=0 ; i<argc ; i++)
    {
        if(strcmp(argv[i], "--source") == 0)
            showSource = 1;
        if(strcmp(argv[i], "--clean") == 0)
            showClean = 1;

    }

    //ImportSourceCode(ifp);

    if(showSource)
    {
        printf("source code:\n------------\n");
        PrintOutInputFile(ifpFGETS);
    }


    //ImportSourceCode(ifp);

    if(showClean)
    {
        printf("\n\nsource code without comments:\n-----------------------------\n");
        PrintOutInputFileNoComments(ifpNOCOMMENTS);
    }

    printf("\n\ntokens:\n-------\n");

    if(ifp == NULL)
    {
        printf("Could not open file.");
        return 0;
    }
    else
    {
         //ImportSourceCode(ifp);
    }




    char tokenInput[100];
    while(fscanf(ifp, "%s", tokenInput) != EOF)
    {

        if (strlen(tokenInput) < MAX_ID_LENGTH) //If the length of the token is within the limit of 12 chars plus one symbol at the end
        {

            if(strcmp(tokenInput, "/*") == 0) // if the beginning of a comment
            {   //printf("got to comment section\n");
                while(strcmp(tokenInput, "*/") != 0 )// until the token reads in the end of comment symbol read through tokens
                {
                	//printf("read in %s\n", tokenInput);
                    fscanf(ifp, "%s", tokenInput);
                }

                //printf("read in the whole comment and now %s is the token\n", tokenInput);
            }

            else if(!isalnum(tokenInput[strlen(tokenInput) -1 ] )) // if the last char is a special character
            {
                if(strlen(tokenInput) < 3)// if the token is length 2 or lower
                {
                    if(!isalnum(tokenInput[0]))//check if the first char is a special character
                    {// covers both 1 and 2 length special characters
                        printf("%s\t\t\t%d\n", tokenInput, (int)FindTokenType(tokenInput));
                    }
                    else// tear them apart and send each then move to next token
                    {

                    	char* buffer0[1];
                        buffer0[0] = tokenInput[0];
						//printf("buffer0 = %s", &buffer0);
                    	char* buffer1[1];
                    	buffer1[0] = tokenInput[1];


                    	//printf("sending %s and %s   \n" , &buffer0 , &buffer1);
                        printf("%s\t\t\t%d\n", buffer0, (int)FindTokenType(buffer0));
                        printf("%s\t\t\t%d\n", buffer1, (int)FindTokenType(buffer1));
                    }
                }
                else if(isdigit(tokenInput[0]))// if the first char is a digit, then check all chars to see find a alpha
                {

                    int i;
                    for(i = 0 ; i < strlen(tokenInput) -1; i++)
                    {// for every character except the last
                        if(isalpha(tokenInput[i]))
                        {// if the word contains a character after starting with a number
                            printf("Invalid identifier %s", tokenInput);
                            return 0;
                        }
                    }
                    //all chars are digits

                    if(strlen(tokenInput) > 7)//7 is to account for the special character as well as the length of the digit
                    {
                        printf("Integer digit Overflow");
                        return 0;
                    }
                    else if(strlen(tokenInput) == 7)// if the digit is 6 characters long
                    {
                        if(atoi(chop(tokenInput)) > MAX_INTEGER)// check if over max allowed value
                        {
                            printf("Integer Overflow");
                            return 0;
                        }
                        else// is valid number
                        {

                        	char* buffer1[1];
                    		buffer1[0] = tokenInput[strlen(tokenInput) - 1];// take the last char

                            printf("%s\t\t\t%d\n", chop(tokenInput), (int)FindTokenType(chop(tokenInput)));
                            printf("%s\t\t\t%d\n", buffer1, (int)FindTokenType(buffer1));
                        }
                    }
                    else// if the token is not longer than 6 characters chop and print
                    {

                    	char* buffer1[1];
                    	buffer1[0] = tokenInput[strlen(tokenInput) - 1];// take the last char
                      printf("%s\t\t\t%d\n", chop(tokenInput), (int)FindTokenType(chop(tokenInput)));
                      printf("%s\t\t\t%d\n", buffer1, (int)FindTokenType(buffer1));
                    }
                }
                else // if the length of the token  is longer than 2 and starts with an alpabet character then it will be a variable or keyword
                {
                	char* buffer1[1];
                    	buffer1[0] = tokenInput[strlen(tokenInput) - 1];// take the last char
                    printf("%s\t\t\t%d\n", chop(tokenInput), (int)FindTokenType(chop(tokenInput)));
                    printf("%s\t\t\t%d\n", buffer1, (int)FindTokenType(buffer1));
                }
            }
                else if(isdigit(tokenInput[strlen(tokenInput) -1]))// if last char is a digit
                {
                    if(isdigit(tokenInput[0]))// check if first is a digit
                    {
                        int i;
                        for(i = 0 ; i < strlen(tokenInput) -1; i++)
                        {// for every character except the last
                            if(isalpha(tokenInput[i]))
                            {// if the word contains a character after starting with a number
                                printf("Invalid identifier %s", tokenInput);
                                return 0;
                            }
                        }
                        //all chars are digits
                        if(strlen(tokenInput) > 6)
                        {
                            printf("Integer digit Overflow");
                            return 0;
                        }
                        else if(strlen(tokenInput) == 6)// if the digit is 6 characters long
                        {
                            if(atoi(chop(tokenInput)) > MAX_INTEGER)// check if over max allowed value
                            {
                                printf("Integer Overflow");
                                return 0;
                            }
                            else// is valid number
                            {

                            	char* buffer1[1];
                    			buffer1[0] = tokenInput[strlen(tokenInput) - 1];// take the last char
                                printf("%s\t\t\t%d\n", chop(tokenInput), (int)FindTokenType(chop(tokenInput)));
                                printf("%s\t\t\t%d\n", buffer1, (int)FindTokenType(buffer1));
                            }

                        }
                    }
                    else if(isalpha(tokenInput[0])) // if first character is a letter
                    {// then it is a identitier

                        printf("%s\t\t\t%d\n", tokenInput, (int)FindTokenType(tokenInput));
                    }

                }
                else if(isalpha(tokenInput[strlen(tokenInput) -1 ] ))// if the last character is a char then it is either a ident or a keyword
                {
                    if(isdigit(tokenInput[0]))// then invalid ident
                    {
                        printf("Invalid identifier %s", tokenInput);
                        return 0;
                    }
                    else // is valid token
                    {
                        printf("%s\t\t\t%d\n", tokenInput, (int)FindTokenType(tokenInput));
                    }
                }
            }
            else if(isdigit(tokenInput[strlen(tokenInput) -1 ] ))// if the last char is a digit
            {
                if(isalpha(tokenInput[0]))// if the first char is a character then it is an ident and send it through
                {
                    printf("%s\t\t\t%d\n", tokenInput, (int)FindTokenType(tokenInput));
                }
                else if(isdigit(tokenInput[0]))// if the first char is a number
                {
                       int i;
                        for(i = 0 ; i < strlen(tokenInput) -1; i++)
                        {// for every character except the last
                            if(isalpha(tokenInput[i]))
                            {// if the word contains a character after starting with a number
                                printf("Invalid identifier %s", tokenInput);
                                return 0;
                            }
                        }
                        //all chars are digits

                        if(strlen(tokenInput) > 6)
                        {
                            printf("Integer digit Overflow");
                            return 0;
                        }
                        else if(strlen(tokenInput) == 6)// if the digit is 6 characters long
                        {
                            if(atoi(chop(tokenInput)) > MAX_INTEGER)// check if over max allowed value
                            {
                                printf("Integer Overflow");
                                return 0;
                            }
                            else// is valid number
                            {
                            	char* buffer1[1];
                    			buffer1[0] = tokenInput[strlen(tokenInput) - 1];// take the last char
                                printf("%s\t\t\t%d\n", chop(tokenInput), (int)FindTokenType(chop(tokenInput)));
                                printf("%s\t\t\t%d\n", buffer1, (int)FindTokenType(buffer1));
                            }
                        }
                        else// if the token is not longer than 6 characters chop and print
                        {
                        	char* buffer1[1];
                    		buffer1[0] = tokenInput[strlen(tokenInput) - 1];// take the last char
                        printf("%s\t\t\t%d\n", chop(tokenInput), (int)FindTokenType(chop(tokenInput)));
                        printf("%s\t\t\t%d\n", buffer1, (int)FindTokenType(buffer1));
                        }
                }
            }
        else
        {
            printf("Identifier length exceeded maximum size.");
            return 0;
        }
        //printf("%s\t\t\t%d\n", tokenInput, (int)FindTokenType(tokenInput));

		memset(tokenInput,0,sizeof(tokenInput));
    }// end of while loop

    return 0;
}// end of main

// read in each token and find out what kind of valid token it is
TokenType FindTokenType( char* token)
{
	if(strlen(token) == 1)
	{
		//printf("token = %s\n", token);
	}
	else
	{
		//printf("token = %s\n" , token);
	}

    /*nulsym = 1,   done
    identsym = 2,   done
    numbersym = 3,  done
    plussym = 4,    done
    minussym = 5,   done
  multsym = 6,      done
  slashsym = 7,     done
  oddsym = 8,       done
  eqsym = 9,        done
  neqsym = 10,      done
  lessym = 11,      done
  leqsym = 12,      done
  gtrsym = 13,      done
  geqsym = 14,      done
  lparentsym = 15,  done
  rparentsym = 16,  done
  commasym = 17,    done
  semicolonsym = 18,done
  periodsym = 19,   done
  becomessym = 20,  done
  beginsym = 21,    done
  endsym = 22,      done
  ifsym = 23,       done
  thensym = 24,     done
  whilesym = 25,    done
  dosym = 26,       done
  callsym = 27,     done
  constsym = 28,    done
  varsym = 29,      done
  procsym = 30,     done
  writesym = 31,    done
  readsym =  32,    done
  elsesym = 33      done*/


// need to write code to do the comment handling



  if(strlen(token) == 1)
  {// if the token is a single character
  	//printf("token is a single char\n");
    if(isdigit(token[0]))
    {
        return numbersym;
    }
    else if(isalpha(token[0]))
    {
    	//printf("%c is an alpla\n", token[0]);
        return identsym;
    }
    else
        switch(token[0])
        {
            case '+':
                return plussym;
            case '-':
                return minussym;
            case '*':
                return multsym;
            case '/':
                return slashsym;
            case '(':
                return lparentsym;
            case ')':
                return rparentsym;
            case ';':
                return semicolonsym;
            case '.':
                return periodsym;
            case '=' :
                return eqsym;
            case ',' :
                return commasym;
            case '<' :
                return lessym;
            case '>' :
                return geqsym;
            default  :
                //if not any of the single symbols
                printf("illegal token\n");
                return nulsym;

        }

  }// end of if length 1 tokens

  else if(strlen(token) > 1)
  {// if longer than 1 char
    if(isdigit(token[0]))
    {//if the first char is a digit
        return numbersym;
    }
    else if(isalpha(token[0]))
    {// if it is a word check all keywords
            if(strcmp(token, "const") == 0)
            {
                return constsym;
            }
            else if(strcmp(token, "var") == 0)
            {
                return varsym;
            }
            else if(strcmp(token, "procedure") == 0)
            {
                return procsym;
            }
            else if(strcmp(token, "call") == 0)
            {
                return callsym;
            }
            else if(strcmp(token, "begin") == 0)
            {
                return beginsym;
            }
            else if(strcmp(token, "end") == 0)
            {
                return endsym;
            }
            else if(strcmp(token, "if") == 0)
            {
                return ifsym;
            }
            else if(strcmp(token, "then") == 0)
            {
                return thensym;
            }
            else if(strcmp(token, "else") == 0)
            {
                return elsesym;
            }
            else if(strcmp(token, "while") == 0)
            {
                return whilesym;
            }
            else if(strcmp(token, "do") == 0)
            {
                return dosym;
            }
            else if(strcmp(token, "read") == 0)
            {
                return readsym;
            }
            else if(strcmp(token, "write") == 0)
            {
                return writesym;
            }
            else if(strcmp(token, "odd") == 0)
            {
                return oddsym;
            }
            else
            {
                return identsym;
            }

    }
    else if(!isalnum(token[0]))
    {// multiple character non alphanumeric symbols

        if(strcmp(token, ">=") == 0)
        {
            return geqsym;
        }
        else if(strcmp(token, "<=") == 0)
        {
            return leqsym;
        }
        else if(strcmp(token, ":=") == 0)
        {
            return becomessym;
        }
        else if(strcmp(token, "<>") == 0)
        {
            return neqsym;
        }
        else
        {// non valid multiple character symbols
            printf("illegal token\n");
            return nulsym;
        }


    }
    else
    {
        printf("illegal token\n");
        return nulsym;
    }
  }// end of multiple char tokens


}


//prints out the input file with comments
void PrintOutInputFile(FILE * ifpFGETS)
{
    char line[500];
    //reads in an entire line and prints
    while(fgets(line, 500, ifpFGETS))
    {
        printf("%s", line);
    }

}

//removes comments and prints out the input file contents
void PrintOutInputFileNoComments(FILE * ifpNOCOMMENTS)
{
    char lineNOCOMMENTS[500];
    //reads in every line
    while(fgets(lineNOCOMMENTS, 500, ifpNOCOMMENTS))
    {
        //checks if a comment is present and removes the comment
        if (strstr(lineNOCOMMENTS, "/*") != NULL)
        {
            char *a = strstr(lineNOCOMMENTS, "/*");
            char *b = strstr (lineNOCOMMENTS, "*/");
            if ((a == NULL) || (b == NULL) || (b < a))
            {
                exit(0);
            }

            memmove(a, b+2, strlen(b)+1);
        }

        //prints out the line with no comments
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

char* chop(char* raw)
{

    int length = strlen(raw);
    char* string = (char*)malloc(sizeof(char) * (length -1) );
    int i;
    for(i = 0 ; i < length - 1 ; i++)
    {
        string[i] = raw[i];
    }
    string[length-1] = '\0'; // to attach an end to the string to be returned
    return string;
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
