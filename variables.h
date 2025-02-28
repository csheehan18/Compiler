#ifndef VARIABLES_H

#define VARIABLES_H
typedef enum
{
    // Types of tokens (Im sure it's gonna be much longer in the end)
    L_UNDEFINED,
    L_COMMENT,
    L_EQUAL,
    L_PLUS,
    L_MINUS,
    L_FLOAT,
    L_INT,
    L_STRING,
    L_TYPE,
    L_DIVIDE,
    L_MULTIPLY,
    L_ENDSTATEMENT,
    L_OPENPARENTHESE,
    L_CLOSEDPARENTHESE,
    L_OPENBRACE,
    L_CLOSEDBRACE,
    L_OPENBRACKET,
    L_CLOSEDBRACKET,
    L_AND,
    L_NOT,
    L_COMMA,
    L_OR,
    L_SYMBOL
} LexTokens;

typedef struct Token_s
{
    LexTokens lt;
    union Values
    {
        // Optional Values for Token
        int integerNum;
        float floatNum;
        // Allocate memory for the variable name when calling it
        char *name;
    } value;

    // For Linked-List
    struct Token_s *next;
    struct Token_s *prev;
} Token_t;

typedef struct stringBuff_s
{
    char c;

    // For linked list
    struct stringBuff_s *next; // Changed 'stringBuff' to 'struct stringBuff_s'
} stringBuff;

// For me to easily print values
const char *LexTokenNames[] = {
    "L_UNDEFINED",
    "L_COMMENT",
    "L_EQUAL",
    "L_PLUS",
    "L_MINUS",
    "L_FLOAT",
    "L_INT",
    "L_STRING",
    "L_TYPE",
    "L_DIVIDE",
    "L_MULTIPLY",
    "L_ENDSTATEMENT",
    "L_OPENPARENTHESE",
    "L_CLOSEDPARENTHESE",
    "L_OPENBRACE",
    "L_CLOSEDBRACE",
    "L_OPENBRACKET",
    "L_CLOSEDBRACKET",
    "L_AND",
    "L_NOT",
    "L_COMMA",
    "L_OR",
    "L_SYMBOL"};

    #endif