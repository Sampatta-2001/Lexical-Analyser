#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

// file pointer for source code
static FILE *sourceFile;

// list of reserved keywords
static const char *keywords[MAX_KEYWORDS] = {
    "int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", "struct"};

const char operators[] = "+-*/=<>!";    // operators
const char specialChars[] = "();{}[],"; // special characters and deliminators

// open source file
void initializeLexer(const char *filename)
{
    sourceFile = fopen(filename, "r");
    if (!sourceFile)
    {
        printf("Error: cannot open %s\n", filename);
    }
}

// checking string is a keyword
int isKeyword(const char *str)
{
    for (int i = 0; i < MAX_KEYWORDS; i++)
    {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

// checking string is an operator
int isOperator(const char *str)
{
    return (strlen(str) == 1 && strchr(operators, str[0]) != NULL);
}

// checking string special character
int isSpecialCharacter(char ch)
{
    return (strchr(specialChars, ch) != NULL);
}

// checking constant (digit only for now) numbers
int isConstant(const char *str)
{
    int i = 0;
    if (!str || !*str)
        return 0;
    while (str[i])
    {
        if (!isdigit((unsigned char)str[i]))
            return 0;
        i++;
    }
    return 1;
}

// checking identifier (variable or function name)
int isIdentifier(const char *str)
{
    if (!isalpha((unsigned char)str[0]) && str[0] != '_')
        return 0;
    for (int i = 1; str[i]; i++)
    {
        if (!isalnum((unsigned char)str[i]) && str[i] != '_')
            return 0;
    }
    return 1;
}

// categorize token after extracting
void categorizeToken(Token *token)
{
    if (isKeyword(token->lexeme))
    {
        token->type = KEYWORD;
    }
    else if (isConstant(token->lexeme))
    {
        token->type = CONSTANT;
    }
    else if (isOperator(token->lexeme))
    {
        token->type = OPERATOR;
    }
    else if (isIdentifier(token->lexeme))
    {
        token->type = IDENTIFIER;
    }
    else if (strcmp(token->lexeme, "{") == 0)
    {
        token->type = O_BRACE;
    }
    else if (strcmp(token->lexeme, "}") == 0)
    {
        token->type = C_BRACE;
    }
    else if (strcmp(token->lexeme, "(") == 0)
    {
        token->type = O_PAREN;
    }
    else if (strcmp(token->lexeme, ")") == 0)
    {
        token->type = C_PAREN;
    }
    else if (strcmp(token->lexeme, "[") == 0)
    {
        token->type = O_BRACKET;
    }
    else if (strcmp(token->lexeme, "]") == 0)
    {
        token->type = C_BRACKET;
    }
    else
    {
        token->type = DELIMETER;
    }
}

// extract next token
Token getNextToken()
{
    Token token;
    int c = fgetc(sourceFile);

    while (isspace(c))
        c = fgetc(sourceFile);

    if (c == '#')
    {
        // preprocessor directive
        int i = 0;
        token.lexeme[i++] = c;
        while ((c = fgetc(sourceFile)) != '\n' && c != EOF)
        {
            token.lexeme[i++] = c;
        }
        token.lexeme[i] = '\0';
        token.type = PREPROCESSOR;
    }
    else if (isalpha(c) || c == '_')
    {
        // identifier/keyword
        int i = 0;
        while (isalnum(c) || c == '_')
        {
            token.lexeme[i++] = c;
            c = fgetc(sourceFile);
        }
        token.lexeme[i] = '\0';
        ungetc(c, sourceFile);
        categorizeToken(&token);
    }
    else if (isdigit(c))
    {
        int i = 0;
        while (isdigit(c))
        {
            token.lexeme[i++] = c;
            c = fgetc(sourceFile);
        }
        token.lexeme[i] = '\0';
        ungetc(c, sourceFile);
        categorizeToken(&token);
    }
    else if (isOperator((char[]){c, '\0'}))
    {
        token.lexeme[0] = c;
        token.lexeme[1] = '\0';
        token.type = OPERATOR;
    }
    else if (isSpecialCharacter(c))
    {
        token.lexeme[0] = c;
        token.lexeme[1] = '\0';
        categorizeToken(&token);
    }
    else if (c == EOF)
    {
        strcpy(token.lexeme, "EOF");
        token.type = END_OF_FILE;
    }
    else
    {
        token.lexeme[0] = c;
        token.lexeme[1] = '\0';
        token.type = DELIMETER;
    }
    return token;
}

// return readable type
const char *getTokenTypeName(TokenType type)
{
    switch (type)
    {
    case PREPROCESSOR:
        return "preprocessor directive";
    case KEYWORD:
        return "reserved keyword";
    case IDENTIFIER:
        return "identifier";
    case CONSTANT:
        return "numeric constant";
    case OPERATOR:
        return "operator";
    case O_BRACE:
        return "o_brace";
    case C_BRACE:
        return "c_brace";
    case O_PAREN:
        return "o_paren";
    case C_PAREN:
        return "c_paren";
    case O_BRACKET:
        return "o_bracket";
    case C_BRACKET:
        return "c_bracket";
    case END_OF_FILE:
        return "end_of_file";
    default:
        return "delimeter";
    }
}

/*
    in this code it opens the file reads characters groups them into tokens(keywords,identifiers,constants,operators,special symbols)
    Then it categorizes each token and returns a readable token type name.
*/