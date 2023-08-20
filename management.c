/*-----------------------------------------------------------------------------
 * Project 1 
 * Created by Angel He (angelh1@student.unimelb.edu.au) 25/03/2023
 * management.c :
              = the implementation of the module `management` of the project 
 ----------------------------------------------------------------------------*/
 
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "management.h"

/* names for the scheduling and memory strategies */
const char *const scheduler_str[] = {[SJF] = "SJF", [RR] = "RR"};
const char *const memory_strategy_str[] = 
        {[INFINITE] = "infinite", [BEST_FIT] = "best-fit"};

/* functions for the scheduling and memory strategies */
const scheduler_fp_t scheduler_func[] = 
        {[SJF] = shortest_job_first, [RR] = round_robin};
const memory_strategy_fp_t memory_strategy_func[] = 
        {[INFINITE] = infinite_malloc, [BEST_FIT] = best_fit_malloc};

/* for the transcript */
const char *const process_state_str[] = 
        {[FINISHED] = "FINISHED", [FINISHED_PROCESS] = "FINISHED-PROCESS", 
         [READY] = "READY", [RUNNING] = "RUNNING"};

/* some 'private' helper functions */
static double round_to_two_dp(double d);
static uint32_t uint_safe_subtract(uint32_t u1, uint32_t u2);
static node_t *find_shortest_job(queue_t *ready_queue);


/* Creates, initializes, and returns a process manager.
 */
process_manager_t *create_manager(FILE *f, char *scheduler, 
        char *memory_strategy, int quantum) {
    assert(f && scheduler && memory_strategy);

    process_manager_t *manager = malloc(sizeof(*manager));
    assert(manager);

    manager->quantum = quantum;
    manager->unsubmitted_queue = build_queue(f); // load the processes
    manager->input_queue = make_empty_queue();
    manager->ready_queue = make_empty_queue();

    manager->running_process = NULL;
    manager->memory = initialize_memory();
    manager->current_sim_time = 0;

    manager->total_turnaround_time = 0;
    manager->num_process = manager->unsubmitted_queue->count;
    manager->total_time_overhead = 0;
    manager->max_time_overhead = 0;

    /* Assign the algorithms */
    manager->scheduler = NULL;
    manager->malloc_strategy = NULL;

    for (int i = SJF; i <= RR; i++) {
        if (strcmp(scheduler, scheduler_str[i]) == 0) {
            manager->scheduler = scheduler_func[i];
        }
    }
    if (!manager->scheduler) { // check input validity
        perror("Invalid scheduling algorithm");
        exit(EXIT_FAILURE);
    }

    for (int i = INFINITE; i <= BEST_FIT; i++) {
        if (strcmp(memory_strategy, memory_strategy_str[i]) == 0) {
            manager->malloc_strategy = memory_strategy_func[i];
        }
    }
    if (!manager->malloc_strategy) {
        perror("Invalid memory strategy");
        exit(EXIT_FAILURE);
    }

    return manager;
}

/* Submits processes (moves processes from the unsubmitted 
   queue to the input queue in the manager).
 */
void submit_processes(process_manager_t *manager) {
    if (is_empty_queue(manager->unsubmitted_queue)) // no process to submit
        return;

    node_t *curr;
    while ((curr = manager->unsubmitted_queue->head) && 
        process_arrived(curr->data, manager->current_sim_time)) {
        // move arrived processes to the input queue
        dequeue(manager->unsubmitted_queue);
        enqueue(manager->input_queue, curr);
    }
}

/* The manager allocates memory infinitely to submitted processes. 
 */
void infinite_malloc(process_manager_t *manager) {
    assert(manager);

    if (is_empty_queue(manager->input_queue)) // no process to malloc
        return;

    // move all processes in the input queue to the ready queue
    manager->ready_queue = 
            append_queue(manager->ready_queue, manager->input_queue);
}

/* Manager allocates memory by best fit to submitted processes.
 */
