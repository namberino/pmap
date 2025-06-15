#ifndef LL_H
#define LL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct pNode
{
    int num;
    struct pNode* next;
} pnode;

typedef struct hNode
{
    char* host_str;
    struct hNode* next;
} hnode;


void free_ll(pnode* head);

#endif
