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

int main (int argc, char ** argv)
{
    char input[MAX_BUFFER];                      // line buffer
    char input_before[MAX_BUFFER];
    char * args[MAX_ARGS];                     // pointers to arg strings
    char ** arg;                               // working pointer thru args
    char batchfile[MAX_BUFFER];

    char result[MAX_BUFFER]; // https://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from
    readlink("/proc/self/exe", result, MAX_BUFFER);
    setenv("SHELL", result, 1);
    
    if (!argv[1]) { // if no args given (not batchmode), display welcome message to user
        welcome();
    }
    while (!feof(stdin)) {

/* get command line from input */
        if(argv[1]) {
            strcpy(batchfile, argv[1]);
            batchmode(batchfile);
        }
        
        
        printf("%s - %s --> ", getenv("PWD"), getenv("USER"));  
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

            int arg_count = 0;
            for (int i = 0; args[i] != NULL; i++) {
                arg_count++;
                if(!strcmp(args[i], "<") && args[i + 1]) {
                    redirection_stdin = 1;
                    stdin_arg_file = i + 1;
                }
                if(!strcmp(args[i], ">") && args[i + 1]) {
                    redirection_stdout = 1;
                    stdout_arg_file = i + 1;
                }
                if(!strcmp(args[i], ">>") && args[i + 1]) {
                    redirection_stdout = 1;
                    redirection_create_append = 1;
                    stdout_arg_file = i + 1;
                }
            }
            if (arg_count > 1) {
                if (!strcmp(args[arg_count - 1],"&")) {
                    background_execution = 1;
                } 
            }
            if (args[0] && background_execution == 1) {
                background_execute(args);
            }

            if (args[0] && background_execution == 0) {                     // if there's anything there
                if (redirection_stdout == 0 && redirection_stdin == 0) {
                    int status = command(args);
                    if (status == 0) {
                        fork_exec(args, result);
                    }
                } 
                else if (redirection_stdout == 1 && redirection_stdin == 0) { // https://www.tutorialspoint.com/c_standard_library/c_function_freopen.htm
                    FILE *stdout_pointer;
                    stdout_pointer = freopen(args[stdout_arg_file], "w", stdout);
                    int status = command(args);
                    if (status == 0) {

                        int execvp_status_code = execvp(args[0], args);
                        if (execvp_status_code == -1) { // https://www.digitalocean.com/community/tutorials/execvp-function-c-plus-plus
                            printf("Terminated Incorrectly\n");
                            return 1;
                        }
                        //system(input_before);
                        
                        //execlp(args[0], input_before);
                    }
                    fclose(stdout_pointer);
                    break;
                    
                } else if (redirection_stdout == 1 && redirection_stdin == 1) { // https://www.tutorialspoint.com/c_standard_library/c_function_freopen.htm
                    FILE *stdin_pointer;
                    FILE *stdout_pointer;
                    stdin_pointer = freopen(args[stdin_arg_file], "r", stdin);
                    if (redirection_create_append == 0) {
                        stdout_pointer = freopen(args[stdout_arg_file], "w", stdout);
                    } else {
                        stdout_pointer = freopen(args[stdout_arg_file], "a", stdout);
                    }
                    
                    int status = command(args);
                    fclose(stdin_pointer);
                    fclose(stdout_pointer);
                    if (status == 0) {
                        system(input_before);
                        //fclose(stdout_pointer);
                        //execlp(args[0], input_before);
                    }
                }
                else if (redirection_stdout == 0 && redirection_stdin == 1) { // https://www.tutorialspoint.com/c_standard_library/c_function_freopen.htm
                    FILE *stdin_pointer;
                    stdin_pointer = freopen(args[stdin_arg_file], "r", stdin);
                    
                    int status = command(args);
                    if (status == 0) {
                        system(input_before);
                        //fclose(stdout_pointer);
                        //execlp(args[0], input_before);
                    }
                    fclose(stdin_pointer);
                    continue;
                }
            }                
        }
    }
    return 0; 
}