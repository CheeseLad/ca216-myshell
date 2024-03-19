#include "./myshell.h"

#define MAX_BUFFER 1024

int batchmode(char batchfile[MAX_BUFFER]);
void welcome();
int fork_exec(char **args);
int background_execute(char **args);