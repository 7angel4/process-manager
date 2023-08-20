/*-----------------------------------------------------------------------------
 * Project 1
 * Created by Angel He (angelh1@student.unimelb.edu.au) 25/03/2023
 * process_data.h :
              = the interface of the module `process_data` of the project 
              = simulates a process and manipulates the relevant data
 ----------------------------------------------------------------------------*/

#ifndef _PROCESS_DATA_H_
#define _PROCESS_DATA_H_

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include "doubly_linked_list.h"

#define MAX_NAME_LEN 8    // max length of a process name
#define HASH_LEN 64   // length of the hash from stdout of `process`
/* On process creation */
#define NOT_ASSIGNED NULL // no memory address assigned yet
#define NOT_SUBMITTED -1  // process not yet submitted


enum process_state {FINISHED, FINISHED_PROCESS, READY, RUNNING};

typedef struct {
    uint32_t time_arrived;
    char name[MAX_NAME_LEN+1];
    uint32_t service_time;
    int memory_requirement;

    double_node_t *address_assigned; // assigned address in memory
    enum process_state state;
    uint32_t time_finished;
    uint32_t time_remaining;

    /* to attach with the real process */
    pid_t pid;
    int write_fd[2];           // pipe for sending the hex string and bytes
    int read_fd[2];            // pipe for receiving time
    uint8_t hash[HASH_LEN+1];  // SHA-256 hash output by `process`

} process_t;


/************** function declarations **************/

/* Creates and returns a process holding data read from the file `f`.
 * Returns NULL if a process could not be succesfully created.
 */
 process_t *read_process(FILE *f);

/* Compares the arrival time of two processes.
 * Returns negative if `p1` arrived earlier, positive if `p2` arrived earlier, 
   0 otherwise.
 */
int cmp_by_time_arrived(process_t *p1, process_t *p2);

/* Compares the service time of two processes.
 * Returns negative if `p1` requires less service time, 
   positive if `p2` requires less, 0 otherwise.
 */
int cmp_by_service_time(process_t *p1, process_t *p2);

/* Compares the name of two processes.
 * Returns negative if `p1` requires less service time, 
   positive if `p2` requires less, 0 otherwise.
 */
int cmp_by_name(process_t *p1, process_t *p2);

/* Sets the process state as terminated.
*/
void set_process_terminated(process_t *process, uint32_t current_sim_time);

/* Sets the process state as running.
*/
void set_process_running(process_t *process);

/* Sets the (previously running) process as suspended.
*/
void set_process_suspended(process_t *process);

/* Returns TRUE (1) if the process is run for the first time, 
   FALSE (0) otherwise.
*/
int first_run(process_t *process);

/* Returns TRUE (1) if the process has arrived by the given time, 
   FALSE (0) otherwise.
*/
int process_arrived(process_t *process, uint32_t time);

/* Returns the turnaround time of the process.
*/
uint32_t get_turnaround_time(process_t *process);

/* Returns the time overhead of the process.
*/
double get_time_overhead(process_t *process);

/* Prints the information about a process (for sanity checking).
*/
void print_process(process_t *process);


#endif