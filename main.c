/*
Name : Sampatta Ghulanawar
Description: Lexical Anyalyzer
the first phase of a compiler that reads source code and converts it into a stream of tokens. 
It reads the program character by character, groups them into meaningful lexemes, and outputs tokens like keywords, identifiers, and operators. 
This process also involves removing comments and white spaces and handling errors
*/




#include <stdio.h>
#include <string.h>
#include "lexer.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    initializeLexer(argv[1]);
    Token token;
    do
    {
        /* code */
        token = getNextToken();
        if (strcmp(token.lexeme, "EOF") != 0)
        {
            printf("%s -> %s\n", token.lexeme, getTokenTypeName(token.type));
        }
    } while (strcmp(token.lexeme, "EOF") != 0);

    return 0;
}
