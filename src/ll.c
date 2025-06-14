#include "ll.h"

void free_ll(lnode* head)
{
    lnode* curr_node = head;
    lnode* temp;

    // printf("Freeing linked list...\n");
    while (curr_node != NULL)
    {
        temp = curr_node;
        curr_node = curr_node->next;
        free(temp);
    }
    // printf("Linked list freed\n");
}
