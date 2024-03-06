/* 
  strtokeg - skeleton shell using strtok to parse command line

  usage:

    strtokeg

    reads in a line of keyboard input at a time, parsing it into
    tokens that are separated by white spaces (set by #define
    SEPARATORS).

    can use redirected input

    if the first token is a recognized internal command, then that
    command is executed. otherwise the tokens are printed on the
    display.

    internal commands:

        clear - clears the screen

        quit - exits from the program

 ********************************************************************
   version: 1.0
   date:    December 2003
   author:  Ian G Graham
            School of Information Technology
            Griffith University, Gold Coast
            ian.graham@griffith.edu.au

   copyright (c) Ian G Graham, 2003. All rights reserved.

   This code can be used for teaching purposes, but no warranty,
   explicit or implicit, is provided.
 *******************************************************************/

#include "../myshell.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "../commands.h" // https://stackoverflow.com/questions/21260735/how-to-invoke-function-from-external-c-file-in-c

extern char **environ;  // NULL terminated array of char *
char *getenv(const char *name);

extern int errno;        // system error number 
void syserr(char* );     // error report and abort routine 

//void syserr(char * msg)   // report error code and abort
//{
//   fprintf(stderr,"%s: %s", strerror(errno), msg);
//   abort(errno);
//}

int main (int argc, char ** argv)
{
    char input[MAX_BUFFER];                      // line buffer
    char input_before[MAX_BUFFER];
    char * args[MAX_ARGS];                     // pointers to arg strings
    char ** arg;                               // working pointer thru args
    char batchfile[MAX_BUFFER];
    //char * prompt = strcat(getenv("PWD")," --> ");                    // shell prompt
    //setenv("SHELL", strcat(getenv("PWD"), "/myshell"), 1);

/* keep reading input until "quit" command or eof of redirected input */

    while (!feof(stdin)) {

/* get command line from input */
        if(argv[1]) {
            strcpy(batchfile, argv[1]);
            //printf("%s\n", batchfile);
        }
        
        printf("%s --> ", getenv("PWD"));                // write prompt
        //if (fgets(input, MAX_BUFFER, stdin) == NULL) continue;
        if (fgets (input, MAX_BUFFER, stdin )) { // read a line

/* tokenize the input into args array */

            arg = args;
            strcpy(input_before, input);
            *arg++ = strtok(input,SEPARATORS);   // tokenize input
            while ((*arg++ = strtok(NULL,SEPARATORS)));
                                               // last entry will be NULL 
            
            /* int is_background = 0;
            for(int i = 0; i < MAX_ARGS; i++) {
                if (!strcmp(args[i],"&")) {
                    is_background = 1;
                    break;
                }

            } */

            if (args[0]) {                     // if there's anything there
            
                int status = command(args);
                if (status == 0) {
                    system(input_before);
                }
            }
        }
    }
    return 0; 
}