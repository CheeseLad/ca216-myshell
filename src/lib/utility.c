#include "../myshell.h"
#include <stdlib.h>
#include <stdio.h>

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