#include <stdio.h>
#include "tokens.h"

TokenType tok;

int main(){




	return 0;
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
