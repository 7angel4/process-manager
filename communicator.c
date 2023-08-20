/*-----------------------------------------------------------------------------
 * Project 1 
 * Created by Angel He (angelh1@student.unimelb.edu.au) 28/03/2023
 * communicator.c :
              = the implementation of the module `communicator` of the project 
 ----------------------------------------------------------------------------*/

#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include "communicator.h"

/* Creates a (real) process and attaches it to the given (simulated) process.
 */
void create_process(process_t *process, uint32_t time) {
    assert(process);
    pipe(process->write_fd);
    pipe(process->read_fd);
    
    pid_t child_pid;
    if ((child_pid = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) { // child process
        prctl(PR_SET_PDEATHSIG, SIGHUP);
        close(process->read_fd[WRITE_END]);
        dup2(process->read_fd[READ_END], STDIN_FILENO); // redirect stdin
        close(process->read_fd[READ_END]);

        close(process->write_fd[READ_END]);
        dup2(process->write_fd[WRITE_END], STDOUT_FILENO); // redirect stdout
        close(process->write_fd[WRITE_END]);

        char *args[] = {"process", process->name, NULL};
        execv(PROCESS_PATH, args);

    } else { // parent process

        // attach the real process to the simulated `process`
        process->pid = child_pid;
        close(process->read_fd[READ_END]);
        close(process->write_fd[WRITE_END]);

        uint8_t sent_byte = send_time(process, time);
        uint8_t received_byte = read_byte(process);
        // verify the last sent byte and received byte match
        if (sent_byte != received_byte) {
            perror("Received byte different to the last sent byte.");
            exit(EXIT_FAILURE);
        }
    }
}

/* Sends the simulation time to stdin of process, in Big Endian Byte Ordering, 
   and returns the least significant byte sent.
 */
uint8_t send_time(process_t *process, uint32_t time) {
    assert(process);

    uint8_t byte;
    for (int i = 3; i >= 0; i--) { // send the most significant byte first
        byte = time >> (8*i) & 0xFF;
        write(process->read_fd[WRITE_END], &byte, 1);
    }
    return byte; // least significant byte
}

/* Reads one byte from stdout of process, and returns it.
 */
uint8_t read_byte(process_t *process) {
    assert(process);

    uint8_t byte;
    int n = read(process->write_fd[READ_END], &byte, 1);
    if (n != 1) {
        perror("read");
        exit(1);
    }
    return byte;
}

/* Suspends the process.
 */
void suspend_process(process_t *process, uint32_t time) {
    assert(process);

    send_time(process, time);
    kill(process->pid, SIGTSTP);

    int wstatus;
    pid_t w;
    do {  // wait for process to enter a stopped state
        w = waitpid(process->pid, &wstatus, WUNTRACED);
        if (w == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
    } while (!WIFSTOPPED(wstatus));
}

/* Resumes or continues the process.
 */
void resume_process(process_t *process, uint32_t time) {
    assert(process);

    uint8_t sent_byte = send_time(process, time);
    kill(process->pid, SIGCONT);

    uint8_t received_byte = read_byte(process);
    if (sent_byte != received_byte) {
        perror("Read byte different to the last sent byte.");
        exit(EXIT_FAILURE);
    }
}

/* Terminates the process.
 */
void terminate_process(process_t *process, uint32_t time) {
    assert(process);

    send_time(process, time);
    close(process->read_fd[WRITE_END]); // no more input from parent
    kill(process->pid, SIGTERM);
    
    // read the 64 byte string from stdout of process
    int n = read(process->write_fd[READ_END], process->hash, HASH_LEN);
    if (n != HASH_LEN) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    process->hash[n] = '\0';
    close(process->write_fd[READ_END]); // no more output to parent
}