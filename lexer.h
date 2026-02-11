#ifndef LEXER_H
#define LEXER_H // Prevents multiple inclusion of this header

#define MAX_KEYWORDS 20    // maximum no. of keywords
#define MAX_TOKEN_SIZE 100 // maximum size of token string

typedef enum
{
    KEYWORD,      // reserved word(int,if.....)
    IDENTIFIER,   // variable and function name
    CONSTANT,     // numbers
    OPERATOR,     //=,+,-,/,* etc
    O_BRACE,      // {
    C_BRACE,      // }
    O_PAREN,      // (
    C_PAREN,      // )
    O_BRACKET,    // [
    C_BRACKET,    // ]
    PREPROCESSOR, // #include, #define etc
    DELIMETER,    //; , other symbols
    END_OF_FILE
} TokenType;

typedef struct
{
    char lexeme[MAX_TOKEN_SIZE];
    TokenType type;
} Token;

// function prototypes
void initializeLexer(const char *filename);
Token getNextToken();
void categorizeToken(Token *token);

int isKeyword(const char *str);
int isOperator(const char *str);
int isSpecialCharacter(char ch);
int isConstant(const char *str);
int isIdentifier(const char *str);
const char *getTokenTypeName(TokenType type);

#endif

/*
    This header defines token types, token structure, and lexer function prototypes used to break source code into categorized tokens.
*/