/****************
LE2: Introduction to Unnamed Pipes
****************/
#include <unistd.h> // pipe, fork, dup2, execvp, close
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main () {
    // lists all the files in the root directory in the long format
    char* cmd1[] = {(char*) "ls", (char*) "-al", (char*) "/", nullptr};
    // translates all input from lowercase to uppercase
    char* cmd2[] = {(char*) "tr", (char*) "a-z", (char*) "A-Z", nullptr};

    // TODO: add functionality
    // Create pipe
    int fd[2];
    int orig = dup(STDIN_FILENO);
    int orig_o = dup(STDOUT_FILENO);

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }
    
    // Create child to run first command
    pid_t pid = fork();
    // In child, redirect output to write end of pipe
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else {
        dup2(fd[1], STDOUT_FILENO);
        // Close the read end of the pipe on the child side.
        close(fd[0]);
        // In child, execute the command
        execvp(cmd1[0], cmd1);
        close(fd[1]);
    }  
    // Create another child to run second command
    pid_t pid2 = fork();

    if (pid2 < 0) {
        perror("fork2");
        exit(1);
    } else {
    // In child, redirect input to the read end of the pipe
        dup2(fd[0], STDIN_FILENO);
    // Close the write end of the pipe on the child side
        close(fd[1]);
    // Execute the second command.
        execvp(cmd2[0], cmd2);
        close(fd[0]);
    }
    
    // Reset the input and output file descriptors of the parent.
    dup2(orig, STDIN_FILENO);
    dup2(orig_o, STDOUT_FILENO);
}   
