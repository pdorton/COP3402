/*
    William Jeremy RiCharde
    COP 3402 - Systems Software
    Assignment 2 - Lexical Analyzer
    Due June 20, 2014

    Notes:  This is a lexical analyzer for the PL/O programming language. It 
    reads in text in "input.txt" and tokenizes it based on the PL/0 programming
    language based just on syntax, without consideration to grammar.  It outputs
    a tokenized version of the code in a file named lexemelist.txt and creates 
    a symbol table for all variables, numbers, and procedure names, which it 
    outputs to symboltable.txt.  The main output for the program is written to a
    file named output.txt.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_SYMBOL_TABLE_SIZE 100
#define MAX_IDENTIFIER_LENGTH 11
#define MAX_NUMBER_LENGTH 5

// a struct to store the symbol information
typedef struct
{
    int kind;           // const = 1, var = 2, procedure = 3
    char name[12];      // reg
    int val;            // number (ASCII value)
    int level;          // L level
    int addr;           // M level
} symbol;


// a node that represents a single word or symbol in the program code
typedef struct node
{
    // a variable, number or symbol in the code
    char *word;
    
    // a pointer to the next word in the code
    struct node *next;
    
} node;

// create a enum for the token types
typedef enum {
    nulsym = 1, identsym, numbersym, plussym, minussym,
    multsym,  slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
    gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
    periodsym, becomessym, beginsym, endsym, ifsym, thensym,
    whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
    readsym , elsesym } token_type;


// function declarations
node *BeginsWithLetter( char firstLetter, FILE *inputFile, node* tail,
                       FILE *outputFile );
node *createNode();
node *BeginsWithNumber( char firstDigit, FILE *inputFile, node* tail,
                       FILE *outputFile );
node *BeginsWithSymbol( char firstSymbol, FILE *inputFile, node* tail,
                       FILE *outputFile );
void findLexeme(FILE *outFP, char *text, FILE *lexFP, symbol *table,
                int *numSymbs );

int putInSymbolTable( symbol *symbol_table, char *text, int *numSymbols );


// begin main
int main(void)
{
    
    //char currentLine[100];
    //strcpy( currentLine, "" );
    int c, buffer;
    char letter;
    
    int i = 0;
    int numSymbols = 0;
    
    // begin a linked list to store each word or symbol in a node
    node *head, *tail;
    head = tail = createNode();
    
    
    // declare and initialize the symbol table
    symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
    for ( i = 0; i < MAX_SYMBOL_TABLE_SIZE; i++ )
    {
        symbol_table[i].kind = 0;
        strcpy( symbol_table[i].name,  "");
        symbol_table[i].val = 0;
        symbol_table[i].level = 0;
        symbol_table[i].addr = 0;
    }
    
    // create a pointer for the input file
    FILE *ifp = fopen("input.txt", "rb");
    
    // exit program if file not found
    if (ifp == NULL) {
        printf("\"input.txt\" file not found\n");
        exit(1);
    }
    
    // create and begin writing to the main output file
    FILE *ofp = fopen("lexout.txt", "w+");
    
    fprintf(ofp, "Source Program:\n");
    
    // get the first character from the input file
    c = fgetc(ifp);
    
    
	// Read the first character of each new word.  Creates a node for each
    // symbol, word, or number in the code (excluding comments).  Prints out the
    // code to output, removing comments
	while( c  != EOF ) {
        
        // if the first character is a letter, handle as a letter
        if ( isalpha ( c ) )
        {
            letter = c;
            tail = BeginsWithLetter(c, ifp, tail, ofp );
            c = fgetc(ifp);
        }
        
        // handle differently if the first character is a number
        else if ( isdigit( c ) )
        {
            tail = BeginsWithNumber(c, ifp, tail, ofp );
            c = fgetc(ifp);
        }
        
        // and if the first character is a symbol
        else if ( ispunct( c ) )
        {
            letter = c;
            tail = BeginsWithSymbol(c, ifp, tail, ofp );
            c = fgetc(ifp);
            
        }
        
        // if it is none of these, print it out to maintain white space
        else
        {
            fprintf( ofp, "%c", c );
            c = fgetc(ifp);
        }
    }// end while loop
    
    // close the input file
    fclose (ifp);
    
    // create the output files for the lexeme list and the symbol table
    FILE *lexemeFP = fopen("lexemelist.txt", "w+");
    FILE *symbTabFP = fopen("symboltable.txt", "w+");

    
    //  print the lexime list to the output file and the lexime output file
    // go through each "word" in the linked list code
    fprintf(ofp, "\n\nLexeme Table:\n");
    fprintf(ofp, "lexeme\t\ttoken type\n");
    for (; head->next != NULL; head = head->next )
    {
        fprintf(ofp, "%s\t\t", head->word);
        findLexeme(ofp, head->word, lexemeFP, symbol_table, &numSymbols);
    }

    // close the lexeme file
    fclose(lexemeFP);
    
    
    fprintf(ofp, "\nSymbol Table:\n");
    fprintf(ofp, "index\t\tsymbol\n");
    
    // print out the symbol table to the output file and the symbol output file
    for (i = 0; i < numSymbols; i++ )
    {
        fprintf(ofp, "%d\t\t%s \n", i, symbol_table[i].name);
        fprintf(symbTabFP, "%s ", symbol_table[i].name);
    }
    // close the symbol file
    fclose( symbTabFP);
    
    // reopen the lexeme list file to read for printing to output file
    FILE *newIFP = fopen("lexemelist.txt", "rb");
    
    fprintf(ofp, "\nLexeme List:\n");
    while (fscanf(newIFP, "%d", &buffer) != EOF)
    {
        fprintf(ofp, "%d ", buffer);
    }
    
    // close all remaining files
    fclose(newIFP);
    fclose (ofp);
    
}// end function main



// creates a node if the first character is a letter, so it is either a variable
// or a reserved word.
node *BeginsWithLetter( char firstLetter, FILE *inputFile, node *tail,
                       FILE *outputFile )
{
    int wordLen = 11;
    int nextLetter;
    int letterPos = 1;
    
    // create space for a word of size 11
    char *word = calloc(wordLen + 1, sizeof(char) );
    
    // put the first letter to begin building the word
    strcpy( word, "" );
    word[0] = firstLetter;
    
    // get the next character
    nextLetter = fgetc(inputFile);
    
    // as long as the next character is a letter or number, keep adding it to
    // the word
    while( isalpha(nextLetter) || isdigit(nextLetter) )
    {
        // reallocate space if the word is too big
        if ( letterPos >= wordLen )
        {
            wordLen *= 2;
            word = realloc(word, wordLen + 1);
        }
        word[letterPos] = nextLetter;
        letterPos++;
        nextLetter = fgetc(inputFile);
    }// ends when the next character is not a letter or number
    
    // copy the word to the linkedlist node
    tail->word = malloc( strlen(word) + 1 );
    strcpy( tail->word,  word);
    tail->next = createNode();
    
    // free up the space occupied by the word
    free(word);
    
    // go back in the file so the character can be read again in the main program
    if (nextLetter != EOF )
        fseek(inputFile, -1, SEEK_CUR);
    
    // print the word to the file
    fprintf( outputFile, "%s", tail->word );
    
    // return pointer to the newly created node
    return tail->next;
    
}// end function BeginsWithLetter


// creates a node if the first character is a number
node *BeginsWithNumber( char firstDigit, FILE *inputFile, node *tail,
                       FILE *outputFile )
{
    int numDigits = 5;
    int digitPos = 1;
    int nextDigit;
    
    
    // allocate space for the number and begin copying the first digit
    char *word = calloc(numDigits + 1, sizeof(char) );
    strcpy( word, "" );
    word[0] = firstDigit;
    
    // get the next character from the file
    nextDigit = fgetc(inputFile);
    
    // as long as the next character is a number, keep adding it to the string
    while( isdigit(nextDigit) )
    {
        // reallocate space if necessary
        if ( digitPos >= numDigits )
        {
            numDigits *= 2;
            word = realloc(word, numDigits + 1);
        }
        // add new digits to the string
        word[digitPos] = nextDigit;
        digitPos++;
        nextDigit = fgetc(inputFile);
        
    }// end if nextDigit is not a digit
    
    // if the next character is a letter, print error message and exit program
    if ( isalpha(nextDigit) )
    {
        printf("Error 22. Variable does not start with a letter. \n");
        exit(1);
    }
    
    // copy string to the linked list
    tail->word = malloc( strlen(word) + 1 );
    strcpy( tail->word,  word);
    tail->next = createNode();
    
    // go back a character so the main file can read the non-digit character
    if ( nextDigit != EOF )
        fseek(inputFile, -1, SEEK_CUR);
    
    // free memory for the word
    free(word);
    
    // print number to output
    fprintf( outputFile, "%s", tail->word );
    
    return tail->next;
    
}// end function BeginsWithNumber


// creates a node if the first character is a symbol
node *BeginsWithSymbol( char firstSymbol, FILE *inputFile, node* tail,
                       FILE *outputFile )
{
    int maxNumSymbols = 2;
    
    // allocate space and begin building string for the symbol
    char *symbol = calloc(maxNumSymbols + 1, sizeof(char) );
    
    strcpy( symbol, "" );
    symbol[0] = firstSymbol;
    
    // check for the first character, and determine string accordingly
    switch ( firstSymbol )
    {
        case '/':   // could be / or /*
        {
            char nextChar;
        
            nextChar = fgetc(inputFile);
            
            // if the string is /*, begin comments.  Remove all characters until
            // the close comments */ string is found. Do not output any
            // characters between comments.
            if ( nextChar == '*' )
            {
                nextChar = fgetc(inputFile);
                // repeat until the / charcter is found, but only checks right
                // after the * is found
                do
                {
                    // repeat until the * is found
                    while (nextChar != '*' )
                    {
                        // exit program with error if no closing comments found
                        if ( nextChar == EOF )
                         {
                             printf("Error 21. No end to comments. */ required. \n");
                             exit(1);
                         }
                        nextChar = fgetc(inputFile);
                    }
                    nextChar = fgetc(inputFile);
                } while( nextChar != '/' );
                
                // dont add new node to the linked list
                return tail;
            }
            
            // if the 2nd character is not comments go back so main program
            // can read it
            else
            {
                if ( nextChar != EOF )
                    fseek(inputFile, -1, SEEK_CUR);
            }
            break;
        }// end case for /
        
        case '<' :      // can be <, <>, abd <=
        {
            char nextChar;
        
            int digitPos = 1;
        
            nextChar = fgetc(inputFile);
        
            // if the next character is = or >, add to string
            if ( nextChar == '=' || nextChar == '>' )
            {
                symbol[digitPos] = nextChar;
            }
            else    // is only <, go back so character can be re-read
            {
                if ( nextChar != EOF )
                    fseek(inputFile, -1, SEEK_CUR);
            
            }
            break;
        }// end case for <
            
        case '>' :       // can be > or >=
        {
            char nextChar;
            
            int digitPos = 1;
            
            nextChar = fgetc(inputFile);
            
            // add = to string to make >=
            if ( nextChar == '=')
            {
                symbol[digitPos] = nextChar;
            }
            else    // if not = then go back so character can be reread by
                    // main file
            {
                if ( nextChar != EOF )
                    fseek(inputFile, -1, SEEK_CUR);
                
            }
            break;
        }// end case for >
            
        case ':' :      // can only be :=
        {
            char nextChar;
            
            int digitPos = 1;
            
            nextChar = fgetc(inputFile);
            
            // if :=
            if ( nextChar == '=')
            {
                symbol[digitPos] = nextChar;
            }
            else    // if not :=, then is invalid symbol
            {
                printf("Error 20. Invalid symbol.  Exiting program.\n");
                exit(1);
            }
            break;
        }// end case for :
        
        // if any other approved symbol, string is ok, and nothing else needs
        // to be done
        case '+' :
        case '-' :
        case '*' :
        case '(' :
        case ')' :
        case '=' :
        case ',' :
        case '.' :
        case ';' :
            break;
        // if any other symbol, it is invalid.  Exit program.
        default :
            printf("Error 20. Invalid symbol.  Exiting program.\n");
            exit(1);
        
    }// end switch case statement
    
    // add the string for the symbol to the linked list
    tail->word = malloc( strlen(symbol) + 1 );
    strcpy( tail->word,  symbol);
    tail->next = createNode();
    
    // free space used for the symbol
    free(symbol);
    
    // print symbol to output file
    fprintf( outputFile, "%s", tail->word );
    
    return tail->next;
    
}// end function BeginsWithSymbol