void best_fit_malloc(process_manager_t *manager) {
    assert(manager);
    if (is_empty_queue(manager->input_queue)) // no process to malloc
        return;

    node_t *curr = manager->input_queue->head;
    node_t *prev = NULL;
    process_t *curr_process;
    while (curr) {
        curr_process = (process_t *) curr->data;
        malloc_by_best_fit(manager->memory, curr_process);
        if (curr_process->address_assigned != NOT_ASSIGNED 
                && curr_process->state != READY) {
            // memory successfully allocated
            curr_process->state = READY;
            // store the next node before deletion
            node_t *node_to_move = curr;
            curr = curr->next; // `prev` remains
            // move from input to ready queue
            move_between_queues(manager->input_queue, manager->ready_queue, 
                    node_to_move, prev);
            print_transcript(manager, curr_process, READY);

        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}

/* Helper function to find and return the node for the shortest ready process.
 * Returns NULL if there are no more ready processes.
 */
static node_t *find_shortest_job(queue_t *ready_queue) {
    if (is_empty_queue(ready_queue)) // no ready processes available
        return NULL;

    node_t *curr = ready_queue->head;
    node_t *shortest_job = curr;
    // node-deletion requires the previous node in a (singly-linked) queue
    node_t *shortest_job_prev = NULL;

    while (curr->next) { // linear search
        if (cmp_by_service_time(curr->next->data, shortest_job->data) < 0) {
            shortest_job = curr->next;
            shortest_job_prev = curr;
        } else if (cmp_by_service_time(curr->next->data, 
                shortest_job->data) == 0) { // same service time
            if (cmp_by_time_arrived(curr->next->data, shortest_job->data) < 0) {
                // take the one that arrived earlier
                shortest_job = curr->next;
                shortest_job_prev = curr;
            } else if (cmp_by_time_arrived(curr->next->data, 
                    shortest_job->data) == 0 
                    && cmp_by_name(curr->next->data, shortest_job->data) < 0) {
                // same arrival time
                // take the one whose name comes first lexicographically
                shortest_job = curr->next;
                shortest_job_prev = curr;
            }
        }
        curr = curr->next;
    }

    delete_node_from_queue(shortest_job_prev, shortest_job, ready_queue);

    return shortest_job;
}

/* Returns the next process to run, 
   as determined by the Shortest Job First algorithm.
 * Returns NULL if no next process is found.
 */
process_t *shortest_job_first(process_manager_t *manager) {
    assert(manager);

    if (manager->running_process 
            && manager->running_process->state == RUNNING) {
        // there is a running process and it has not finished
        return manager->running_process;
    }

    // no running process or that process has finished
    node_t *shortest_job = find_shortest_job(manager->ready_queue);
    if (!shortest_job) // no job found;
        return NULL;

    process_t *shortest_process = shortest_job->data;
    free(shortest_job);  // unwrap
    shortest_job = NULL;
    
    return shortest_process;
}

/* Returns the next process to run, 
   as determined by the Round-Robin scheduling algorithm.
*/
process_t *round_robin(process_manager_t *manager) {
    assert(manager);
    if (is_empty_queue(manager->ready_queue)) {
        // no other ready processes -> keep running this one
        return manager->running_process;
    }

    // switch regardless of whether the current process has finished
    node_t *next_job = dequeue(manager->ready_queue);
    
    if (manager->running_process 
            && manager->running_process->state == RUNNING) {
        // unfinished process put to the back of the ready queue
        set_process_suspended(manager->running_process);
        suspend_process(manager->running_process, manager->current_sim_time);
        enqueue(manager->ready_queue, create_node(manager->running_process));
    }

    process_t *next_process = next_job->data;  // unwrap
    free(next_job); 
    next_job = NULL;

    return next_process;
}

/* Checks whether the current process is completed.
 */
void check_current_process_completed(process_manager_t *manager) {
    assert(manager);
    if (!manager->running_process) // no process currently running
        return;

    if (manager->running_process->time_remaining > 0) 
        return;  // current process not completed

    /* current process completed */
    // deallocate memory
    demalloc(manager->running_process, manager->memory);
    // terminate the process and report
    set_process_terminated(manager->running_process, manager->current_sim_time);
    print_transcript(manager, manager->running_process, FINISHED);
    terminate_process(manager->running_process, manager->current_sim_time);
    print_transcript(manager, manager->running_process, FINISHED_PROCESS);

    /* store the info */
    manager->total_turnaround_time += 
            get_turnaround_time(manager->running_process);
    double time_overhead = get_time_overhead(manager->running_process);
    manager->total_time_overhead += time_overhead;
    if (time_overhead > manager->max_time_overhead)
        manager->max_time_overhead = time_overhead;
    
    // remove the completed process
    free(manager->running_process);
    manager->running_process = NULL;
}

/* The manager schedules the next process.
 */
void schedule_next_process(process_manager_t *manager) {
    assert(manager);

    process_t *next_process = manager->scheduler(manager);
    if (!next_process) // no next job scheduled
        return;
    if (next_process == manager->running_process) {
        // current process continues
        resume_process(manager->running_process, manager->current_sim_time);
        return;
    }

    // switch process
    set_process_running(next_process);
    manager->running_process = next_process;
    print_transcript(manager, manager->running_process, RUNNING);

    if (first_run(manager->running_process)) { 
        // process runs for the first time
        create_process(manager->running_process, manager->current_sim_time);
    } else {
        resume_process(manager->running_process, manager->current_sim_time);
    }
}

/* Returns the total number of waiting processes 
   (in the manager's input and ready queue). 
 */
int count_waiting_processes(process_manager_t *manager) {
    return (manager->input_queue->count + manager->ready_queue->count);
}

/* Returns TRUE (1) if the manager finds that there are 
   no more processes to run, FALSE (0) otherwise. 
 */
int no_processes_left(process_manager_t *manager) {
    assert(manager);
    return is_empty_queue(manager->unsubmitted_queue)
            && is_empty_queue(manager->input_queue)
            && is_empty_queue(manager->ready_queue)
            && !manager->running_process; // no currently running process
}

/* The manager prints the execution transcript, 
   for the given process and state type.
 */
void print_transcript(process_manager_t *manager, process_t *process, 
        enum process_state type) {
    assert(manager && process);

    printf(TRANSCRIPT, manager->current_sim_time, 
            process_state_str[type], process->name);
    switch (type) {
        case READY:
            printf("assigned_at=%d\n", 
                    ((mem_entry_t *) process->address_assigned->data)->start);
            break;
        case RUNNING:
            printf("remaining_time=%u\n", process->time_remaining);
            break;
        case FINISHED:
            printf("proc_remaining=%d\n", count_waiting_processes(manager));
            break;
        case FINISHED_PROCESS:
            printf("sha=%s\n", manager->running_process->hash);
            break;
        default:
            printf("Unknown transcript type\n");
            break;
    }
}

/* Returns the average turnaround time of all processes, 
   rounded up to the nearest integer.
 */
int avg_turnaround_time(process_manager_t *manager) {
    return ceil((double) manager->total_turnaround_time / manager->num_process);
} 

/* Helper function to return the double value rounded to two decimal places.
 */
static double round_to_two_dp(double d) {
    // C rounds 0.355 to 0.35 rather than 0.36
    return round(d * 100) / 100;
}

/* Helper function to calculate and return max(0, u1 - u2). 
 */
static uint32_t uint_safe_subtract(uint32_t u1, uint32_t u2) {
    return u1 < u2 ? 0 : (u1 - u2);
}

/* The manager prints the performance statistics. 
 */
void print_performance_stats(process_manager_t *manager) {
    printf("Turnaround time %d\n", avg_turnaround_time(manager));
    printf("Time overhead %.2lf %.2lf\n", 
        round_to_two_dp(manager->max_time_overhead), 
        round_to_two_dp(manager->total_time_overhead / manager->num_process));
    printf("Makespan %u\n", manager->current_sim_time);
}

/* The manager runs a cycle. 
 * Returns 0 in the last cycle (no processes left), 1 otherwise.
 */
int run_cycle(process_manager_t *manager) {
    /* check if the running process in the previous cycle has completed */
    check_current_process_completed(manager);
    /* submit processes to input queue */
    submit_processes(manager);
    /* allocate memory to processes in input queue */
    manager->malloc_strategy(manager);
    /* determine next process to run */
    schedule_next_process(manager);

    if (no_processes_left(manager))  // stop timing in the last cycle
        return 0;
    
    manager->current_sim_time += manager->quantum;
    if (manager->running_process)
        manager->running_process->time_remaining = 
                uint_safe_subtract(manager->running_process->time_remaining, 
                        manager->quantum); // to prevent overflow

    return 1;
}

/* Wrapper function to run the process manager.
 */
void run(process_manager_t *manager) {
    assert(manager);
    while (run_cycle(manager));
    print_performance_stats(manager);
    clean_up(manager);
}

/* Performs the clean up after the manager has finished running 
   (frees all memory).
 */
void clean_up(process_manager_t *manager) {
    // the queues should be empty now
    free(manager->unsubmitted_queue);
    manager->unsubmitted_queue = NULL;
    free(manager->input_queue);
    manager->input_queue = NULL;
    free(manager->ready_queue);
    manager->ready_queue = NULL;
    free_memory(manager->memory);
    manager->memory = NULL;
}