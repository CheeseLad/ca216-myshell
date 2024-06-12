/* 
Name: Jake Farrell
Student Number: 22XXXXXX
I acknowledge the DCU Academic Integrity Policy in this submitted work
*/

#include "../myshell.h"
#include "../commands.h" // https://stackoverflow.com/questions/21260735/how-to-invoke-function-from-external-c-file-in-c
#include "../utility.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;  // NULL terminated array of char *
char *getenv(const char *name); // get's the value of an environment variable

extern int errno;        // system error number 
void syserr(char* );     // error report and abort routine 

int main (int argc, char **argv) // starter code adapted from CA216 Lab04/05 // https://loop.dcu.ie/mod/book/view.php?id=2322719
{
    char input[MAX_BUFFER];         // line buffer
    char *args[MAX_ARGS];           // pointers to arg strings
    char **arg;                     // working pointer thru args
    char batchfile[MAX_BUFFER];     // batchfile buffer
    char *args_cleaned[MAX_ARGS];   // pointers to arg strings without redirection symbols
    char **arg_cleaned;             // working pointer thru args without redirection symbols
    char result[MAX_BUFFER];        // buffer for the result of getcwd
    char cwd[MAX_BUFFER];           // buffer for the current working directory

    if (getcwd(cwd, sizeof(cwd)) == NULL) { // https://stackoverflow.com/questions/298510/how-to-get-the-current-directory-in-a-c-program
        printf("Error getting the current working directory.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(result, cwd);
    strcat(result, "/myshell"); // get the current working directory and append the name of the shell
    setenv("SHELL", result, 1); // set the SHELL environment variable
    
    if (!argv[1]) { // if no args given (not batchmode), display welcome message to user
        welcome();
    }
    while (!feof(stdin)) {
        /* get command line from input */
        if(argv[1]) { // if file provided in the arguments, run it in batchmode
            strcpy(batchfile, argv[1]);
            batchmode(batchfile);
        }
        char arg_filter[MAX_BUFFER] = "";
        printf("%s - %s --> ", getenv("PWD"), getenv("USER"));  // creates the prompt with the current working directory and user
        if (fgets (input, MAX_BUFFER, stdin )) { // read a line
            /* tokenize the input into args array */
            arg = args;
            *arg++ = strtok(input,SEPARATORS);  // tokenize input
            while ((*arg++ = strtok(NULL,SEPARATORS))); // last entry will be NULL 

            // switches to determine what the user requests to do
            int background_execution = 0;
            int redirection_stdin = 0;
            int redirection_stdout = 0;
            int redirection_create_append = 0;
            int stdout_arg_file = 0; // stores the position of the file to redirect stdout to
            int stdin_arg_file = 0; // stores the position of the file to redirect stdin to
            int stop = 0; // flag to stop adding arguments to the arg_filter when redirection symbols are found

            int arg_count = 0;
            for (int i = 0; args[i] != NULL; i++) { // loop through the args to find redirection symbols
                if(!strcmp(args[i], "<") && args[i + 1]) {
                    redirection_stdin = 1; // update flags accordingly
                    stdin_arg_file = i + 1;
                    stop = 1; // stop adding arguments to the arg_filter (we don't want to include the redirection symbol in the command)
                }
                if(!strcmp(args[i], ">") && args[i + 1]) {
                    redirection_stdout = 1;
                    stdout_arg_file = i + 1;
                    stop = 1;
                }
                if(!strcmp(args[i], ">>") && args[i + 1]) {
                    redirection_stdout = 1;
                    redirection_create_append = 1;
                    stdout_arg_file = i + 1;
                    stop = 1;
                }
                if (stop == 0) {
                    arg_count++; // count the number of arguments before redirection symbols are found
                }
            }
            for (int i = 0; i < arg_count; i++) {
                strcat(arg_filter, args[i]);
            }
            arg_cleaned = args_cleaned;
            *arg_cleaned++ = strtok(arg_filter,SEPARATORS); // tokenize the filtered args
            while ((*arg_cleaned++ = strtok(NULL,SEPARATORS))); // last entry will be NULL


            if (arg_count > 1) {
                if (!strcmp(args[arg_count - 1],"&")) { // if the last argument is &, the user wants to run the command in the background
                    background_execution = 1;
                } 
            }
            if (args[0] && background_execution == 1) {
                background_execute(args);
            }
            if (args[0] && background_execution == 0 && redirection_stdout == 0 && redirection_stdin == 0) {
                int status = command(args);
                if (status == 0) { // if the command is not an internal command, fork and execute
                    fork_exec(args, result);
                }
            } 
            // execute a different function depending on the user's redirection request
            if (args[0] && background_execution == 0 && redirection_stdout == 1 && redirection_stdin == 0) {
                process_stdout(args, redirection_create_append, stdout_arg_file, args_cleaned);
            }
            if (args[0] && background_execution == 0 && redirection_stdout == 0 && redirection_stdin == 1) {
                process_stdin(args, stdin_arg_file, args_cleaned);
            }    
            if (args[0] && background_execution == 0 && redirection_stdout == 1 && redirection_stdin == 1) {
                process_stdin_stdout(args, redirection_create_append, stdin_arg_file, stdout_arg_file, args_cleaned);
            }          
        }
    }
}