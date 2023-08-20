/*-----------------------------------------------------------------------------
 * Project 1 
 * Created by Angel He (angelh1@student.unimelb.edu.au) 24/03/2023
 * doubly_linked_list.c :
              = the implementation of the module `doubly_linked_list` 
                of the project 
 ----------------------------------------------------------------------------*/

#include "doubly_linked_list.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

/* Creates and returns an empty doubly-linked list.
 */
doubly_linked_list_t *make_empty_dll() {
    doubly_linked_list_t *list = malloc(sizeof(*list));
    assert(list);
    list->head = list->foot = NULL;
    return list;
}

/* Returns TRUE (1) if the doubly-linked list is empty, FALSE (0) otherwise. 
 */
int is_empty_dll(doubly_linked_list_t *list) {
    return list->head == NULL;
}

/* Inserts the node to the foot of the doubly-linked list;
 * Returns the pointer to the modified list.
 */
doubly_linked_list_t *insert_at_dll_foot(doubly_linked_list_t *list, 
        double_node_t *new) {
    assert(list && new);
    new->next = NULL;
    new->prev = NULL;

    if (is_empty_dll(list)) {
        // first insertion into the list
        list->head = list->foot = new;
    } else {
        list->foot->next = new;
        new->prev = list->foot;
        list->foot = new;
    }

    return list;
}

/* Inserts the node n2 behind n1.
 * Returns the pointer to the modified doubly-linked list.
 */
doubly_linked_list_t *insert_behind(double_node_t *n1, double_node_t *n2, 
        doubly_linked_list_t *list) {
    assert(n1 && n2 && list);

    n2->next = n1->next;
    if (n1->next)
        n1->next->prev = n2;
    else // n1 was the foot
        list->foot = n2;
    n1->next = n2;
    n2->prev = n1;

    return list;
}

/* Deletes the head of the doubly-linked list, and returns the deleted head.
 * Returns NULL if the list is already empty.
 */
double_node_t *delete_dll_head(doubly_linked_list_t *list) {
    assert(list);
    if (is_empty_dll(list))
        return NULL;

    double_node_t *old_head = list->head;
    list->head = list->head->next;
    if (list->head == NULL) // empty list now
        list->foot = NULL;
    
    list->head->prev = NULL;
    old_head->next = NULL;

    return old_head;
}

/* Creates and returns a double-node holding the data. 
 */
double_node_t *create_double_node(void *data) {
    assert(data);
    double_node_t *new = malloc(sizeof(*new));
    assert(new);
    new->data = data;
    new->next = new->prev = NULL;
    return new;
}

/* Deletes the node from the doubly-linked list, 
   and frees the deleted node.
 */
void delete_double_node(double_node_t *node, doubly_linked_list_t *dll) {
    assert(dll && node);

    if (node->prev)
        node->prev->next = node->next;
    else // deleting list head
        dll->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else // deleting foot
        dll->foot = node->prev;
    
    /* Free the deleted node */
    free(node->data);
    node->data = NULL;
    node->prev = node->next = NULL;
    free(node);
    node = NULL;
}

/* Frees all memory allocated to the doubly-linked list. 
 */
void free_dll(doubly_linked_list_t *dll) {
    assert(dll);
    double_node_t *curr, *prev;
    curr = dll->head;
    
    while (curr) {
        prev = curr;
        curr = curr->next;
        free(prev);
        prev = NULL;
    }
    free(dll);
    dll = NULL;
}