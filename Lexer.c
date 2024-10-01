#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Forward declaration of Token struct
typedef struct Token_s Token_t;

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

typedef struct stringBuff_s
{
    char c;

    // For linked list
    struct stringBuff_s *next; // Changed 'stringBuff' to 'struct stringBuff_s'
} stringBuff;

stringBuff *bufferHead = NULL;

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
    };

    // For Linked-List
    Token_t *next;
    Token_t *prev;
} Token_t;

Token_t *tokenHead = NULL;

// Create or add to our list
void AddTokens(LexTokens l, int *i, float *f, char *name)
{
    Token_t *link = (Token_t *)malloc(sizeof(Token_t));
    link->lt = l;
    link->next = NULL;
    if (i != NULL)
    {
        link->integerNum = *i;
    }
    else if (f != NULL)
    {
        link->floatNum = *f;
    }
    else if (name != NULL)
    {
        link->name = name;
    }

    // Start of list
    if (tokenHead == NULL)
    {
        tokenHead = link;
        tokenHead->prev = NULL;
        return;
    }

    Token_t *current = tokenHead;
    while (current->next != NULL)
    {
        current = current->next;
    }

    current->next = link;
    current->next->prev = current;
}

// Create a dynamic array that is flushed once we reach a special character and sends it through before being reset
// Create or add to our list
void AddToBuffer(char c)
{
    stringBuff *link = (stringBuff *)malloc(sizeof(stringBuff));
    link->c = c;
    link->next = NULL;

    // New buffer, create head
    if (bufferHead == NULL)
    {
        bufferHead = link;
        return;
    }

    // Find last element or tail
    stringBuff *current = bufferHead;
    while (current->next != NULL)
    {
        current = current->next;
    }

    // Append the new node to the end of the list
    current->next = link;
}

void ClearBuffer()
{
    stringBuff *current = bufferHead;
    while (current != NULL)
    {
        stringBuff *temp = current;
        current = current->next;
        free(temp);
    }
    bufferHead = NULL;
}

int GetBufferCount()
{
    int count = 0;
    stringBuff *current = bufferHead;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    return count;
}

bool RemoveFromBuffer(int index) {
    if (tokenHead == NULL) {
        printf("\nError: Wtf we doing... Line: 183\n");
        return false;
    }
    //remove head
    if (index == 0) {
        stringBuff *temp = bufferHead->next;
        bufferHead->next = NULL;
        bufferHead = temp;
        return true; 
    }

    int count = 0;
    stringBuff *curr = bufferHead;
    while (curr != NULL) {
        if (count == index - 1) {
            if (index == GetBufferCount()) {
                //There is no next
                stringBuff *delete = curr->next;
                curr->next = NULL;
                free(delete);
                return true; 
            }
            //The curr.next is what we want to delete
            stringBuff *temp = curr->next->next;
            stringBuff *delete = curr->next;
            curr->next->next = NULL;
            curr->next = NULL;
            curr->next = temp;
            free(delete);
            return true; 
        }
        count++;
        curr = curr->next;
    }

    return false;
}

bool IsNumber(char *num)
{
    for (int i = 0; i < GetBufferCount(); i++)
    {
        if (!isdigit(num[i]) && num[i] != '.')
        {
            return false;
        }
    }
    return true;
}

