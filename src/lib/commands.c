// commands.c
#include "../myshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char **environ;

int command(char * args[MAX_ARGS]) {
    if (!strcmp(args[0],"clear") || !strcmp(args[0],"clr")) { // "clear"/"clr" command
                    system("clear");
                    return 1;
                }

                else if (!strcmp(args[0],"dir") || !strcmp(args[0],"ls")) { // "dir" command
                    system("ls -al");
                    return 1;
                }

                else if (!strcmp(args[0],"cd")) { // "dir" command
                    if (args[1] == NULL) {
                        //setenv(getenv("PWD"), "HOME", 1);
                        //printf("%s", getenv("PWD"));
                            chdir(getenv("HOME"));
                            //char newcd[MAX_BUFFER] = "PWD=";
                            //strcat(newcd, system("pwd"));
                            putenv("PWD=/home/jake");
                            //system("pwd");
                    } else if (args[1]) {
                        if (!strcmp(args[1],"..")) {
                            //system("cd ..");
                            chdir("..");
                            //system("pwd");
                            //printf("YES");
                            //printf("%s", chdir(".."));
                        } else {
                            chdir(args[1]);
                            //system("pwd");
                        }
                    }
                    //chdir();
                    //char test = getcwd();
                    //printf("%s", getenv("PWD"));
                    //setenv();
                    return 1;
                }

                else if (!strcmp(args[0],"pwd")) { // "dir" command
                    system("pwd");
                    return 1;
                }

                else if (!strcmp(args[0],"environ")) { // "environ" command
                    for (int i = 0; environ[i] != NULL; i++)
                        printf("%s\n",environ[i]);
                    return 1;
                }

                else if (!strcmp(args[0],"quit")|| !strcmp(args[0],"q")) {  // "quit" command
                    printf("----------------------");
                    printf("\nMyShell is quitting...\n");
                    exit(0);                    // break out of 'while' loop
                }

                else if (!strcmp(args[0],"echo")) { // "dir" command
                    if (args[1]) {
                        printf("%s\n", args[1]);
                    }
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
                    printf("-----------------------------------------------------");
                    printf("\nMyShell is paused, press the ENTER key to continue... ");  // https://stackoverflow.com/questions/18801483/press-any-key-to-continue-function-in-c
                    getchar();   
                    return 1;
                }

                else {
                    return 0;
                }
}