/*-----------------------------------------------------------------------------
 * Project 1
 * Created by Angel He (angelh1@student.unimelb.edu.au) 24/03/2023
 * queue.h :
              = the interface of the module `queue` of the project 
              = a wrapper of the polymorphic `linked_list` module 
                - the data in each node in the queue is a process
 ----------------------------------------------------------------------------*/

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdio.h>
#include "process_data.h"
#include "linked_list.h"

typedef struct node node_t;
typedef list_t queue_t;


/************** function declarations **************/

/* Creates and returns an empty queue.
 */
queue_t *make_empty_queue();

/* Returns TRUE (1) if the queue is empty, FALSE (0) otherwise.
*/
int is_empty_queue(queue_t *queue);

/* Dequeues a node and returns the pointer to that node.
 */
node_t *dequeue(queue_t *queue);

/* Creates and returns a node holding the process.
 */
node_t *create_process_node(process_t *process);

/* Inserts the footpath data as a node to the foot of the queue;
 * Returns the pointer to the modified queue 
 */
queue_t *enqueue(queue_t *queue, node_t *node);

/* Builds a dictionary in the form of a linked queue
   from the input file given by `filename`;
 * Returns a pointer to the queue built
 */
queue_t *build_queue(FILE *f);

/* Frees all memory allocated to the queue 
 */
void free_queue(queue_t *queue);

/* Prints the queue (for sanity checking). 
 */
void print_queue(queue_t *queue);

/* Appends queue `q2` to `q1`, clears `q2`, and returns a pointer to `q1`. 
 */
queue_t *append_queue(queue_t *q1, queue_t *q2);

/* Moves the node from queue `q1` to the end of queue `q2`, 
   and returns a pointer to queue `q2`.
 * `prev` is the original node preceding `node` in `q1`.
 */
queue_t *move_between_queues(queue_t *q1, queue_t *q2, 
        node_t *node, node_t *prev);

/* Deletes the `node` from the queue.
 * `prev` is the original node preceding `node`.
 */
void delete_node_from_queue(node_t *prev, node_t *node, queue_t *q);


#endif