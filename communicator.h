/*-----------------------------------------------------------------------------
 * Project 1
 * Created by Angel He (angelh1@student.unimelb.edu.au) 28/03/2023
 * communicator.h :
              = the interface of the module `communicator` of the project 
              = creates and communicates with real processes
 ----------------------------------------------------------------------------*/

#ifndef _COMMUNICATOR_H_
#define _COMMUNICATOR_H_

#include <stdint.h>
#include "process_data.h"

#define PROCESS_PATH "./process"  // executable in the current directory
#define READ_END 0                // read end of a pipe
#define WRITE_END 1               // write end of a pipe


/************** function declarations **************/

/* Creates a (real) process and attaches it to the given (simulated) process.
 */
void create_process(process_t *process, uint32_t time);

/* Sends the simulation time to stdin of process, in Big Endian Byte Ordering, 
   and returns the least significant byte sent.
 */
uint8_t send_time(process_t *process, uint32_t time);

/* Reads one byte from stdout of process, and returns it.
 */
uint8_t read_byte(process_t *process);

/* Suspends the process.
 */
void suspend_process(process_t *process, uint32_t time);

/* Resumes or continues the process.
 */
void resume_process(process_t *process, uint32_t time);

/* Terminates the process.
 */
void terminate_process(process_t *process, uint32_t time);


#endif