void Lex(char *s)
{
    // printf("Lex: %s\n", s);
    //  Types
    if (strcmp(s, "int") == 0 || strcmp(s, "bool") == 0 || strcmp(s, "string") == 0)
    {
        AddTokens(L_TYPE, NULL, NULL, s);
    }
    else if (strcmp(s, "=") == 0)
    {
        AddTokens(L_EQUAL, NULL, NULL, NULL);

        // For numbers
    }
    else if (IsNumber(s))
    {
        if (strchr(s, '.') != NULL)
        {
            float value;
            sscanf(s, "%f", &value);
            AddTokens(L_FLOAT, NULL, &value, NULL);
        }
        else
        {
            int value;
            sscanf(s, "%i", &value);
            AddTokens(L_INT, &value, NULL, NULL);
        }
    }
    else if (strchr(s, '/') && strlen(s) == 1)
    {
        AddTokens(L_DIVIDE, NULL, NULL, NULL);
    }
    else if (strchr(s, ';') && strlen(s) == 1)
    {
        AddTokens(L_ENDSTATEMENT, NULL, NULL, NULL);
    }
    else if (strchr(s, ',') && strlen(s) == 1)
    {
        AddTokens(L_COMMA, NULL, NULL, NULL);
    }
    else if (strchr(s, '!') && strlen(s) == 1)
    {
        AddTokens(L_NOT, NULL, NULL, NULL);
    }
    else if (strchr(s, '&') && strlen(s) == 1)
    {
        AddTokens(L_AND, NULL, NULL, NULL);
    }
    else if (strchr(s, '+') && strlen(s) == 1)
    {
        AddTokens(L_PLUS, NULL, NULL, NULL);
    }
    else if (strchr(s, '{') && strlen(s) == 1)
    {
        AddTokens(L_OPENBRACE, NULL, NULL, NULL);
    }
    else if (strchr(s, '}') && strlen(s) == 1)
    {
        AddTokens(L_CLOSEDBRACE, NULL, NULL, NULL);
    }
    else if (strchr(s, '[') && strlen(s) == 1)
    {
        AddTokens(L_OPENBRACKET, NULL, NULL, NULL);
    }
    else if (strchr(s, ']') && strlen(s) == 1)
    {
        AddTokens(L_CLOSEDBRACKET, NULL, NULL, NULL);
    }
    else if (strchr(s, '(') && strlen(s) == 1)
    {
        AddTokens(L_OPENPARENTHESE, NULL, NULL, NULL);
    }
    else if (strchr(s, ')') && strlen(s) == 1)
    {
        AddTokens(L_CLOSEDPARENTHESE, NULL, NULL, NULL);
        // String
    }
    else if (strchr(s, '\'') || strchr(s, '\"'))
    {
        //Need to remove the quotation mark at the end
        s[strlen(s) - 1] = '\0';
        AddTokens(L_STRING, NULL, NULL, s);
    }
    else
    {
        AddTokens(L_SYMBOL, NULL, NULL, s);
    }
}

void Assign()
{
    char *rebuildString = (char *)malloc(GetBufferCount() + 1 * sizeof(char));
    stringBuff *current = bufferHead;
    int count = 0;
    while (current != NULL)
    {
        rebuildString[count] = current->c;
        current = current->next;
        count++;
    }
    // For string end
    rebuildString[count] = '\0';
    // printf("%s\n", rebuildString);
    Lex(rebuildString);
}

void StartReading(FILE *file)
{
    char letter;
    char lastLetter = NULL;
    bool inString = false;

    do
    {
        // NEED TO CHECK FOR COMMENTS AND SET fgetc to next line
        //  Gets current letter from file
        letter = fgetc(file);
        if (letter == EOF)
        {
            break;
        }
        // Using Ascii table check for values (A-Z, 1-9 and . and "" because we use it for floats and decimals)
        // A-Z
        //Go into // State - craig idea
        // If two symbols come after each other fix here
        if ((lastLetter != '\\') && (letter == '\'' || letter == '\"'))
        {
            inString = !inString;
            if (GetBufferCount() > 1) {
                //Will remove later but used for checking if string or not
                AddToBuffer(letter);
            }
        } else if (lastLetter == '\\') {
            switch (letter)
            {
            case 'n':
                RemoveFromBuffer(GetBufferCount() - 1);
                AddToBuffer('\n');
                break;
            case 't':
                RemoveFromBuffer(GetBufferCount() - 1);
                AddToBuffer('\t');
                break;
            case '\\':
                RemoveFromBuffer(GetBufferCount() - 1);
                AddToBuffer('\\');
                break;
            default:
                //Idk what there doing so will allow it for now? - Conner 5/6/2024
                AddToBuffer(letter);
                break;
            }
        }
        else if (letter >= 'A' && letter <= 'Z')
        {
            AddToBuffer(letter);
        }
        else if (letter >= 'a' && letter <= 'z')
        {
            AddToBuffer(letter);
        }
        else if ((letter >= '0' && letter <= '9') || letter == '.')
        {
            AddToBuffer(letter);
        }
        else
        {
            if (GetBufferCount() >= 1 && inString == false)
            {
                Assign();
                ClearBuffer();
            }
            if (!(letter == ' ' || letter == '\n') || inString == true)
            {
                AddToBuffer(letter);
                if (inString == false)
                {
                    Assign();
                    ClearBuffer();
                }
            }
        }
        lastLetter = letter;

        // Checking if character is not EOF.
    } while (letter != EOF);
    fclose(file);
}

