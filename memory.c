/*-----------------------------------------------------------------------------
 * Project 1 
 * Created by Angel He (angelh1@student.unimelb.edu.au) 25/03/2023
 * memory.c :
              = the implementation of the module `memory` of the project 
 ----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <assert.h>
#include "memory.h"

/* Creates a memory entry with the given type, starting address and length.
 */
mem_entry_t *create_mem_entry(int type, int start, int length) {
    mem_entry_t *entry = malloc(sizeof(*entry));
    assert(entry);
    
    entry->type = type;
    entry->start = start;
    entry->length = length;
    
    return entry;
}

/* Creates, initializes, and returns the memory structure.
 */
memory_t *initialize_memory() {
    // memory initialized as a single hole of size MEMORY_CAPACITY (2048) MB
    mem_entry_t *entry = create_mem_entry(HOLE, 0, MEMORY_CAPACITY);
    memory_t *memory = make_empty_dll();
    memory = insert_at_dll_foot(memory, create_double_node(entry));
    return memory;
}

/* Allocates memory to the process by best fit.
 */
void malloc_by_best_fit(memory_t *memory, process_t *process) {
    assert(memory && process);
    double_node_t *curr = memory->head;
    assert(curr);
    mem_entry_t *curr_entry;

    int smallest_hole_size = MEMORY_CAPACITY+1;
    double_node_t *best_hole = NULL;
    
    while (curr) {
        curr_entry = curr->data;
        if (curr_entry->type == HOLE 
                && curr_entry->length >= process->memory_requirement
                && curr_entry->length < smallest_hole_size) {
            // this hole can fit the program and is a better (smaller) fit
            smallest_hole_size = curr_entry->length;
            best_hole = curr;
        }
        curr = curr->next;
    }

    //printf("best hole: %d\n", ((mem_entry_t *)(best_hole->data))->start);
    if (best_hole) // only allocate memory to the process if there is a fit
        allocate_process(best_hole, process, memory);
}

/* Allocates the node (in the given memory list) to the process.
 */
void allocate_process(double_node_t *node, process_t *process, 
        memory_t *memory) {
    assert(node && process);

    ((mem_entry_t *) node->data)->type = PROCESS;
    process->address_assigned = node;

    // split out the portion unused by the process
    split_mem_entry(node, process->memory_requirement, memory);
}

/* Splits the memory entry held in the node into two:
   - a process type of the given size, and 
   - a hole type (if necessary), following the process entry.
 */
void split_mem_entry(double_node_t *node, int process_size, memory_t *memory) {
    assert(node && memory);
    mem_entry_t *entry = node->data;

    // unused portion becomes the hole
    int hole_size = entry->length - process_size;
    assert(hole_size >= 0); // hole must fit
    if (hole_size == 0) // exact fit
        return;
    
    entry->length = process_size;
    // new hole to hold the remaining memory
    mem_entry_t *new_hole = 
            create_mem_entry(HOLE, entry->start + process_size, hole_size);
    double_node_t *new_node = create_double_node(new_hole); // wrap up

    // new hole follows the original (process type) entry
    insert_behind(node, new_node, memory);
}

/* Deallocates memory for the process, 
   and merges the adjacent holes in the memory list if possible.
*/
void demalloc(process_t *process, memory_t *memory) {
    assert(process);
    if (!process->address_assigned) {
        // no address assigned (e.g. using infinite malloc) -> do nothing
        return;
    }

    merge_adjacent_holes(process->address_assigned, memory);
}

/* Merges the adjacent holes to the given node in memory.
 * Frees the memory at the node even if no merging is done.
 */
void merge_adjacent_holes(double_node_t *node, memory_t *memory) {
    assert(node && memory);
    // data for the new hole
    mem_entry_t *entry = node->data;
    int new_hole_length = entry->length;
    int new_hole_start = entry->start;

    mem_entry_t *tmp_entry; // temporary placeholder for a memory entry
    if (node->prev) {
        tmp_entry = (mem_entry_t *) node->prev->data;
        if (tmp_entry->type == HOLE) {
            // previous node is a hole
            new_hole_start = tmp_entry->start;
            new_hole_length += tmp_entry->length;

            // previous node no longer needed (merged into this node)
            delete_double_node(node->prev, memory);
        }
    }

    if (node->next) {
        tmp_entry = (mem_entry_t *) node->next->data;
        if (tmp_entry->type == HOLE) {
            // next node is a hole
            new_hole_length += tmp_entry->length;
            // next node no longer needed
            delete_double_node(node->next, memory);
        }
    }

    // replace data in the current node, as if it's a new hole node
    // (these steps are performed even if no merging was done)
    entry->length = new_hole_length;
    entry->start = new_hole_start;
    entry->type = HOLE;
}

/* Frees the memory. 
 */
void free_memory(memory_t *memory) {
    assert(memory);
    // memory should be a single hole node in the end, 
    // once all processes are completed
    free(memory->head->data);
    memory->head->data = NULL;
    free(memory->head);
    memory->head = NULL;
    free(memory);
    memory = NULL;
}