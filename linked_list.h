/*-----------------------------------------------------------------------------
 * Project 1
 * Created by Angel He (angelh1@student.unimelb.edu.au) 24/03/2023
 * linked_list.h :
              = the interface of the module `linked_list` of the project 
              = provides a polymorphic singly-linked list
 ----------------------------------------------------------------------------*/

#ifndef _LIST_H_
#define _LIST_H_

typedef struct node node_t;

struct node {
    void *data;
	node_t *next;
};

typedef struct {
    node_t *head; // first node in the list
    node_t *foot; // last node in the list
    int count;    // number of nodes in the list
} list_t;


/************** function declarations **************/

/* Creates and returns an empty list. 
 */
list_t *make_empty_list();

/* Returns TRUE (1) if the list is empty, FALSE (0) otherwise. 
 */
int is_empty_list(list_t *list);

/* Creates and returns a node holding the data. 
 */
node_t *create_node(void *data);

/* Inserts the node to the foot of the list;
 * Returns the pointer to the modified list.
 */
list_t *insert_at_foot(list_t *list, node_t *new);

/* Deletes the head of the list, and returns the deleted head.
 * Returns NULL if the list is already empty.
 */
node_t *delete_head(list_t *list);

/* Deletes the `node` from the list.
 * `prev` is the original node preceding `node`.
 */
void delete_node_from_list(node_t *prev, node_t *node, list_t *list);

/* Frees all memory allocated to the list. 
 */
void free_list(list_t *list);

/* Appends list `l2` to `l1`, clears `l2`, and returns a pointer to `l1`. 
 */
list_t *append_list(list_t *l1, list_t *l2);

/* Moves the node from list `l1` to the end of list `l2`, 
   and returns a pointer to list `l2`.
 * `prev` is the original node preceding `node` in `l1`.
 */
list_t *move_between_lists(list_t *l1, list_t *l2, 
        node_t *node, node_t *prev);


#endif