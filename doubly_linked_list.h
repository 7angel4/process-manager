/*-----------------------------------------------------------------------------
 * Project 1
 * Created by Angel He (angelh1@student.unimelb.edu.au) 24/03/2023
 * doubly_linked_list.h :
              = the interface of the module `linked_list` of the project 
              = provides a polymorphic doubly-linked list
 ----------------------------------------------------------------------------*/

#ifndef _DOUBLY_LINKED_LIST_H_
#define _DOUBLY_LINKED_LIST_H_

typedef struct double_node double_node_t;

struct double_node { // node with two linkages
    void *data;
    double_node_t *prev;
    double_node_t *next;
};

typedef struct {
    double_node_t *head;
    double_node_t *foot;
} doubly_linked_list_t;


/************** function declarations **************/

/* Creates and returns an empty doubly-linked list. 
 */
doubly_linked_list_t *make_empty_dll();

/* Returns TRUE (1) if the doubly-linked list is empty, FALSE (0) otherwise. 
 */
int is_empty_dll(doubly_linked_list_t *list);

/* Creates and returns a double-node holding the data. 
 */
double_node_t *create_double_node(void *data);

/* Inserts the node to the foot of the doubly-linked list;
 * Returns the pointer to the modified list.
 */
doubly_linked_list_t *insert_at_dll_foot(doubly_linked_list_t *list, 
        double_node_t *new);

/* Inserts the node n2 behind n1.
 * Returns the pointer to the modified doubly-linked list.
 */
doubly_linked_list_t *insert_behind(double_node_t *n1, double_node_t *n2, 
        doubly_linked_list_t *list);

/* Deletes the head of the doubly-linked list, and returns the deleted head.
 * Returns NULL if the list is already empty.
 */
double_node_t *delete_dll_head(doubly_linked_list_t *list);

/* Deletes the node from the doubly-linked list, 
   and frees the deleted node.
 */
void delete_double_node(double_node_t *node, doubly_linked_list_t *dll);

/* Frees all memory allocated to the doubly-linked list. 
 */
void free_dll(doubly_linked_list_t *dll);


#endif