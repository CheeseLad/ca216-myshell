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

//FILE *popen(const char *command, const char *mode); // https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-popen-initiate-pipe-stream-from-process

extern int errno;        // system error number 
void syserr(char* );     // error report and abort routine 

int main (int argc, char ** argv)
{
    char input[MAX_BUFFER];                      // line buffer
    char input_before[MAX_BUFFER];
    char * args[MAX_ARGS];                     // pointers to arg strings
    char ** arg;                               // working pointer thru args
    char batchfile[MAX_BUFFER];
    /*char cwd[MAX_BUFFER]; // https://stackoverflow.com/questions/298510/how-to-get-the-current-directory-in-a-c-program
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("Current working dir: %s\n", cwd);
    } else {
       perror("getcwd() error");
       return 1;
    } */   

    char result[MAX_BUFFER]; // https://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from
    readlink("/proc/self/exe", result, MAX_BUFFER);
    //char * prompt = strcat(getenv("PWD")," --> ");                    // shell prompt
    //setenv("SHELL", strcat(getenv("PWD"), "/myshell"), 1);
    setenv("SHELL", result, 1);

/* keep reading input until "quit" command or eof of redirected input */
    
    
    if (!argv[1]) {
        welcome();
    }
    while (!feof(stdin)) {

/* get command line from input */
        if(argv[1]) {
            strcpy(batchfile, argv[1]);
            batchmode(batchfile);
        }
        
        
        printf("%s - %s --> ", getenv("PWD"), getenv("USER"));                // write prompt
        //if (fgets(input, MAX_BUFFER, stdin) == NULL) continue;
        if (fgets (input, MAX_BUFFER, stdin )) { // read a line

/* tokenize the input into args array */

            arg = args;
            strcpy(input_before, input);
            *arg++ = strtok(input,SEPARATORS);   // tokenize input
            while ((*arg++ = strtok(NULL,SEPARATORS)));
                                               // last entry will be NULL 
    
            int background_execution = 0;
            int redirection_stdin = 0;
            int redirection_stdout = 0;
            int redirection_create_append = 0;
            int stdout_arg_file = 0;
            int stdin_arg_file = 0;

            int arg_count = 0;
            for (int i = 0; args[i] != NULL; i++) {
                arg_count++;
                if(!strcmp(args[i], "<")) {
                    redirection_stdin = 1;
                    stdin_arg_file = i - 1;
                }
                if(!strcmp(args[i], ">") && args[i + 1]) {
                    redirection_stdout = 1;
                    stdout_arg_file = i + 1;
                }
                if(!strcmp(args[i], ">>")) {
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

            if (args[0] && background_execution == 0) {                     // if there's anything there
                if (redirection_stdout == 0) {
                    int status = command(args);
                    if (status == 0) {
                        //system(input_before);
                        //char *program = args[0];
                        //memmove(&args[0], &args[1], (MAX_BUFFER - 1) * sizeof(args[0])); // https://stackoverflow.com/questions/42652425/how-to-remove-the-first-element-of-an-array-and-add-a-new-element-to-the-end
                        //printf("%s\n", program);
                        //execlp(args[0], input_before);
                        //execvp(args[0], args);
                        //syserr("execvp"); // error if return from exec
                        fork_exec(args);
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
                
            }                

            if (args[0] && background_execution == 1) {

                background_execute(args);

	            /*pid_t pid = fork();

	            if (pid < 0) {
		            fprintf(stderr, "Fork Failed\n");
		            return 1;
	            }
	            else if (pid == 0) {
		            int status = command(args);
                    if (status == 0) {
                        int execvp_status_code = execvp(args[0], args);
                        if (execvp_status_code == -1) { // https://www.digitalocean.com/community/tutorials/execvp-function-c-plus-plus
                            printf("Terminated Incorrectly\n");
                            return 1;
                        }
                    }
	            } */
            }
        }
    }
    return 0; 
}