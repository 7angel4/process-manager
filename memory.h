/*-----------------------------------------------------------------------------
 * Project 1
 * Created by Angel He (angelh1@student.unimelb.edu.au) 25/03/2023
 * memory.h :
              = the interface of the module `memory` of the project 
              = simulates memory in the form of a doubly-linked list
 ----------------------------------------------------------------------------*/

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "process_data.h"
#include "doubly_linked_list.h"

#define MEMORY_CAPACITY 2048  // memory capacity in MB
#define PROCESS 1             // process type entry (occupied memory)
#define HOLE 0                // hole type entry (free memory)

/* data held in a node of the memory 'list' */
typedef struct {
    int type;    // PROCESS or HOLE
    int start;   // starting address
    int length;  // size of this entry
} mem_entry_t;

typedef doubly_linked_list_t memory_t; // memory as a doubly-linked list


/************** function declarations **************/

/* Creates a memory entry with the given type, starting address and length.
 */
mem_entry_t *create_mem_entry(int type, int start, int length);

/* Creates, initializes, and returns the memory structure.
 */
memory_t *initialize_memory();

/* Allocates memory to the process by best fit.
 */
void malloc_by_best_fit(memory_t *memory, process_t *process);

/* Allocates the node (in the given memory list) to the process.
 */
void allocate_process(double_node_t *node, process_t *process, 
        memory_t *memory);

/* Splits the memory entry held in the node into two:
   - a process type of the given size, and 
   - a hole type (if necessary), following the process entry.
 */
void split_mem_entry(double_node_t *node, int process_size, memory_t *memory);

/* Deallocates memory for the process, 
   and merges the adjacent holes in the memory list if possible.
*/
void demalloc(process_t *process, memory_t *memory);

/* Merges the adjacent holes to the given node in memory.
 * Frees the memory at the node even if no merging is done.
 */
void merge_adjacent_holes(double_node_t *node, memory_t *memory);

/* Frees the memory. 
 */
void free_memory(memory_t *memory);


#endif