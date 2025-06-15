#include "ll.h"

void free_ll(pnode* head)
{
    pnode* curr_node = head;
    pnode* temp;

    // printf("Freeing linked list...\n");
    while (curr_node != NULL)
    {
        temp = curr_node;
        curr_node = curr_node->next;
        free(temp);
    }
    // printf("Linked list freed\n");
}
