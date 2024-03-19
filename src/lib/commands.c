// commands.c
#include "../myshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/wait.h>

extern char **environ;

int command_clear() {
    printf("\033[H"); // https://stackoverflow.com/questions/37774983/clearing-the-screen-by-printing-a-character
    printf("\033[2J");
    return 1;
}

int command_pause() {
    printf("-----------------------------------------------------");
    printf("\nMyShell is paused, press the ENTER key to continue... ");  // https://stackoverflow.com/questions/18801483/press-any-key-to-continue-function-in-c
    getchar();  
    return 1;
}

int command(char * args[MAX_ARGS]) {
    if (!strcmp(args[0],"clear") || !strcmp(args[0],"clr")) { // "clear"/"clr" command
        return command_clear();
    } else if (!strcmp(args[0],"dir") || !strcmp(args[0],"ls")) { // "dir" command
                    //system("ls -al");
                    // https://stackoverflow.com/questions/12489/how-do-you-get-a-directory-listing-in-c
                    DIR *dp;
                    struct dirent *ep;     
                    dp = opendir ("./");
                    if (dp != NULL) {
                        while ((ep = readdir (dp)) != NULL)
                            //printf("%s", ep);
          
                        closedir(dp);
                        return 0;
                    }
                    else {
                        perror ("Couldn't open the directory");
                        return -1;
                    }
                }

                else if (!strcmp(args[0],"cd")) { // "dir" command
                    if (args[1] == NULL) {
                        // https://stackoverflow.com/questions/17695413/store-output-of-systemfile-command-as-a-string-in-c
                        FILE *dir_file_blank;
                        char blank_pwd[MAX_BUFFER];
                        dir_file_blank = popen("pwd", "r");
                        if (dir_file_blank == NULL) {
                            printf("Failed to run command\n" );
                            return 1;
                        }

                        while (fgets(blank_pwd, sizeof(blank_pwd), dir_file_blank) != NULL) {
                            printf("%s", blank_pwd);
  }
                    } else if (args[1]) {
                        int dir_status = chdir(args[1]);
                        if(dir_status == -1) {
                            printf("Directory not found.\n");
                        }
                        if (dir_status == 0) {
                            char newcd[MAX_BUFFER];
                            strcpy(newcd, "PWD="); // https://stackoverflow.com/questions/17695413/store-output-of-systemfile-command-as-a-string-in-c
                            FILE *dir_file_pointer;
                            char new_pwd[MAX_BUFFER];
                            dir_file_pointer = popen("pwd", "r");
  if (dir_file_pointer == NULL) {
      printf("Failed to run command\n" );
      return 1;
  }

  while (fgets(new_pwd, sizeof(new_pwd), dir_file_pointer) != NULL) {
      //printf("%s", new_pwd);
      strcat(newcd, new_pwd);
  }

                            //strcat(newcd, system("pwd"));
                            //printf("%s", newcd);
                            putenv(newcd);
                            //setenv(newcd);
                        }
                         

                        //if (!strcmp(args[1],"..")) {
                            //system("cd ..");
                            //chdir("..");
                            //system("pwd");
                            //printf("YES");
                            //printf("%s", chdir(".."));
                        //}
                    }
                    //chdir();
                    //char test = getcwd();
                    //printf("%s", getenv("PWD"));
                    //setenv();
                    return 1;
                }

                else if (!strcmp(args[0],"environ")) { // "environ" command
                    for (int i = 0; environ[i] != NULL; i++)
                        printf("%s\n",environ[i]);
                    return 1;
                }

                else if (!strcmp(args[0],"echo")) { // "dir" command
                    for (int i = 1; args[i] != NULL; i++) {
                        printf("%s ", args[i]);
                    }
                    printf("\n");
                    return 1;
                }

                else if (!strcmp(args[0], "help")) {
                    FILE *pfile = NULL;
                    pfile = fopen("./manual/readme", "r");
                    int c;
                    if (pfile) { // https://stackoverflow.com/questions/3463426/in-c-how-should-i-read-a-text-file-and-print-all-strings
                        while ((c = getc(pfile)) != EOF)
                            putchar(c);
                        fclose(pfile);
                        }
                    printf("\n----------------------------------");
                    printf("\nPress the ENTER key to continue... ");  // https://stackoverflow.com/questions/18801483/press-any-key-to-continue-function-in-c
                    getchar();   
                    return 1;
                }

                
                else if (!strcmp(args[0], "pause")) {
                    return command_pause();
                }

                else if (!strcmp(args[0],"quit")|| !strcmp(args[0],"q")) {  // "quit" command
                    printf("----------------------");
                    printf("\nMyShell is quitting...\n");
                    exit(0);                    // break out of 'while' loop
                }

                else {
                    return 0;
                }
}