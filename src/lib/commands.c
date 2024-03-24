/* 
Name: Jake Farrell
Student Number: 22349856
I acknowledge the DCU Academic Integrity Policy in this submitted work
*/

#include "../myshell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

int command_clear() { // https://stackoverflow.com/questions/37774983/clearing-the-screen-by-printing-a-character
    printf("\033[H"); 
    printf("\033[2J");
    return 1;
}

int command_pause() { // https://stackoverflow.com/questions/18801483/press-any-key-to-continue-function-in-c
    printf("-----------------------------------------------------");
    printf("\nMyShell is paused, press the ENTER key to continue... ");  
    getchar();  
    return 1;
}

int command_environ() {
    for (int i = 0; environ[i] != NULL; i++) { // loops through each environment variable and prints it out
        printf("%s\n",environ[i]);
    }              
    return 1;
}

int command_quit() { // clean exit upon quit
    printf("----------------------");
    printf("\nMyShell is quitting...\n");
    exit(0);   
}

int command_echo(char * args[MAX_ARGS]) {
    for (int i = 1; args[i] != NULL; i++) {
        if (!strcmp(args[i],">")) {
            break;
        }
        else {
            printf("%s ", args[i]);
        }
    }
    printf("\n");
    return 1;
}

int command_help() {
    FILE *pfile_help = NULL;
    pfile_help = fopen("./manual/readme", "r");
    int c;
    if (pfile_help) { // https://stackoverflow.com/questions/3463426/in-c-how-should-i-read-a-text-file-and-print-all-strings
        while ((c = getc(pfile_help)) != EOF)
            putchar(c);
        fclose(pfile_help);
    }
    printf("\n----------------------------------\n");
    //printf("\nPress the ENTER key to continue... ");  // https://stackoverflow.com/questions/18801483/press-any-key-to-continue-function-in-c
    //getchar();   
    return 1;
}

int command_dir(char * args[MAX_ARGS]) { // https://stackoverflow.com/questions/12489/how-do-you-get-a-directory-listing-in-c
    DIR *dp;
    struct dirent *ep;
    if (args[1] != NULL) {
        dp = opendir(args[1]);
    } 
    else {     
        dp = opendir ("./");
    }
    if (dp != NULL) {
        while ((ep = readdir (dp)) != NULL) { 
            printf("%s\n", ep->d_name);
        }
    } 
    else {
        printf("Directory `%s` not found.\n", args[1]);
        return 1;
    }      
    closedir(dp);
    return 1;
}

int command_cd(char * args[MAX_ARGS]) {
    if (args[1] == NULL) {
        // https://stackoverflow.com/questions/17695413/store-output-of-systemfile-command-as-a-string-in-c
        FILE *dir_file_blank;
        char blank_pwd[MAX_BUFFER] = "";
        dir_file_blank = popen("pwd", "r");
        if (dir_file_blank == NULL) {
            printf("Failed to run command\n" );
            return 1;
        }

        while (fgets(blank_pwd, sizeof(blank_pwd), dir_file_blank) != NULL) { // https://stackoverflow.com/questions/17695413/store-output-of-systemfile-command-as-a-string-in-c
            printf("%s", blank_pwd);
        }
    } 
    else if (args[1]) {
        int dir_status = chdir(args[1]);
        if(dir_status == -1) {
            printf("Directory not found.\n");
            return 1;
        }
        if (dir_status == 0) {
            char newcd[MAX_BUFFER] = "";
            FILE *dir_file_pointer;
            char new_pwd[MAX_BUFFER]= "";
            dir_file_pointer = popen("pwd", "r");
            if (dir_file_pointer == NULL) {
                printf("Failed to run command\n");
                return 1;
            }
            while (fgets(new_pwd, sizeof(new_pwd), dir_file_pointer) != NULL) {
                strcat(newcd, new_pwd);
            }
            int size = strlen(newcd);
            if (newcd[size - 1] == '\n') {
                newcd[size - 1] = '\0';
            }
            setenv("PWD", newcd, 1);
        }
    }
    return 1;         
}

int command(char * args[MAX_ARGS]) {
    if (!strcmp(args[0],"cd")) { // "cd" command
        return command_cd(args);
    }

    else if (!strcmp(args[0],"clr")) { // "clr" command
        return command_clear();
    }

    else if (!strcmp(args[0],"dir")) { // "dir" command
        return command_dir(args);
    }

    else if (!strcmp(args[0], "help")) { // "help" command
        return command_help();
    }

    else if (!strcmp(args[0], "pause")) { // "pause" command
        return command_pause();
    }

    else if (!strcmp(args[0],"environ")) { // "environ" command
        return command_environ();
    }

    else if (!strcmp(args[0],"quit")) {  // "quit" command
        return command_quit(); 
    }

    else if (!strcmp(args[0],"echo")) { // "echo" command
        return command_echo(args);
    }

    else {
        return 0; // internal commands will return 1, external commands will return 0 and will be forked and executed
    }
}