#ifndef LL_H
#define LL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Node
{
    int num;
    struct Node* next;
} lnode;

void free_ll(lnode* head);

#endif