void PrintTokens()
{
    Token_t *current = tokenHead;
    puts("");
    while (current != NULL)
    {
        if (current->lt == L_SYMBOL)
        {
            printf("Type: %s Value: %s\n", LexTokenNames[current->lt], current->name);
        }
        else if (current->lt == L_FLOAT)
        {
            printf("Type: %s Value: %f\n", LexTokenNames[current->lt], current->floatNum);
        }
        else if (current->lt == L_INT)
        {
            printf("Type: %s Value: %i\n", LexTokenNames[current->lt], current->integerNum);
        }
        else if (current->lt == L_STRING)
        {
            printf("Type: %s Value: %s\n", LexTokenNames[current->lt], current->name);
        }
        else
        {
            printf("Type: %s\n", LexTokenNames[current->lt]);
        }
        current = current->next;
    }
}

typedef struct Number_s Number_t;

typedef struct Number_s {

    LexTokens lt;
    union values
    {
        int i;
        float f;
        char op;
    };
    Number_t *next;

}Number_t;

Number_t *Expressionhead = NULL;

void AddToExpression(int *i, float *f, char *op) {
    Number_t *link = (Number_t *)malloc(sizeof(Number_t));
    if (i != NULL) {
        link->i = *i;
        link->lt = L_INT;
    } else if (f != NULL) {
        link->f = *f;
        link->lt = L_FLOAT;
    } else {
        link->op = *op;
        link->lt = L_SYMBOL;
    }
    link->next = NULL;

    // New buffer, create head
    if (Expressionhead == NULL)
    {
        Expressionhead = link;
        return;
    }

    // Find last element or tail
    Number_t *current = Expressionhead;
    while (current->next != NULL)
    {
        current = current->next;
    }

    // Append the new node to the end of the list
    current->next = link;
}

void EvaluateExpression() {
    int iNum = 0;
    float fNum = 0.00f;
    char c = NULL;
    int count = 0;
    Number_t *current = Expressionhead;

    while (current != NULL) {
        if (current->lt == L_INT) {
            if (count == 0) {
                iNum = current->i;
            } else {
                iNum = iNum * 10 + current->i;
            }
        } else if (current->lt == L_FLOAT) {
            if (count == 0) {
                fNum = current->i;
            } else {
                fNum = iNum * 10 + current->i;
            }
        } else {
            c = current->op;
        }
        current = current->next;
        printf("%i", iNum);
        printf("%f", fNum);
        printf("%c", c);
    }
}

void PrintStatement(char *output)
{
    printf(output);
}

void PrintEval(Token_t *t) {
    while (t->lt != L_CLOSEDPARENTHESE) {
        if (t->lt == L_INT) {
            printf("Integer found!");
        } else if (t->lt == L_FLOAT) {
            printf("Float found");
        } else if (t->lt == L_STRING) {
            printf("%s", t->name);
        }
        t = t->next;
    }
}

void Determine()
{
    Token_t *t = tokenHead;
    while (t != NULL)
    {
        if (strcmp(t->name, "print") == 0 && t->next->lt == L_OPENPARENTHESE)
        {
            printf("\nPrint Statment: ");
            PrintEval(t->next->next);
        }
        t = t->next;
    }
}

int main()
{
    const char input_file[] = "C:/Users/conai/Documents/Coding Projects/test.txt";
    // Open file and initialize arrays
    FILE *file = fopen(input_file, "r");
    if (!file)
    {
        perror(input_file);
        return EXIT_FAILURE;
    }

    StartReading(file);

    //PrintTokens();
    puts("Success!");
    Determine();
    return 0;
}
