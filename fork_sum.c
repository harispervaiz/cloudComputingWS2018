#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1024
#define READ  0
#define WRITE 1

int sum(int a, int b) {
  int i, s=0;
  for (i=a; i<=b; i++)
    s += i;
  return s;
}

int main() {

  int   parent_result;
  int   child_result;
  int   fd[2];
  pid_t pid;
  int selectedNo1,selectedNo2;

  printf("Enter the starting range of numbers to add\n");
  scanf("%d",&selectedNo1);
  printf("Enter the ending range of numbers to add\n");
  scanf("%d",&selectedNo2);

  int mid = selectedNo2/2;
  int next = mid+1;
  printf("Mid value is :%d \n",mid);

  printf("Next value is :%d \n",next);

  if (-1 == pipe(fd)) {
    fprintf(stderr, "Pipe failed\n");
    exit(EXIT_FAILURE);
  }

  pid = fork();

  if (pid < 0) {
    fprintf(stderr, "Pipe failed\n");
    exit(EXIT_FAILURE);
  }

  else if (pid == 0) {
    close(fd[READ]);
    clock_t t;
    t = clock();
    child_result = sum(selectedNo1, mid);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("Child process took %f seconds to execute \n", time_taken);
    write(fd[WRITE], &child_result, sizeof(int));
    printf("Child sum: %d\n", child_result);
    close(fd[WRITE]);
  }

  else {
    close(fd[WRITE]);
    clock_t t;
    t = clock();
    parent_result = sum(next, selectedNo2);
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("Parent process took %f seconds to execute \n", time_taken);
    printf("Parent sum:  %d\n", parent_result);
    wait(NULL);|
    read(fd[READ], &child_result, sizeof(int));
    close(fd[READ]);
    printf("Total sum is:%d\n", parent_result+child_result);
  }

  return 0;

}
