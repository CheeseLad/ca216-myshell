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

/*void syserr(char * msg)   // report error code and abort
{
   fprintf(stderr,"%s: %s", strerror(errno), msg);
   abort(errno);
}*/

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
            batchmode(batchfile);
        }
        
        welcome();
        printf("%s --> ", getenv("PWD"));                // write prompt
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


           int arg_count = 0;
           for (int i = 0; args[i] != NULL; i++) {
            arg_count++;
           }
           if (arg_count > 1) {
            if (!strcmp(args[arg_count - 1],"&")) {
                //printf("BACKGROUND EXECUTE\n");
                background_execution = 1;
                } 
           }


            /* if (args[3]) {
                if (!strcmp(args[3],"<")) {
                    redirection_stdin = 1;
                }
            }

            if (!strcmp(args[5],">")) {
                redirection_stdout = 1;
            }*/



            if (args[0] && background_execution == 0) {                     // if there's anything there
            
                int status = command(args);
                if (status == 0) {
                    system(input_before);
                }
            }
                        /*FILE *p;
                        int ch; // https://c-for-dummies.com/blog/?p=1418

                        p = popen("neofetch","r");
                        if( p == NULL) {
                            puts("Unable to open process");
                            return(1);
                        }
                        while( (ch=fgetc(p)) != EOF)
                            putchar(ch);
                            pclose(p); */
                    //FILE *pOutput = NULL;
                    // system(input_before);
                    //pOutput = popen(input_before, "r");
    //char *filename = "myname.txt";
    //if(!pOutput)  // Open myfile.txt to write it
      //  printf("Failed to open %s.\n", filename);

    //fprintf(pOutput,"%s", filename);
    //fclose(pOutput);
	//return 0;
                    //printf("%s", pOutput);
                    //int c;
                    //if (pOutput) { // https://stackoverflow.com/questions/3463426/in-c-how-should-i-read-a-text-file-and-print-all-strings
                    //    while ((c = getc(pOutput)) != EOF)
                    //        putchar(c);
                    //    fclose(pOutput);
                     //   }
              //  }
            //} 

            if (args[0] && background_execution == 1) {
                pid_t pid; // source lecture slides

	            int status;
	            pid_t cpid;

	            pid = fork();

	            if (pid < 0) {
		            fprintf(stderr, "Fork Failed\n");
		            return 1;
	            }
	            else if (pid == 0) {
		            int status = command(args);
                    if (status == 0) {
                        system(input_before);
                    }
	            }
            }
        }
    }
    return 0; 
}