#include "kernel/types.h"
#include "user/user.h"

#define READEND 0
#define WRITEEND 1
#define PRIME_NUM 35

void child(int *pipefd);
int
main(int argc, char *argv[])
{
  int pipefd[2];
  int cpid; 

  pipe(pipefd); // pipefd[0] = 3, pipefd[1]) = 4

  cpid = fork();
  if (cpid == 0) {
    child(pipefd); 
  } else {
    close(pipefd[READEND]);
    for (int i = 2; i < PRIME_NUM + 1; i++) {
      write(pipefd[WRITEEND], &i, sizeof(int));
    }
    close(pipefd[WRITEEND]); 
    wait(0);
  }
  exit(0);
}

void
child(int* pipefd) {
  close(pipefd[WRITEEND]); 

  int n;
  int read_result = read(pipefd[READEND], &n, sizeof(int));
  if (read_result == 0) {
    exit(0); 
  } else if (read_result == -1)
  {
    fprintf(2, "read error\n");
  }
  printf("prime %d\n", n);
  
  int child_pipefd[2];
  pipe(child_pipefd);

  int cpid;
  cpid = fork();
  if (cpid == 0) {
    child(child_pipefd);
    exit(0);
  } else {
    int prime = n;
    while (read(pipefd[READEND], &n, sizeof(int))) { 
      if(n % prime > 0) {
        write(child_pipefd[WRITEEND], &n, sizeof(int));
      }
    }
    close(pipefd[READEND]);
    close(child_pipefd[WRITEEND]); 
    wait(0);
  }
}