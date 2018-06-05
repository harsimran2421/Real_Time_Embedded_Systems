#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include<sys/stat.h>
#include <semaphore.h>
#include <errno.h>

int main()
{
sem_t *binary_semaphore;
printf("\nProcess 1 initiated\n");
binary_semaphore=sem_open("/sem_name",O_CREAT, 0666 , 0);
if(binary_semaphore==SEM_FAILED)
{
  perror("Initialization Failed");
  exit(1);
}
sem_wait(binary_semaphore);
printf("\nProcess 1 complete\n");
return 0;
}
