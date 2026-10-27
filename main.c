#include <stdio.h>
#include <string.h>
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
      printf("goodbye!");
      break;
    }
    //convert string to token
    char* token = strtok(input, " "); // strtok creates tokens separated by space (" ")
    int i = 0;

    while (token != NULL && i < MAX_ARGS) { //loop untill there are no more tokens or buffer overflows
      args[i++] = token; //inserts first token into first position >> goes to next position and prepares empty space
      token = strtok(NULL, " "); //makes strtok look for next token, after the one it just inserted, istead of starting from the beginning
    }

    printf("%s\n", input); //the \n still works how supposed to, after strcspn terminates \n index
  }

  return 0;

}