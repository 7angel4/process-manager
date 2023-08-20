/*-----------------------------------------------------------------------------
 * Project 1: Process Management
 * Created by Angel He (angelh1@student.unimelb.edu.au) 25/03/2023
 * 
 * This project involves the implementation of a process manager capable of 
   allocating memory to processes and scheduling them for execution.
 * main.c :
              = the main program of the project
 *
 * The executable is named `allocate`.
 * Usage: 
   allocate -f <filename> -s (SJF | RR) -m (infinite | best-fit) -q (1 | 2 | 3)
 *
 * -f <filename>       : a relative/absolute path to the input file describing 
                         the processes. 
 * -s <scheduler>      : scheduler is one of {SJF, RR}.
 * -m <memory-strategy>: memory-strategy is one of {infinite, best-fit}.
 * -q <quantum>        : quantum is one of {1, 2, 3}.
 ----------------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "management.h"

#define IMPLEMENTS_REAL_PROCESS

#define FILE_OPT 'f'
#define SCHEDULER_OPT 's'
#define MEMORY_OPT 'm'
#define QUANTUM_OPT 'q'
#define NUM_ARGS 4

void process_args(int argc, char **argv, int *filename_id, int *scheduler_id, 
    int *memory_strategy_id, int *quantum);


int main(int argc, char **argv) {
    int filename_id, scheduler_id, memory_strategy_id, quantum;
    process_args(argc, argv, &filename_id, &scheduler_id, 
            &memory_strategy_id, &quantum);

    FILE *f = fopen(argv[filename_id], "r");
    assert(f);

    // build the manager
    process_manager_t *process_manager = create_manager(f, argv[scheduler_id], 
            argv[memory_strategy_id], quantum);
    fclose(f);

    // run the manager
    run(process_manager);
    // remove the manager
    free(process_manager);
    process_manager = NULL;

    return 0;
}

/* Extracts the command line arguments, and stores the associated index/value
   in the corresponding pointers.
 */
void process_args(int argc, char **argv, int *filename_id, int *scheduler_id, 
    int *memory_strategy_id, int *quantum) {
    
    int c;
    opterr = 0;
    int values_read = 0;
    
    while ((c = getopt(argc, argv, "f:s:m:q:")) != -1) {
        switch (c) {
            case FILE_OPT:
                // optind = index of the next element to be processed in argv
                *filename_id = optind-1;
                values_read++;
                break;
            case SCHEDULER_OPT:
                *scheduler_id = optind-1;
                values_read++;
                break;
            case MEMORY_OPT:
                *memory_strategy_id = optind-1;
                values_read++;
                break;
            case QUANTUM_OPT:
                *quantum = atoi(optarg);
                values_read++;
                break;
            case ':':
                printf("Option needs a value.\n");
                break;
            case '?':
                printf("Unknown option.\n");
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }
    
    if (values_read != NUM_ARGS) {
        perror("Invalid number of arguments.\n");
        exit(EXIT_FAILURE);
    }
}