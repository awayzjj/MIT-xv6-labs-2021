#include "kernel/types.h"
#include "user/user.h"

#define READEND 0
#define WRITEEND 1
#define BUF_SIZE 800

int
main(int argc, char *argv[])
{
  int pipefd[2];
  int cpid; 
  char buf[BUF_SIZE];
  char ping[] = "ping";
  char pong[] = "pong";

  pipe(pipefd); // pipefd[0] = 3, pipefd[1]) = 4

  cpid = fork();
  if (cpid == 0) {
    read(pipefd[0], buf, BUF_SIZE);
    printf("%d: received %s\n", getpid(), buf);
    close(pipefd[READEND]); 

    write(pipefd[WRITEEND], pong, strlen(pong));
    close(pipefd[WRITEEND]); 
    exit(0);
  } else {
    if(argc > 1) {
      write(pipefd[WRITEEND], argv[1], strlen(argv[1]));
    }
    else {
      write(pipefd[WRITEEND], ping, strlen(ping));
    }
    close(pipefd[WRITEEND]); 

    wait(0);
    read(pipefd[0], buf, BUF_SIZE);
    printf("%d: received %s\n", getpid(), buf);
    close(pipefd[READEND]);
    exit(0);
  }
}