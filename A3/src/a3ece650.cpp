#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define SIZE 10000

pid_t pid_rgen, pid_a1, pid_a2; // for python script

void sig_handler(int sig) {
  if (sig == SIGCHLD) {
    kill(pid_a1, SIGTERM);
    kill(pid_a2, SIGTERM);
    exit(0);
  }
}

int main(int argc, char *argv[]) {

  setvbuf(stdout, NULL, _IONBF, 0);

  //int rn1 = 0; // return value of execvp() of rgen
  char *rgenArgv[argc]; // for execvp "rgen"
  rgenArgv[0] = (char *) "./rgen";
  int j;
  /* store command line arguments */
  for (j = 1; j < argc; j++) {
    rgenArgv[j] = argv[j];
  }
  rgenArgv[argc] = NULL;

  char *a1Argv[3] = {(char *) "python", (char *) "a1ece650.py", (char *) 0}; // for evecv "a1ece650.py"
  char *a2Argv[2] = {(char *) "./ece650-a2", (char *) 0};

  char readStr[SIZE]; // handle user input

  int pipe_fd1[2]; // rgen -> a1
  int pipe_fd2[2]; // a1 -> a2
  signal(SIGCHLD, sig_handler);
  pipe(pipe_fd1);
  pipe(pipe_fd2);

  while ((pid_rgen = fork()) < 0);
  if (pid_rgen == 0) { // rgen
    close(pipe_fd1[0]);
    dup2(pipe_fd1[1], 1); // replace stdout with pipe_fd1
    close(pipe_fd1[1]);

    execvp(rgenArgv[0], rgenArgv);
    fprintf(stderr, "Error: %s: program not found\n", rgenArgv[0]);
    exit(-1);
  }

  while ((pid_a1 = fork()) < 0);
  if (pid_a1 == 0) { // assignment 1, python
    // rgen -> a1
    close(pipe_fd1[1]);
    dup2(pipe_fd1[0], 0); // replace stdin with pipe_fd1
    close(pipe_fd1[0]);

    // a1 output -> a2
    close(pipe_fd2[0]);
    dup2(pipe_fd2[1], 1); // replace stdout with pipe_fd2
    close(pipe_fd2[1]);

    execvp(a1Argv[0], a1Argv);
    fprintf(stderr, "Error: %s: program not found\n", a1Argv[0]);
    exit(-1);

  }

  while ((pid_a2 = fork()) < 0);
  if (pid_a2 == 0) { // assignment 2
    // a1 -> a2
    close(pipe_fd2[1]);
    dup2(pipe_fd2[0], 0);  // replace stdin with pipe_fd2
    close(pipe_fd2[0]);

    execvp(a2Argv[0], a2Argv);
    fprintf(stderr, "Error: %s: program not found\n", a2Argv[0]);
    exit(-1);

  }
  if (pid_a2 > 0) { // main process
    // main stdin -> a2
    close(pipe_fd2[0]);
    dup2(pipe_fd2[1], 1); // replace stdout with pipe_fd2
    close(pipe_fd2[1]);

    // main stdin -> stdout -> pipe_fd2 -> a2 input
    while ((fgets(readStr, SIZE, stdin)) != NULL) {
      printf("%s\n", readStr);
    }

    kill(pid_rgen, SIGTERM);
    kill(pid_a1, SIGTERM);
    kill(pid_a2, SIGTERM);

  }
  return 0;
}
