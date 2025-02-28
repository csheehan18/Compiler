#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "variables.h"

stringBuff *bufferHead = NULL;

Token_t *tokenHead = NULL;

// Create or add to our list
void AddTokens(LexTokens l, int *i, float *f, char *name)
{
    Token_t *link = (Token_t *)malloc(sizeof(Token_t));
    link->lt = l;
    link->next = NULL;
    if (i != NULL)
    {
        link->value.integerNum = *i;
    }
    else if (f != NULL)
    {
        link->value.floatNum = *f;
    }
    else if (name != NULL)
    {
        link->value.name = name;
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
    if (strcmp(s, "int") == 0 || strcmp(s, "bool") == 0 || strcmp(s, "string") == 0 || strcmp(s, "float") == 0 || strcmp(s, "var") == 0)
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
        // Super odd edge case idk something to do with new lines
        if (strchr(s, '\0') && strlen(s) == 1) {
            //printf("I think this new line being weird\n");
        } else
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
    char lastLetter;
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
    while (current != NULL)
    {
        if (current->lt == L_SYMBOL)
        {
            printf("Type: %s Value: %s\n", LexTokenNames[current->lt], current->value.name);
        }
        else if (current->lt == L_FLOAT)
        {
            printf("Type: %s Value: %f\n", LexTokenNames[current->lt], current->value.floatNum);
        }
        else if (current->lt == L_INT)
        {
            printf("Type: %s Value: %i\n", LexTokenNames[current->lt], current->value.integerNum);
        }
        else
        {
            if (current->value.name != NULL) {
                printf("Type: %s Value: %s\n", LexTokenNames[current->lt], current->value.name);
            }
            else 
                printf("Type: %s\n", LexTokenNames[current->lt]);
        }
        current = current->next;
    }
}

void FreeTokens() {
    Token_t *current = tokenHead;
    while (current != NULL) {
        Token_t *temp = current;
        current = current->next;
        free(temp);
    }
}

int main(int argc, char **args)
{
    if (argc < 2) {
        printf("Error: No file given\n");
        return EXIT_FAILURE;
    }
    const char *input_file = args[1];
    // Open file and initialize arrays
    FILE *file = fopen(input_file, "r");
    if (!file)
    {
        perror(input_file);
        return EXIT_FAILURE;
    }

    StartReading(file);

    //PrintTokens();
    FreeTokens();
    puts("Success!");
    return 0;
}
