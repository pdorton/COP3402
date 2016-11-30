//This header files contains multiple variable declarations that are shared across more than one .c file between lex, parser, and vm. If a variable is only needed in one .c file, it is locally or globally defined there. If it needs to be shared across more than one .c file, it is declared here instead.

/**defines**/

//used in parser.c
#define MAX_SYMBOL_TABLE_SIZE 1000
//used in parser.c and vm.c
#define CODE_SIZE 1000
#define MAX_LEXI_LEVELS 3
//used in vm.c
#define MAX_STACK_HEIGHT 2000

/**variables**/

extern int lexoutput;
extern int codegenoutput;
extern int vmoutput;

extern int errorCount;

//Given enum for internal representation
typedef enum {
    nulsym = 1, identsym, numbersym, plussym, minussym,
    multsym,  slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
    gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
    periodsym, becomessym, beginsym, endsym, ifsym, thensym,
    whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
    readsym , elsesym
} token_type;

//struct used to contain all tokens details
typedef struct {
    token_type tokenID;
    int numberValue;
    char name[12];
}tokenStruct;

extern tokenStruct lexList[5000];
extern int lexListIndex;

//for connecting parser to vm, add "extern instruction code[CODE_SIZE];
//as well as instruction struct below

 typedef struct {
     int op; //Opcode
     int l;  //L
     int m;  //M
 } instruction;

extern instruction code[CODE_SIZE];


/**functions**/

//Called by main
void compile(FILE* in, FILE* out);
void lex(FILE* in);
void parse(FILE* out);
void vm(void);