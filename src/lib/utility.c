/* 
Name: Jake Farrell
Student Number: 22349856
I acknowledge the DCU Academic Integrity Policy in this submitted work
*/
#include "../myshell.h"
#include "../utility.h"
#include "../commands.h" // https://stackoverflow.com/questions/21260735/how-to-invoke-function-from-external-c-file-in-c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/wait.h>

void batchmode(char batchfile[MAX_BUFFER]) {
  FILE* pBatchfile;
  char reading[MAX_BUFFER];
  char input_before_batchmode[MAX_BUFFER];
  char * args_batchmode[MAX_ARGS];                     // pointers to arg strings
  char ** arg_batchmode;      
  pBatchfile = fopen(batchfile, "r");

  if(fgets(reading, MAX_BUFFER, pBatchfile)) { // https://stackoverflow.com/questions/3501338/c-read-file-line-by-line
    arg_batchmode = args_batchmode;
    strcpy(input_before_batchmode, reading);
    *arg_batchmode++ = strtok(reading,SEPARATORS);   // tokenize input
    while ((*arg_batchmode++ = strtok(NULL,SEPARATORS)));

    if (args_batchmode[0]) {
      args_batchmode[strcspn(*args_batchmode, "\n")] = '\0'; // make newline charcter at the end of a line into a null character
      int status = command(args_batchmode);
        if (status == 0) {
          system(input_before_batchmode);
        }
    }
  }
  fclose(pBatchfile);
  exit(0);
    
    /*strcpy(input_before2, reading);
    //system(input_before2);
    //execvp(input_before2, input_before2);
    arg = args;
    *arg++ = strtok(input_before2,SEPARATORS);   // tokenize input
    while ((*arg++ = strtok(NULL,SEPARATORS)));


    if (args[0]) {
    int status = command(args);
                    if (status == 0) {
                        int execvp_status_code = execvp(args[0], args);
                        if (execvp_status_code == -1) { // https://www.digitalocean.com/community/tutorials/execvp-function-c-plus-plus
                            printf("Terminated Incorrectly\n");
                        }
                    }
    }
  }
  fclose(pBatchfile);
  exit(0); */
}
      /*char * args[MAX_ARGS];                     // pointers to arg strings
    char ** arg;                               // working pointer thru args
  char batchfile_content[MAX_BUFFER];
  char input_before[MAX_BUFFER];
  FILE *pfile = NULL;
  pfile = fopen(batchfile, "r");
  if (pfile == NULL) {
    printf("Error opening file, pleas try again.\n");
    return -1;
  } // https://stackoverflow.com/questions/25823332/c-programming-read-file-and-store-it-as-a-string
  while (fgets(batchfile_content, sizeof(batchfile_content), pfile) != NULL) { }
    fclose(pfile);
  //system(batchfile_content);
              arg = args;
              strcpy(input_before, batchfile_content);
            *arg++ = strtok(batchfile_content,SEPARATORS);   // tokenize input
            while ((*arg++ = strtok(NULL,SEPARATORS))) {
              if (args[0]) {

             
            
                    int status = command(args);
                    //fclose(stdin_pointer);
                    //fclose(stdout_pointer);
                    if (status == 0) {
                        system(input_before);
                        //fclose(stdout_pointer);
                        //execlp(args[0], input_before);
                    }
                   }
            }

  exit(0);
}*/

void welcome() {
  printf("______________________________________________________________________________________\n");
  printf("|                                                                                    |\n");
  printf("|                               Welcome to MyShell                                   |\n");
  printf("|                     Creator: Jake Farrell - 22349856                               |\n");
  printf("|                  For help, please run the 'help' command                           |\n");
  printf("|                                                                                    |\n");
  printf("______________________________________________________________________________________\n");
}

