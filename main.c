#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

// REPL: READ-EVAL-PRINT LOOP

// BASE FOR SHELL WITH NOTES

int main() {
  char input[MAX_INPUT];
  char* args[MAX_ARGS]; //array of strings to save arguments/TOKENS

  while (1) {
    printf("sh>");
    fflush(stdout); //flush the buffer

    //null prevention
    if(fgets(input, MAX_INPUT, stdin) == NULL) {
      perror("fgets failed");
      continue;
    }

    //unnecessary \n in "exit\n" removal: strcmp counts the \n after hitting enter after response
    input[strcspn(input, "\n")] = '\0'; //strcspn to find index (\n) in input >> \0 is a way to null terminate strings in C (this makes the end of the string)

    //string compare
    if(strcmp(input, "exit") == 0) {
      printf("goodbye!\n");
      break;
    }
    //convert string to token
    char* token = strtok(input, " "); // strtok creates tokens separated by space (" ")
    int i = 0;

    while (token != NULL && i < MAX_ARGS) { //loop untill there are no more tokens or buffer overflows
      args[i++] = token; //inserts first token into first position >> goes to next position and prepares empty space
      token = strtok(NULL, " "); //makes strtok look for next token, after the one it just inserted, istead of starting from the beginning
    }

    args[i] = NULL; //null terminate

    //implementing cd into the shell
    if (strcmp(args[0], "cd") == 0) {
      if(args[1] == NULL) { //if the second token is empty, output error msg (ex. "cd  ")
        perror("cd: missing arguments");
      } else if(chdir(args[1]) != 0) { //change dir
          perror("cd failed");
      }
      continue;
    }

    //executeing
    pid_t pid = fork(); //clone of the program as a child process >> pid to declare which fork is parent process and child process
    
    if(pid == 0) { //0 means it's the child process
      execvp(args[0], args); //takes first token as command and then takes all of the arguments
      perror("execvp fail");
      exit(EXIT_FAILURE);
    } else if(pid > 0) { //bigger than 0 means it's the parent process
        int status;
        waitpid(pid, &status, 0);
        printf("Exist status: %d\n", status);
    } else {
      perror("fork has failed");
    }

  }

  return 0;

}
