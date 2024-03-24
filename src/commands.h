/* 
Name: Jake Farrell
Student Number: 22349856
I acknowledge the DCU Academic Integrity Policy in this submitted work
*/
#include "./myshell.h"

int command(char *args[MAX_ARGS]);
int command_clear();
int command_pause();
int command_environ();
int command_quit();
int command_echo(char *args[MAX_ARGS]);
int command_help();
int command_dir(char *args[MAX_ARGS]);
int command_cd(char *args[MAX_ARGS]);