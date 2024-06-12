/* 
Name: Jake Farrell
Student Number: 22XXXXXX
I acknowledge the DCU Academic Integrity Policy in this submitted work
*/
#include "./myshell.h"

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token separators

void batchmode(char batchfile[MAX_BUFFER]);
void welcome();
void fork_exec(char **args, char result[MAX_BUFFER]);
int background_execute(char **args);
void process_stdout(char **args, int redirection_create_append, int stdout_arg_file, char **args2);
void process_stdin(char **args, int stdin_arg_file, char **args2);
void process_stdin_stdout(char **args, int redirection_create_append, int stdin_arg_file, int stdout_arg_file, char **args2);