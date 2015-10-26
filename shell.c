#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_BUFFER_MAX_SIZE (1024 * sizeof(char))
#define MAX_COMMAND_ARGUMENT_COUNT 255
#define BUFFER_INPUT_FILE stdin

#define LAST_CHAR(input) input[strlen(input)-1]

int main() {

    char*  input;
    char*  input_copy;
    char*  token;
    char*  command[MAX_COMMAND_ARGUMENT_COUNT + 1]; /* +1 to hold the final NULL */
    int    status;
    int    token_counter;
    pid_t  child_pid;

    do {

        /* Print prompt */
        printf("> ");

        /* Read input */
        input = (char*)malloc(INPUT_BUFFER_MAX_SIZE);
        fgets(input, INPUT_BUFFER_MAX_SIZE, BUFFER_INPUT_FILE);
        input_copy = (char*)malloc(sizeof(input));
        strcpy(input_copy, input);
        if (LAST_CHAR(input_copy) == '\n')
            LAST_CHAR(input_copy) = '\0';
        for (token_counter = 0; (token = strsep(&input_copy, " ")); token_counter++) {
            command[token_counter] = token;
        }
        command[token_counter] = NULL;

        /* Exit if EOF or "exit" command */
        if (!*input || !strcmp(command[0], "exit")) exit(0);

        /* Handle cd */
        if (!strcmp(command[0], "cd")) {
            chdir(command[1]);
        }

        /* Execute command */
        child_pid = fork();
        if (child_pid) {
            waitpid(child_pid, &status, 0);
            if (status) {
                printf("Non-zero exit status: %d\n", status);
            }
        }
        else {
            if (execvp(command[0], command)) {
                printf("Command not found: %s\n", command[0]);
            }
            break;
        }

        /* Free allocated memory */
        free(input);
        free(input_copy);

    } while (*input);
    return 0;
}
