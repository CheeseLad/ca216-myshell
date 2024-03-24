NAME
      myshell

SYNOPSIS
      myshell [command_name] [arguments] [options] [file_name]

DESCRIPTION

      MyShell is a fully-featured shell written in C that can execute commands, redirect input and output and execute processes in the background.

BUILT-IN COMMANDS

      cd [arguments]: Change directory
      clr: Clear the screen
      dir [arguments]: List the contents of the current directory
      environ: List all environment variables
      echo: Print a string to the console
      help: Display the user manual
      pause: Pause the shell until the user presses enter
      quit: Exit the shell
      
ENVIRONMENT

      Upon starting up MyShell, a new environment variable called `SHELL` is created and set to the path where the shell executable is located.
      The command `environ` can be used to list all of the environment variables currently set on the system.
      When executing a non-built-in command, an environment variable called `PARENT` is created and links to where the shell executable is stored.

I/O REDIRECTION

      Redirecting to stdout has multiple options:
            `>`: Redirects the output of a command to a file, overwriting the file if it already exists, the file will be created if it does not exist
                  Example: `ls > file.txt`
            `>>`: Redirects the output of a command to a file, appending to the file if it already exists, the file will be created if it does not exist
                  Example: `ls >> file.txt`

      Redirecting to stdin has one option:
            `<`: Redirects the input of a command to a file
                  Example: `cat < file.txt`
            
      Error handling is in place for both redirections, alerting the user if the file does not exist or cannot be opened.


REFERENCES

      [1] B. Fox and C. Ramey, “Bash(1): GNU bourne-again shell - linux man page,” bash(1): GNU Bourne-Again SHell - Linux man page, https://linux.die.net/man/1/bash (accessed Mar. 23, 2024). 

AUTHOR

      Name: Jake Farrell
      Student Number: 22349856
      I acknowledge the DCU Academic Integrity Policy in this submitted work