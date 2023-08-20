/*-----------------------------------------------------------------------------
 * Project 1
 * Created by Angel He (angelh1@student.unimelb.edu.au) 24/03/2023
 * queue.c :
              = the implementation of the module `queue` of the project
 ----------------------------------------------------------------------------*/

#include <assert.h>
#include "queue.h"

/* Creates and returns an empty queue.
 */
queue_t *make_empty_queue() {
    return make_empty_list();
}

/* Returns TRUE (1) if the queue is empty, FALSE (0) otherwise.
*/
int is_empty_queue(queue_t *queue) {
    return is_empty_list(queue);
}

/* Creates and returns a node holding the process.
 */
node_t *create_process_node(process_t *process) {
    return create_node(process);
}

/* Enqueues the node and returns the pointer to the queue.
 */
queue_t *enqueue(queue_t *queue, node_t *node) {
    return insert_at_foot(queue, node);
}

/* Dequeues a node and returns the pointer to that node.
 */
node_t *dequeue(queue_t *queue) {
    return delete_head(queue);
}

/* Builds a queue of processes from the input file `f`.
 * Returns a pointer to the built queue.
 */
queue_t *build_queue(FILE *f) {
    assert(f);

    queue_t *queue = make_empty_queue();
    queue->head = queue->foot = NULL;
    process_t *process_data;
    node_t *node;

    while ((process_data = read_process(f))) {
        node = create_node(process_data);
        queue = enqueue(queue, node);
    }

    return queue;
}

/* Prints the queue (for sanity checking). 
 */
void print_queue(queue_t *queue) {
    assert(queue);
    node_t *curr = queue->head;
    while (curr) {
        print_process((process_t *) curr->data);
        curr = curr->next;
    }
}

/* Appends queue `q2` to `q1`, clears `q2`, and returns a pointer to `q1`. 
 */
queue_t *append_queue(queue_t *q1, queue_t *q2) {
    return append_list(q1, q2);
}

/* Moves the node from queue `q1` to the end of queue `q2`, 
   and returns a pointer to queue `q2`.
 * `prev` is the original node preceding `node` in `q1`.
 */
queue_t *move_between_queues(queue_t *q1, queue_t *q2, 
        node_t *node, node_t *prev) {
    return move_between_lists(q1, q2, node, prev);
}

/* Deletes the `node` from the queue.
 * `prev` is the original node preceding `node`.
 */
void delete_node_from_queue(node_t *prev, node_t *node, queue_t *q) {
    delete_node_from_list(prev, node, q);
}

/* Frees all memory allocated to the queue.
 */
void free_queue(queue_t *queue) {
    free_list(queue);
}