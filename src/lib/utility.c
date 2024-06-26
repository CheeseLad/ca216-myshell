/* 
Name: Jake Farrell
Student Number: 22XXXXXX
I acknowledge the DCU Academic Integrity Policy in this submitted work
*/

#include "../myshell.h"
#include "../utility.h"
#include "../commands.h" // https://stackoverflow.com/questions/21260735/how-to-invoke-function-from-external-c-file-in-c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>

void batchmode(char batchfile[MAX_BUFFER]) {
  FILE* pBatchfile;
  char reading[MAX_BUFFER];
  char input_before_batchmode[MAX_BUFFER];
  char * args_batchmode[MAX_ARGS];                     // pointers to arg strings
  char ** arg_batchmode;      
  pBatchfile = fopen(batchfile, "r");
  if (pBatchfile == NULL) { // https://www.tutorialspoint.com/cprogramming/c_error_handling.htm
    printf("Error opening the batch file.\n");
    exit(EXIT_FAILURE);
  }

  while(fgets(reading, MAX_BUFFER, pBatchfile)) { // https://stackoverflow.com/questions/3501338/c-read-file-line-by-line
    arg_batchmode = args_batchmode;
    strcpy(input_before_batchmode, reading);
    reading[strcspn(reading, "\n")] = '\0'; // make newline charcter at the end of a line into a null character // https://stackoverflow.com/questions/9628637/how-can-i-get-rid-of-n-from-string-in-c

    *arg_batchmode++ = strtok(reading,SEPARATORS);   // same process for getting args as in myshell.c
    while ((*arg_batchmode++ = strtok(NULL,SEPARATORS)));

    if (args_batchmode[0]) {
     
      int status = command(args_batchmode);
        if (status == 0) {
          system(input_before_batchmode);
        }
    }
  }
  fclose(pBatchfile);
  exit(0);
}    

void welcome() {
  printf("______________________________________________________________________________________\n");
  printf("|                                                                                    |\n");
  printf("|                               Welcome to MyShell                                   |\n");
  printf("|                     Creator: Jake Farrell - 22XXXXXX                               |\n");
  printf("|                  For help, please run the 'help' command                           |\n");
  printf("|                                                                                    |\n");
  printf("______________________________________________________________________________________\n");
}

void fork_exec(char **args, char result[MAX_BUFFER]) {
  pid_t pid = fork(); // create a child process
  if (pid < 0) {
		fprintf(stderr, "Fork Failed\n");
	}
	else if (pid == 0) {
		setenv("PARENT", result, 1); // set the parent environment variable as described in the assignment
    int execvp_status_code = execvp(args[0], args);
    if (execvp_status_code == -1) { // https://www.digitalocean.com/community/tutorials/execvp-function-c-plus-plus
      printf("External command did not run successfully, maybe `%s` was a typo?\n", args[0]);
      exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
	} 
  else {
    wait(&pid); // Wait for the child to finish before resuming the parent
  }      
}

int background_execute(char **args) {
  pid_t pid = fork();
  if (pid < 0) {
		fprintf(stderr, "Fork Failed\n"); // if fork fails, print error message and return
		return 1;
	}
	else if (pid == 0) {
		int status = command(args); // check if the command is an internal command, if not, run it as external
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
  int toggle = 0; // will be used to see if need to wait for child process
  pid_t pid = fork(); // try to create fork, print error if fork unsuccessful
	if (pid < 0) {
    fprintf(stderr, "Fork Failed\n");
	}
	else if (pid == 0) {
		FILE *stdout_pointer;
    if (redirection_create_append == 0) {
      stdout_pointer = freopen(args[stdout_arg_file], "w", stdout); // open the file provided to write to it
    }
    else {
      stdout_pointer = freopen(args[stdout_arg_file], "a", stdout); // open the file provided to append to it
    }
    if (stdout_pointer == NULL) { // https://stackoverflow.com/questions/14680232/how-to-detect-a-file-is-opened-or-not-in-c
      printf("Output file was not found.\n");
      toggle = 1;
      exit(EXIT_FAILURE);
    }
    int status = command(args2); // args2 is the args without the redirection symbols, where as args is the full args array
    if (status == 0) {
      toggle = 1;
      int execvp_status_code = execvp(args[0], args2); // https://www.digitalocean.com/community/tutorials/execvp-function-c-plus-plus
      if (execvp_status_code == -1) {
        printf("External command did not run successfully, maybe `%s` was a typo?\n", args[0]);
        exit(EXIT_FAILURE);
      }
    }
    fclose(stdout_pointer); // close the file
	} 
  else {
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
    stdin_pointer = freopen(args[stdin_arg_file], "r", stdin); // open the file and use the contents to replace stdin
    if (stdin_pointer == NULL) {
      printf("Input file was not found, please check that `%s` was the correct file name.\n", args[stdin_arg_file]);
      exit(EXIT_FAILURE);
    }
    int status = command(args2);
    if (status == 0) {
      int execvp_status_code = execvp(args[0], args2); // https://www.digitalocean.com/community/tutorials/execvp-function-c-plus-plus
      if (execvp_status_code == -1) {
        printf("External command did not run successfully, maybe `%s` was a typo?\n", args[0]);
        exit(EXIT_FAILURE);
      }
    }
    fclose(stdin_pointer);
  } 
  else {
    wait(&pid); // Wait for the child
  }
}

void process_stdin_stdout(char **args, int redirection_create_append, int stdin_arg_file, int stdout_arg_file, char **args2) { // https://www.tutorialspoint.com/c_standard_library/c_function_freopen.htm
  pid_t pid = fork(); // try to create fork, print error if fork unsuccessful
  if (pid < 0) {
    fprintf(stderr, "Fork Failed\n");
  }
  else if (pid == 0) {
    FILE *stdin_pointer; // create pointers to store the file data
    FILE *stdout_pointer;
    stdin_pointer = freopen(args[stdin_arg_file], "r", stdin); // open the file and use the contents to replace stdin
    if (stdin_pointer == NULL) {
      printf("Input file was not found.\n");
      exit(EXIT_FAILURE);
    }
    if (redirection_create_append == 0) {
      stdout_pointer = freopen(args[stdout_arg_file], "w", stdout); // open the file provided to write to it
    }
    else {
      stdout_pointer = freopen(args[stdout_arg_file], "a", stdout); // open the file provided to append to it
    }
    if (stdout_pointer == NULL) {
      printf("Output file was not found.\n");
      exit(EXIT_FAILURE);
    }
    int status = command(args2); // args2 is the args without the redirection symbols, where as args is the full args array
    if (status == 0) {
      int execvp_status_code = execvp(args[0], args2); // https://www.digitalocean.com/community/tutorials/execvp-function-c-plus-plus
      if (execvp_status_code == -1) {
        printf("External command did not run successfully, maybe `%s` was a typo?\n", args[0]);
        exit(EXIT_FAILURE);
      }
    }
    fclose(stdin_pointer); // remember to close the files after using them
    fclose(stdout_pointer);
    } 
  else {
    wait(&pid); // Wait for the child
  }
}