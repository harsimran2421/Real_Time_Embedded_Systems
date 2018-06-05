#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include<sys/stat.h>
#include <semaphore.h>
#include <errno.h>

void main()
{
sem_t *binary_semaphore;
printf("\nProcess 2 initiated\n");
binary_semaphore=sem_open("/sem_name",0);
if(binary_semaphore==SEM_FAILED)
{
  perror("Initialization Failed");
  exit(1);
}
sem_post(binary_semaphore);
printf("\nProcess 2 complete\n");
}
