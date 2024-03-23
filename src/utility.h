/* 
Name: Jake Farrell
Student Number: 22349856
I acknowledge the DCU Academic Integrity Policy in this submitted work
*/
#include "./myshell.h"

#define MAX_BUFFER 1024

int batchmode(char batchfile[MAX_BUFFER]);
void welcome();
int fork_exec(char **args, char result[MAX_BUFFER]);
int background_execute(char **args);