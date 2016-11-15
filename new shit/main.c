#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "data.h"


int codegenoutput = 0;
int vmoutput = 0;
int lexoutput = 0;
int c;
FILE* ifp;

//Given enum for internal representation
const char *lexSym[] = { "", "nulsym", "identsym", "numbersym", "plussym", "minussym",
    "multsym",  "slashsym", "oddsym", "eqsym", "neqsym", "lessym", "leqsym",
    "gtrsym", "geqsym", "lparentsym", "rparentsym", "commasym", "semicolonsym",
    "periodsym", "becomessym", "beginsym", "endsym", "ifsym", "thensym",
    "whilesym", "dosym", "callsym", "constsym", "varsym", "procsym", "writesym",
    "readsym" , "elsesym"};

//functions
void usage(void);

//main, which when called, will call other .c files
int main(int argc, char *argv[]) {
    
    FILE* in = fopen(argv[1], "r");
    FILE* out = fopen(argv[2], "w");

    compile(in,out);
}

//runs all .c files
void compile(FILE* in, FILE* out) {
    
    //run lex.c with the test case. Lex outputs a file with the lexeme list that parser reads from
    lex(in);
    
    //run parser.c with the input file, that is the output file from lex. This then writes to another output file that the vm reads
    parse(out);
    
    //run vm.c with the input file, that is the output file from the parser. This then writes to a 3rd and final output file, which will have all the vm prints.
     //vm(); commented out since we are not worried about this for the time being 
    
    
    
    //this reads from "lexoutput.txt" and...
    if (lexoutput == 1) 
    {
        //...first prints out the token (internal represenation) list, and then...
        printf("\n");
        ifp = fopen("lexoutput.txt", "r");
        while((c=fgetc(ifp)) !=EOF)
        {
            printf("%c", c);
        }
        printf("\n\n");
        fclose(ifp);
        
        //...prints out the symbolic representation list!
        int i;
        ifp = fopen("lexoutput.txt", "r");
        //prints out variable names in Lexeme List
        for(i=0;i<lexListIndex;i++)
        {
            printf("%s ", lexSym[lexList[i].tokenID]);
            if(lexList[i].tokenID==2){
                printf("%s ", lexList[i].name);
            }
            else if(lexList[i].tokenID==3)
            {
                printf("%d ",lexList[i].numberValue);
            }
        }
        printf("\n\n");
        fclose(ifp);
    }
    
    //this will only be printed if we had no errors
    if (errorCount==0) 
    {
        printf("No errors, program is syntactically correct\n\n");
    }
    
    //do parse print stuff
    //this reads from "vmout2.txt" and...
    if (codegenoutput == 1) 
    {
        
        //...prints out the generated code
        ifp = fopen("vmout2.txt", "r");
        while((c=fgetc(ifp)) !=EOF)
        {
            //putchar(c);
            printf("%c", c);
        }
        printf("\n");
        fclose(ifp);
    }
    
    //do vm print stuff
    //this reads from "vmout3.txt" and...
    if (vmoutput == 1) 
    {
        
        //...prints out the program after it runs on the virtual machine
        ifp = fopen("vmout3.txt", "r");
        while((c=fgetc(ifp)) !=EOF)
        {
            printf("%c", c);
        }
        printf("\n");
        fclose(ifp);
    }
}

//prints out how to use the program
void usage() {
    
    printf("\n-l prints out the lexeme list from lex.c\n");
    printf("-a prints out instructions from vm.c, which are passed to vm.c through the parser.c\n");
    printf("-v prints out stack frames from vm.c\n\n");
    return;
}