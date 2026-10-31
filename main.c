#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

// REPL: READ-EVAL-PRINT LOOP

// BASE FOR SHELL WITH NOTES

int main() {
  char input[MAX_INPUT];
  char *args[MAX_ARGS]; // array of strings to save arguments/TOKENS

  while (1) {
    printf("sh>");
    fflush(stdout); // flush the buffer

    // null prevention
    if (fgets(input, MAX_INPUT, stdin) == NULL) {
      perror("fgets failed");
      continue;
    }

    // unnecessary \n in "exit\n" removal: strcmp counts the \n after hitting
    // enter after response
    input[strcspn(input, "\n")] = '\0';
    // strcspn to find index (\n) in input >> \0 is a way to null
    // terminate strings in C (this makes the end of the string)

    // string compare
    if (strcmp(input, "exit") == 0) {
      printf("goodbye!\n");
      break;
    }
    // convert string to token
    int is_background = 0;    // flag needed for & detection
    char *output_file = NULL; // value to preserve file name for > redirection
    char *input_file = NULL;  // value to preserve file name for < redirection

    char *token = strtok(input, " ");
    // strtok creates tokens separated by space (" ")
    int i = 0;

    while (token != NULL && i < MAX_ARGS) { // loop untill there are no more
                                            // tokens or buffer overflows
      if (strcmp(token, "&") == 0) { // if the token is &, set the is_background
                                     // flag to = 1, and we break the loop
        is_background = 1;
        break; // breaks the loop and & won't be saved into args[i]
      }

      if (strcmp(token, ">") == 0) {
        // if > has been found, the next token has to be the file's name
        token = strtok(NULL, " "); // recall strtok again to download the file
        output_file = token;       // save the pointer into a file
        break; // breaks the loop and & won't be saved into args[i]
      }

      if (strcmp(token, "<") == 0) {
        token = strtok(NULL, " ");
        input_file = token;
        break;
      }

      args[i++] = token; // inserts first token into first position >> goes to
                         // next position and prepares empty space
      token = strtok(NULL, " ");
      // makes strtok look for next token, after the one it
      // just inserted, istead of starting from the beginning
    }

    args[i] = NULL; // null terminate

    if (args[0] == NULL) {
      continue; // prevent segmentation fault error code after not typeing
                // anything and pressing enter
    }

    // pipe detection
    int pipe_index = -1;
    for (int j = 0; args[j] != NULL; j++) {
      if (strcmp(args[j], "|") == 0) {
        args[j] = NULL;     // terminates the left command array here
        pipe_index = j + 1; // marks where the right command begins
        break;
      }
    }

    // implementing cd into the shell
    if (strcmp(args[0], "cd") == 0) {
      if (args[1] == NULL) { // if the second token is empty, output error msg
        perror("cd: missing arguments");
      } else if (chdir(args[1]) != 0) { // change dir
        perror("cd failed");
      }
      continue;
    }

    // pipe execution logic
    if (pipe_index != -1) {
      int fd[2];
      if (pipe(fd) == -1) {
        perror("pipe failed");
        continue;
      }
    }

    // shell execution
    pid_t pid = fork();
    // clone of the program as a child process >> pid to declare
    // which fork is parent process and child process

    if (pid == 0) { // 0 means it's the child process

      // redirections
      if (output_file != NULL) {
        // to check flag set during parsing, if not null, then the
        // parser intercepted > symbol and saved the filename
        int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        // file descriptor, new file gets assigned number 3 (0, 1, 2
        // are reserved) >> bitwise flags (O_) >> set permissions
        if (fd < 0) { // open() from line before returns -1 if it fails
          perror("failed to open output file");
          exit(EXIT_FAILURE);
        }

        // the kernel closes whatever was previously at slot 1, and makes slot 1
        // point to the text file
        if (dup2(fd, STDOUT_FILENO) <
            0) { // core system call for redirection >> duplicates an existing
                 // fd, forcing newfd slot to point at same file as oldfd slot
          perror("failed to redirect standard output");
          exit(EXIT_FAILURE);
        }

        close(fd);
      }

      if (input_file != NULL) {
        int fd = open(input_file, O_RDONLY); // permissions argument not needed
                                             // when not creating a file
        if (fd < 0) {
          perror("failed to open output file");
          exit(EXIT_FAILURE);
        }

        if (dup2(fd, STDIN_FILENO) < 0) {
          perror("failed to redirect standard input");
          exit(EXIT_FAILURE);
        }

        close(fd);
      }

      execvp(args[0], args); // takes first token as command and then takes all
                             // of the arguments
      perror("execvp fail");
      exit(EXIT_FAILURE);
    } else if (pid > 0) { // bigger than 0 means it's the parent process
      if (is_background ==
          0) { // parent checks if the process should work in the background
        int status;
        waitpid(pid, &status, 0);
      } else {
        printf("(Process running in the background) PID: %d\n",
               pid); // inform the user about the pid process in the background
      }
    } else {
      perror("fork has failed");
    }
  }

  return 0;
}