int fork_exec(char **args, char result[MAX_BUFFER]) {
  pid_t pid = fork();

	            if (pid < 0) {
		            fprintf(stderr, "Fork Failed\n");
		            return 1;
	            }
	            else if (pid == 0) {
		            //int status = command(args);
                    //if (status == 0) {
                        //char result_parent[MAX_BUFFER]; // https://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from
                        //readlink("/proc/self/exe", result_parent, MAX_BUFFER);
                        //setenv("PARENT", result, 1);
                        //printf("%s", getenv("PARENT"));
                        int execvp_status_code = execvp(args[0], args);
                        if (execvp_status_code == -1) { // https://www.digitalocean.com/community/tutorials/execvp-function-c-plus-plus
                            printf("External command did not run successfully, maybe `%s` was a typo?\n", args[0]);
                            return 1;
                        }
                        return 1;
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
                            printf("External command did not run successfully, maybe `%s` was a typo?\n", args[0]);
                            return 1;
                        }
                    }
                    return 1;
	            } 
            return 1;
  }

void process_stdout(char **args, int redirection_create_append, int stdout_arg_file, char **args2) { // https://www.tutorialspoint.com/c_standard_library/c_function_freopen.htm
  int toggle = 0;
  pid_t pid = fork();

	            if (pid < 0) {
		            fprintf(stderr, "Fork Failed\n");
	            }
	            else if (pid == 0) {
		             FILE *stdout_pointer;
                if (redirection_create_append == 0) {
                    stdout_pointer = freopen(args[stdout_arg_file], "w", stdout);
                    if (stdout_pointer == NULL) { // https://stackoverflow.com/questions/14680232/how-to-detect-a-file-is-opened-or-not-in-c
                        printf("Output file was not found.\n");
                    }
                } else {
                    stdout_pointer = freopen(args[stdout_arg_file], "a", stdout);
                    if (stdout_pointer == NULL) {
                        printf("Output file was not found.\n");
                }
                }
                int status = command(args);
                if (status == 0) {
                    toggle = 1;
                    int execvp_status_code = execvp(args[0], args2); // https://www.digitalocean.com/community/tutorials/execvp-function-c-plus-plus
                        if (execvp_status_code == -1) {
                            printf("External command did not run successfully, maybe `%s` was a typo?\n", args[0]);
                        }
                }
                fclose(stdout_pointer);
	            } else {
                if (toggle == 1) {
                  wait(&pid); // Wait for the child
                }
                
              }
 
}

void process_stdin(char **args, int stdin_arg_file, char **args2) { // https://www.tutorialspoint.com/c_standard_library/c_function_freopen.htm
  pid_t pid = fork();

              if (pid < 0) {
                fprintf(stderr, "Fork Failed\n");
              }
              else if (pid == 0) {
                FILE *stdin_pointer;
                stdin_pointer = freopen(args[stdin_arg_file], "r", stdin);
                if (stdin_pointer == NULL) {
                    printf("Input file was not found.\n");
                }
                    
                int status = command(args2);
                if (status == 0) {
                        int execvp_status_code = execvp(args[0], args2); // https://www.digitalocean.com/community/tutorials/execvp-function-c-plus-plus
                        if (execvp_status_code == -1) {
                            printf("External command did not run successfully, maybe `%s` was a typo?\n", args[0]);
                        }
                }
                fclose(stdin_pointer);
              } 
}

void process_stdin_stdout(char **args, int redirection_create_append, int stdin_arg_file, int stdout_arg_file, char **args2) { // https://www.tutorialspoint.com/c_standard_library/c_function_freopen.htm
FILE *stdin_pointer;
                FILE *stdout_pointer;
                stdin_pointer = freopen(args[stdin_arg_file], "r", stdin);
                if (stdin_pointer == NULL) {
                    printf("Input file was not found.\n");
                }
                if (redirection_create_append == 0) {
                    stdout_pointer = freopen(args[stdout_arg_file], "w", stdout);
                    if (stdout_pointer == NULL) {
                        printf("Output file was not found.\n");
                    }
                } else {
                    stdout_pointer = freopen(args[stdout_arg_file], "a", stdout);
                    if (stdout_pointer == NULL) {
                        printf("Output file was not found.\n");
                    }
                }
                    
                int status = command(args2);
                if (status == 0) {
                    execvp(args[0], args2); // https://www.digitalocean.com/community/tutorials/execvp-function-c-plus-plus
                }
                fclose(stdin_pointer);
                fclose(stdout_pointer);
}