/* 
Name: Jake Farrell
Student Number: 22349856
I acknowledge the DCU Academic Integrity Policy in this submitted work
*/
#include "./myshell.h"

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token sparators
#define SEPARATORS2 " \t"                     // token sparators

void batchmode(char batchfile[MAX_BUFFER]);
void welcome();
int fork_exec(char **args, char result[MAX_BUFFER]);
int background_execute(char **args);
void process_stdout(char **args, int redirection_create_append, int stdout_arg_file, char **args2);
void process_stdin(char **args, int stdin_arg_file, char **args2);