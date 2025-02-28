#include <stdio.h>
#include "variables.h"


void parseTokens(Token_t *head) {
    printf("Parsing...\n");
    // Goal is to parse each statement on its own
    Token_t *temp = head;
    while (temp != NULL) {
        temp = temp->next;
    }
}