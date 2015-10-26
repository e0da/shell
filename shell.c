#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_MAX (sizeof(char) * 1024)
#define QUIT_COMMAND "exit\n"
#define NON_ZERO_EXIT_STATUS_MESSAGE \
    "The command returned a non-zero exit status: %d\n"

int should_continue(char* input) {
    return (*input && strcmp(input, QUIT_COMMAND) != 0);
}

void print_prompt() {
    printf("> ");
}

/* TODO This needs to be changed to actually parse a bunch of tokens using
 * spaces as a delimeter and it needs to eliminate the \n */
char* read_input() {
    char* input = (char*)malloc(BUFFER_MAX);
    fgets(input, BUFFER_MAX, stdin);
    return input;
}

void execute_command(char* command, int* status) {
    int child_pid;
    char* args[] = {"/"};
    child_pid = fork();
    if (child_pid) {
        waitpid(child_pid, status, 0);
    }
    else {
        execve("/bin/ls", args, 0);
        exit(0);
    }
}

void report_non_zero_exit_status(int status) {
    printf(NON_ZERO_EXIT_STATUS_MESSAGE, status);
}

void read_execute_loop() {
    char* input;
    int status;
    do {
        print_prompt();
        input = read_input();
        execute_command(input, &status);
        if (status) {
            report_non_zero_exit_status(status);
        }
    } while ( should_continue(input) );
}

int main() {
    read_execute_loop();
    return 0;
}
