/* 
Name: Jake Farrell
Student Number: 22349856
I acknowledge the DCU Academic Integrity Policy in this submitted work
*/
#include "../myshell.h"
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/wait.h>

int batchmode(char batchfile[MAX_BUFFER]) {
  char batchfile_content[MAX_BUFFER];
  FILE *pfile = NULL;
  pfile = fopen(batchfile, "r");
  if (pfile == NULL) {
    printf("Error opening file, pleas try again.\n");
    return -1;
  } // https://stackoverflow.com/questions/25823332/c-programming-read-file-and-store-it-as-a-string
  while (fgets(batchfile_content, sizeof(batchfile_content), pfile) != NULL) { }
  system(batchfile_content);
  fclose(pfile);
  exit(0);
}

void welcome() {
  printf("______________________________________________________________________________________\n");
  printf("|                                                                                    |\n");
  printf("|                               Welcome to MyShell                                   |\n");
  printf("|                     Creator: Jake Farrell - 22349856                               |\n");
  printf("|                  For help, please run the 'help' command                           |\n");
  printf("|                                                                                    |\n");
  printf("______________________________________________________________________________________\n");
}

int fork_exec(char **args) {
  pid_t pid = fork();

	            if (pid < 0) {
		            fprintf(stderr, "Fork Failed\n");
		            return 1;
	            }
	            else if (pid == 0) {
		            //int status = command(args);
                    //if (status == 0) {
                        int execvp_status_code = execvp(args[0], args);
                        if (execvp_status_code == -1) { // https://www.digitalocean.com/community/tutorials/execvp-function-c-plus-plus
                            printf("Terminated Incorrectly\n");
                            return 1;
                        }
                    //}
	            } else {
                wait(&pid); // Wait for the child
                //printf("Child  code is");
                return 0;
              }
                                      //waitpid(pid, NULL, 0);
                        //printf("Resuming main()...");
              
  }

int background_execute(char **args) {
  pid_t pid = fork();

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
	            } 
  }