// takes a string node and converts it to the appropriate token value
// creates a symbol in the symbol table if needed
void findLexeme(FILE *outFP, char *text, FILE *lexFP, symbol *table,
                int *numSymbs )
{
    int index;
    
    // if the first character is a letter
    if ( isalpha(text[0]) )
    {
        // check to make sure identifier name is not too long.  Error and exit
        // if it is too long
        if (strlen( text ) > MAX_IDENTIFIER_LENGTH )
        {
            printf("Error 19. Variable name is too long. \n" );
            exit(1);
        }
        
        // print the appropriate token value to the output file and the lexeme
        // list file for all reserved words
        if ( strcmp( text, "odd") == 0 ) {
            fprintf (outFP, "%d\n", 8 );
            fprintf (lexFP, "%d ", 8 );
        }
        else if ( strcmp( text, "begin") == 0 ) {
            fprintf (outFP, "%d\n", 21 );
            fprintf (lexFP, "%d ", 21 );
        }
        else if ( strcmp( text, "end") == 0 ) {
            fprintf (outFP, "%d\n", 22 );
            fprintf (lexFP, "%d ", 22 );
        }
        else if ( strcmp( text, "if") == 0 ) {
            fprintf (outFP, "%d\n", 23 );
            fprintf (lexFP, "%d ", 23 );
        }
        else if ( strcmp( text, "then") == 0 ) {
            fprintf (outFP, "%d\n", 24 );
            fprintf (lexFP, "%d ", 24 );
        }
        else if ( strcmp( text, "while") == 0 ) {
            fprintf (outFP, "%d\n", 25 );
            fprintf (lexFP, "%d ", 25 );
        }
        else if ( strcmp( text, "do") == 0 ) {
            fprintf (outFP, "%d\n", 26 );
            fprintf (lexFP, "%d ", 26 );
        }
        else if ( strcmp( text, "call") == 0 ) {
            fprintf (outFP, "%d\n", 27 );
            fprintf (lexFP, "%d ", 27 );
        }
        else if ( strcmp( text, "const") == 0 ) {
            fprintf (outFP, "%d\n", 28 );
            fprintf (lexFP, "%d ", 28 );
        }
        else if ( strcmp( text, "var") == 0 ) {
            fprintf (outFP, "%d\n", 29 );
            fprintf (lexFP, "%d ", 29 );
        }
        else if ( strcmp( text, "procedure") == 0 ) {
            fprintf (outFP, "%d\n", 30 );
            fprintf (lexFP, "%d ", 30 );
        }
        else if ( strcmp( text, "write") == 0 ) {
            fprintf (outFP, "%d\n", 31 );
            fprintf (lexFP, "%d ", 31 );
        }
        else if ( strcmp( text, "read") == 0 ) {
            fprintf (outFP, "%d\n", 32 );
            fprintf (lexFP, "%d ", 32 );
        }
        else if ( strcmp( text, "else") == 0 ) {
            fprintf (outFP, "%d\n", 33 );
            fprintf (lexFP, "%d ", 33 );
        }
        
        else    // if it is not a reserved word, it is an identifier
        {
            // print the appropriate token "2" and add it to the symbol table
            fprintf (outFP, "2\n" );
            index = putInSymbolTable( table, text, numSymbs );
            fprintf (lexFP, "2 %d ", index );
        }
    }// end if first character is letter
    
    // if the first character is a number
    else if ( isdigit(text[0]) )
    {
        // error and exit if the number has too many digits
        if (strlen( text ) > MAX_NUMBER_LENGTH )
        {
            printf("Error 17. This number is too large. \n" );
            exit(1);
        }
        // print the appropriate token "3" and add it to the symbol table
        fprintf(outFP, "3\n");
        index = putInSymbolTable( table, text, numSymbs );
        fprintf (lexFP, "3 %d ", index );
    }// end if first character is number
    
    // if the first character is punctuation.  Already tested for invalid
    // characters, so only need to find which one.
    else if ( ispunct(text[0]) )
    {
        // look at first character
        switch ( text[0] )
        {
            case '+' :
                fprintf(outFP, "%d\n", 4);
                fprintf (lexFP, "%d ", 4);
                break;
            case '-' :
                fprintf(outFP, "%d\n", 5);
                fprintf (lexFP, "%d ", 5);
                break;
            case '*' :
                fprintf(outFP, "%d\n", 6);
                fprintf (lexFP, "%d ", 6);
                break;
            case '/' :
                fprintf(outFP, "%d\n", 7);
                fprintf (lexFP, "%d ", 7);
                break;
            case '(' :
                fprintf(outFP, "%d\n", 15);
                fprintf (lexFP, "%d ", 15);
                break;
            case ')' :
                fprintf(outFP, "%d\n", 16);
                fprintf (lexFP, "%d ", 16);
                break;
            case '=' :
                fprintf(outFP, "%d\n", 9);
                fprintf (lexFP, "%d ", 9);
                break;
            case ',' :
                fprintf(outFP, "%d\n", 17);
                fprintf (lexFP, "%d ", 17);
                break;
            case '.' :
                fprintf(outFP, "%d\n", 19);
                fprintf (lexFP, "%d ", 19);
                break;
            case '<' :     // <, <>, <=
                // check for <> and <=
                if ( strlen(text) > 1 )
                {
                    if ( strcmp(text, "<>") == 0 )
                    {
                        fprintf(outFP, "%d\n", 10);
                        fprintf (lexFP, "%d ", 10);
                    }
                    else if ( strcmp(text, "<=") == 0 )
                    {
                        fprintf(outFP, "%d\n", 12);
                        fprintf (lexFP, "%d ", 12);
                    }
                }
                else
                {
                    fprintf(outFP, "%d\n", 11);
                    fprintf (lexFP, "%d ", 11);
                }
                break;
            case '>' :      // > and >=
                if ( strlen(text) > 1 )
                {
                    if ( strcmp(text, ">=") == 0 )
                    {
                        fprintf(outFP, "%d\n", 14);
                        fprintf (lexFP, "%d ", 14);
                    }
                }
                else
                {
                    fprintf(outFP, "%d\n", 13);
                    fprintf (lexFP, "%d ", 13);
                }
                break;
            case ';' :
                fprintf(outFP, "%d\n", 18);
                fprintf (lexFP, "%d ", 18);
                break;
            case ':' :
                fprintf(outFP, "%d\n", 20);
                fprintf (lexFP, "%d ", 20);
                break;
                
        }// end switch statement
        
    }// end if first character is symbol
}// end function FindLexeme


// puts the text into the symbol table.  Checks if it is already there.
int putInSymbolTable( symbol *symbol_table, char *text, int *numSymbols )
{
    int i;
    
    // go through each symbol in symbol table to see if string is already there
    for ( i = 0; i < *numSymbols; i++ )
    {
        // if string in symbol table, return the corresponding index
        if ( strcmp( symbol_table[i].name, text ) == 0 )
        {
            return i;
        }
    }
    
    // if string is not in symbol table, add it to the last index
    strcpy( symbol_table[*numSymbols].name, text );
    
    // increment the last index
    *numSymbols = *numSymbols + 1;
    
    // return the index for the newly added string
    return ( *numSymbols - 1 );
    
}// end function putInSymbolTable


// creates a new node.  Will store strings for each piece of code representing
// identifiers, numbers, and symbols, but for now just creating the node.
node *createNode()
{
    node *ptr = malloc(sizeof(node));

    ptr->next = NULL;
    
    return ptr;
}// end function createNode