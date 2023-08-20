/*-----------------------------------------------------------------------------
 * Project 1 
 * Created by Angel He (angelh1@student.unimelb.edu.au) 25/03/2023
 * process_data.c :
              = the implementation of the module `process_data` of the project 
 ----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "process_data.h"

/* Creates and returns a process holding data read from the file `f`.
 * Returns NULL if a process could not be succesfully created.
 */
process_t *read_process(FILE *f) {
    assert(f);
    uint32_t time_arrived, service_time;
    int memory_requirement;
    char name[MAX_NAME_LEN+1];

    if (fscanf(f, "%u %s %u %d", &time_arrived, name, &service_time, 
            &memory_requirement) != 4) {
        // incorrect number of values read
        return NULL;
    }

    /* create the process */
    process_t *process = malloc(sizeof(*process));
    assert(process);

    /* store the data */
    process->time_arrived = time_arrived;
    strcpy(process->name, name);
    process->service_time = service_time;
    process->time_remaining = service_time;
    process->memory_requirement = memory_requirement;

    process->address_assigned = NOT_ASSIGNED;
    process->state = NOT_SUBMITTED;

    return process;
}

/* Compares the arrival time of two processes.
 * Returns negative if `p1` arrived earlier, positive if `p2` arrived earlier, 
   0 otherwise.
 */
int cmp_by_time_arrived(process_t *p1, process_t *p2) {
    assert(p1 && p2);
    if (p1->time_arrived < p2->time_arrived) return -1;
    if (p1->time_arrived > p2->time_arrived) return 1;
    return 0;
}

/* Compares the service time of two processes.
 * Returns negative if `p1` requires less service time, 
   positive if `p2` requires less, 0 otherwise.
 */
int cmp_by_service_time(process_t *p1, process_t *p2) {
    assert(p1 && p2);
    if (p1->service_time < p2->service_time) return -1;
    if (p1->service_time > p2->service_time) return 1;
    return 0;
}

/* Compares the name of two processes.
 * Returns negative if `p1` requires less service time, 
   positive if `p2` requires less, 0 otherwise.
 */
int cmp_by_name(process_t *p1, process_t *p2) {
    assert(p1 && p2);
    return strcmp(p1->name, p2->name);
}

/* Sets the process state as terminated.
*/
void set_process_terminated(process_t *process, uint32_t current_sim_time) {
    process->time_remaining = 0;
    process->address_assigned = NOT_ASSIGNED;
    process->state = FINISHED;
    process->time_finished = current_sim_time;
}

/* Sets the process state as running.
*/
void set_process_running(process_t *process) {
    process->state = RUNNING;
}

/* Sets the (previously running) process as suspended.
*/
void set_process_suspended(process_t *process) {
    process->state = READY;
}

/* Returns TRUE (1) if the process is run for the first time, 
   FALSE (0) otherwise.
*/
int first_run(process_t *process) {
    assert(process);
    return process->service_time == process->time_remaining;
}

/* Returns TRUE (1) if the process has arrived by the given time, 
   FALSE (0) otherwise.
*/
int process_arrived(process_t *process, uint32_t time) {
    assert(process);
    return process->time_arrived <= time;
}

/* Returns the turnaround time of the process.
*/
uint32_t get_turnaround_time(process_t *process) {
    return process->time_finished - process->time_arrived;
}

/* Returns the time overhead of the process.
*/
double get_time_overhead(process_t *process) {
    return (double) get_turnaround_time(process) / process->service_time;
}

/* Prints the information about a process (for sanity checking).
*/
void print_process(process_t *process) {
    printf("%u %s %u %d %u\n", process->time_arrived, process->name, 
            process->service_time, process->memory_requirement, 
            process->time_remaining);
}