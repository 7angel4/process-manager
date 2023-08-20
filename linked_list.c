/*-----------------------------------------------------------------------------
 * Project 1 
 * Created by Angel He (angelh1@student.unimelb.edu.au) 24/03/2023
 * linked_list.c :
              = the implementation of the module `linked_list` of the project 
 ----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "linked_list.h"

/* Creates and returns an empty list. 
 */
list_t *make_empty_list() {
    list_t *list = malloc(sizeof(*list));
    assert(list);
    list->head = list->foot = NULL;
    list->count = 0;
    return list;
}

/* Returns TRUE (1) if the list is empty, FALSE (0) otherwise. */
int is_empty_list(list_t *list) {
    assert(list);
    return list->head == NULL && list->foot == NULL && list->count == 0;
}

/* Creates and returns a node holding the data. */
node_t *create_node(void *data) {
    assert(data);
    node_t *new = malloc(sizeof(*new));
    assert(new);
    new->data = data;
    new->next = NULL;
    return new;
}

/* Inserts the node to the foot of the list;
 * Returns the pointer to the modified list.
 */
list_t *insert_at_foot(list_t *list, node_t *new) {
    assert(list && new);
    new->next = NULL;

    if (is_empty_list(list)) {
        // first insertion into the list
        list->head = list->foot = new;
    } else {
        list->foot->next = new;
        list->foot = new;
    }
    list->count ++;

    return list;
}

/* Deletes the head of the list, and returns the deleted head.
 * Returns NULL if the list is already empty.
 */
node_t *delete_head(list_t *list) {
    assert(list);

    if (is_empty_list(list))
        return NULL;

    node_t *old_head = list->head;
    list->head = old_head->next;
    old_head->next = NULL; // clear the linkage

    if (list->head == NULL) // empty list now
        list->foot = NULL;
    
    list->count--;
    
    return old_head;
}

/* Deletes the `node` from the list.
 * `prev` is the original node preceding `node`.
 */
void delete_node_from_list(node_t *prev, node_t *node, list_t *list) {
    assert(list && node);
    if (is_empty_list(list))
        return;
    if (node == list->head) { // `node` was the head
        delete_head(list);
        return;
    }
    if (node == list->foot) { // `node` was the foot
        list->foot = prev;
    }

    if (prev)
        prev->next = node->next;
    node->next = NULL; // clear its linkage
    list->count--;
}

/* Frees all memory allocated to the list. */
void free_list(list_t *list) {
    assert(list);

    node_t *curr, *prev;
    curr = list->head;
    while (curr) {
        prev = curr;
        curr = curr->next;
        free(prev);
        prev = NULL;
    }
    free(list);
    list = NULL;
}

/* Appends list `l2` to `l1`, clears `l2`, and returns a pointer to `l1`. 
 */
list_t *append_list(list_t *l1, list_t *l2) {
    assert(l1 && l2);
    if (is_empty_list(l2)) // nothing to append
        return l1;
    if (is_empty_list(l1)) {
        // empty l1 -> 'replace' l1 with l2
        l1->head = l2->head;
        l1->foot = l2->foot;
    } else {
        // non-empty l1 -> attach l2 at l1's foot
        l1->foot->next = l2->head;
        l1->foot = l2->foot;
    }
    l1->count += l2->count;

    // clear l2
    l2->head = NULL;
    l2->foot = NULL;
    l2->count = 0;

    return l1;
}

/* Moves the node from list `l1` to the end of list `l2`, 
   and returns a pointer to list `l2`.
 * `prev` is the original node preceding `node` in `l1`.
 */
list_t *move_between_lists(list_t *l1, list_t *l2, 
        node_t *node, node_t *prev) {
    assert(l1 && l2 && node);

    delete_node_from_list(prev, node, l1); // remove from l1
    insert_at_foot(l2, node);

    return l2;
}