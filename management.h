/*-----------------------------------------------------------------------------
 * Project 1
 * Created by Angel He (angelh1@student.unimelb.edu.au) 25/03/2023
 * management.h :
              = the interface of the module `management` of the project 
              = provides the actions of an abstract process manager
 ----------------------------------------------------------------------------*/

#ifndef _MANAGEMENT_H_
#define _MANAGEMENT_H_

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "queue.h"
#include "memory.h"
#include "process_data.h"
#include "communicator.h"

#define TRANSCRIPT "%u,%s,process_name=%s," // general transcript format

typedef struct process_manager process_manager_t;
/* function pointer types for scheduling and memory strategies */
typedef process_t *(*scheduler_fp_t) (process_manager_t *);
typedef void (*memory_strategy_fp_t) (process_manager_t *);

enum scheduler {SJF, RR};                   // scheduling agorithms
enum memory_strategy {INFINITE, BEST_FIT};  // memory strategies

struct process_manager {
    int quantum;
    uint32_t current_sim_time;  // current simulation time

    queue_t *unsubmitted_queue; // automatically sorted by time arrived
    queue_t *input_queue;
    queue_t *ready_queue;
    process_t *running_process;

    memory_t *memory;
    scheduler_fp_t scheduler;
    memory_strategy_fp_t malloc_strategy;

    // for performance statistics
    uint64_t total_turnaround_time;
    uint32_t num_process;
    double total_time_overhead;
    double max_time_overhead;
};


/************** function declarations **************/

/* Creates, initializes, and returns a process manager.
 */
process_manager_t *create_manager(FILE *f, char *scheduler, 
        char *memory_strategy, int quantum);

/* Submits processes (moves processes from the unsubmitted 
   queue to the input queue in the manager).
 */
void submit_processes(process_manager_t *manager);

/* The manager allocates memory infinitely to submitted processes. 
 */
void infinite_malloc(process_manager_t *manager);

/* Manager allocates memory by best fit to submitted processes.
 */
void best_fit_malloc(process_manager_t *manager);

/* Returns the next process to run, 
   as determined by the Shortest Job First algorithm.
 * Returns NULL if no next process is found.
 */
process_t *shortest_job_first(process_manager_t *manager);

/* Returns the next process to run, 
   as determined by the Round-Robin scheduling algorithm.
*/
process_t *round_robin(process_manager_t *manager);

/* Checks whether the current process is completed.
 */
void check_current_process_completed(process_manager_t *manager);

/* The manager schedules the next process.
 */
void schedule_next_process(process_manager_t *manager);

/* Returns the total number of waiting processes 
   (in the manager's input and ready queue). 
 */
int count_waiting_processes(process_manager_t *manager);

/* The manager prints the execution transcript, 
   for the given process and state type.
 */
void print_transcript(process_manager_t *manager, process_t *process, 
        enum process_state type);

/* The manager prints the performance statistics. 
 */
void print_performance_stats(process_manager_t *manager);

/* Returns the average turnaround time of all processes, 
   rounded up to the nearest integer.
 */
int avg_turnaround_time(process_manager_t *manager);

/* The manager runs a cycle. 
 * Returns 0 in the last cycle (no processes left), 1 otherwise.
 */
int run_cycle(process_manager_t *manager);

/* Returns TRUE (1) if the manager finds that there are 
   no more processes to run, FALSE (0) otherwise. 
 */
int no_processes_left(process_manager_t *manager);

/* Wrapper function to run the process manager.
 */
void run(process_manager_t *manager);

/* Performs the clean up after the manager has finished running 
   (frees all memory).
 */
void clean_up(process_manager_t *manager);


#endif