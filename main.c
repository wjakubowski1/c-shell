#include <stdio.h>
#include <string.h>
#define MAX_INPUT 1024

// REPL: READ-EVAL-PRINT LOOP

// BASE FOR SHELL WITH NOTES

int main() {
  char input[MAX_INPUT];

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

    printf("%s\n", input); //the \n still works how supposed to, after strcspn terminates \n index
  }

  return 0;

}