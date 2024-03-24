/* 
Name: Jake Farrell
Student Number: 22349856
I acknowledge the DCU Academic Integrity Policy in this submitted work
*/
#include "../myshell.h"
#include "../commands.h" // https://stackoverflow.com/questions/21260735/how-to-invoke-function-from-external-c-file-in-c
#include "../utility.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/wait.h>



extern char **environ;  // NULL terminated array of char *
char *getenv(const char *name);

extern int errno;        // system error number 
void syserr(char* );     // error report and abort routine 

int main (int argc, char **argv)
{
    char input[MAX_BUFFER];                      // line buffer
    char input_before[MAX_BUFFER];
    char *args[MAX_ARGS];                     // pointers to arg strings
    char **arg;                               // working pointer thru args
    char batchfile[MAX_BUFFER];
    char *args2[MAX_ARGS];                     // pointers to arg strings
    char **arg2;                               // working pointer thru args
    

    char result[MAX_BUFFER]; // https://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from
    //readlink("/proc/self/exe", result, MAX_BUFFER);
    setenv("SHELL", getenv("PWD"), 1);
    setenv("PARENT", getenv("PWD"), 1);
    
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
            strcpy(input_before, input);
            *arg++ = strtok(input,SEPARATORS);   // tokenize input
            while ((*arg++ = strtok(NULL,SEPARATORS)));
                                               // last entry will be NULL 
            // switches to determine what the user requests to do
            int background_execution = 0;
            int redirection_stdin = 0;
            int redirection_stdout = 0;
            int redirection_create_append = 0;
            int stdout_arg_file = 0;
            int stdin_arg_file = 0;
            int stop = 0;

            int arg_count = 0;
            for (int i = 0; args[i] != NULL; i++) {
                if(!strcmp(args[i], "<") && args[i + 1]) {
                    redirection_stdin = 1;
                    stdin_arg_file = i + 1;
                    stop = 1;
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
                    arg_count++;
                }
            }
            for (int i = 0; i < arg_count; i++) {
                strcat(arg_filter, args[i]);
                //printf("Arg filter: %s\n", arg_filter);
            }
            //printf("Result: %s\n", arg_filter);
            arg2 = args2;
            *arg2++ = strtok(arg_filter,SEPARATORS);   // tokenize input
            while ((*arg2++ = strtok(NULL,SEPARATORS)));


            if (arg_count > 1) {
                if (!strcmp(args[arg_count - 1],"&")) {
                    background_execution = 1;
                } 
            }
            if (args[0] && background_execution == 1) {
                background_execute(args);
            }
            if (args[0] && background_execution == 0 && redirection_stdout == 0 && redirection_stdin == 0) {
                int status = command(args);
                if (status == 0) {
                    fork_exec(args, result);
                }
            } 
            if (args[0] && background_execution == 0 && redirection_stdout == 1 && redirection_stdin == 0) { 
                process_stdout(args, redirection_create_append, stdout_arg_file, args2);
            }
            if (args[0] && background_execution == 0 && redirection_stdout == 0 && redirection_stdin == 1) { // https://www.tutorialspoint.com/c_standard_library/c_function_freopen.htm
                process_stdin(args, stdin_arg_file, args2);
            }    
            if (args[0] && background_execution == 0 && redirection_stdout == 1 && redirection_stdin == 1) { // https://www.tutorialspoint.com/c_standard_library/c_function_freopen.htm
                FILE *stdin_pointer;
                FILE *stdout_pointer;
                stdin_pointer = freopen(args[stdin_arg_file], "r", stdin);
                if (stdin_pointer == NULL) {
                    printf("Input file was not found.\n");
                    break;
                }
                if (redirection_create_append == 0) {
                    stdout_pointer = freopen(args[stdout_arg_file], "w", stdout);
                    if (stdout_pointer == NULL) {
                        printf("Output file was not found.\n");
                        break;
                    }
                } else {
                    stdout_pointer = freopen(args[stdout_arg_file], "a", stdout);
                    if (stdout_pointer == NULL) {
                        printf("Output file was not found.\n");
                        break;
                    }
                }
                    
                int status = command(args2);
                if (status == 0) {
                    execvp(args[0], args2); // https://www.digitalocean.com/community/tutorials/execvp-function-c-plus-plus
                }
                fclose(stdin_pointer);
                fclose(stdout_pointer);
            }          
        }
    }
}