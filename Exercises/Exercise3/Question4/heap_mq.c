/****************************************************************************/
/*                                                                          */
/* Sam Siewert - 10/14/97                                                   */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

//Modifications made to Dr Sam Stiewert's Code
#define _GNU_SOURCE
//#include <msgQLib.h>
//#include <mqueue.h>
//#include <errnoLib.h> 
//#include <ioLib.h> 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <mqueue.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sched.h>
#include <time.h>

#define SNDRCV_MQ "/send_receive_mq"
#define ERROR (-1)
#define MAX_MSG_SIZE 100
#define NUM_THREADS 3

struct mq_attr mq_attr;
static mqd_t mymq;

typedef struct
{
    int threadIdx;
} threadParams_t;


pthread_t threads[NUM_THREADS], schedularThread;
threadParams_t threadParams[NUM_THREADS];
pthread_attr_t rt_sched_attr[NUM_THREADS];
struct sched_param rt_param[NUM_THREADS];
struct sched_param main_param,main_param1;


/* receives pointer to heap, reads it, and deallocate heap memory */

void *receiver(void *threadp)
{
  char buffer[sizeof(void *)+sizeof(int)];
  void *buffptr; 
  int prio;
  int nbytes;
  //int count = 0;
  int id;
  int  i;


    while(1) {
  /* read oldest, highest priority msg from the message queue */

    printf("Reading %ld bytes\n", sizeof(void *));
  
    if((nbytes = mq_receive(mymq, buffer, (size_t)(sizeof(void *)+sizeof(int)), &prio)) < 0)
/*
    if((nbytes = mq_receive(mymq, (void *)&buffptr, (size_t)sizeof(void *), &prio)) == ERROR)
*/
    {
      perror("mq_receive");
    }
    else
    {
      memcpy(&buffptr, buffer, sizeof(void *));
      memcpy((void *)&id, &(buffer[sizeof(void *)]), sizeof(int));
      printf("receive: ptr msg 0x%X received with priority = %d, length = %d, id = %d\n", buffptr, prio, nbytes, id);

      printf("contents of ptr = \n%s\n", (char *)buffptr);

      free(buffptr);

      printf("heap space memory freed\n");
    }
    
    sleep(10);
  }

}


static char imagebuff[4096];

void *sender(void* threadp)
{
  char buffer[sizeof(void *)+sizeof(int)];
  void *buffptr;
  int prio;
  int nbytes;
  int id = 999;
int i;
  while(1) {

    /* send malloc'd message with priority=30 */

    buffptr = (void *)malloc(sizeof(imagebuff));
    strcpy(buffptr, imagebuff);
    printf("Message to send = %s\n", (char *)buffptr);

  printf("Sending %ld bytes\n", sizeof(buffptr));

    memcpy(buffer, &buffptr, sizeof(void *));
    memcpy(&(buffer[sizeof(void *)]), (void *)&id, sizeof(int));

    if((nbytes = mq_send(mymq, buffer, (size_t)(sizeof(void *)+sizeof(int)), 30)) < 0)
    {
      perror("mq_send");
      printf("\nsorry\n");
    }
    else
    {
      printf("send: message ptr 0x%X successfully sent\n", buffptr);
    }
   sleep(3);
  }
  
}


static int sid, rid;


void *heap_mq(void* threadp)
{
  int i, j;
  char pixel = 'A';

  for(i=0;i<4096;i+=64) {
    pixel = 'A';
    for(j=i;j<i+64;j++) {
      imagebuff[j] = (char)pixel++;
    }
    imagebuff[j-1] = '\n';
  }
  imagebuff[4095] = '\0';
  imagebuff[63] = '\0';

  printf("buffer =\n%s", imagebuff);

  mq_attr.mq_maxmsg = 100;
  mq_attr.mq_msgsize = sizeof(void *)+sizeof(int);

  mq_attr.mq_flags = 0;


  /* setup common message q attributes */
   /* note that VxWorks does not deal with permissions? */
  mymq = mq_open(SNDRCV_MQ, O_CREAT|O_RDWR,0666, &mq_attr);

  if(mymq == (mqd_t)ERROR)
    perror("mq_open");

    
    pthread_create(&threads[1],   
                      (void *)&rt_sched_attr[1],
                      receiver, 
                      NULL);
    pthread_create(&threads[2],
                      (void *)&rt_sched_attr[2],
                      sender, 
                      NULL);
   //  /* receiver runs at a higher priority than the sender */

  pthread_join(threads[1],NULL);
  pthread_join(threads[2],NULL);
 
}

void shutdown(void)
{
  mq_close(mymq);
  pthread_join(threads[1],NULL);
  pthread_join(threads[2],NULL);

}

void main(void)
{
int i;
  int rt_max_prio;
  int rc;
  /* setup common message q attributes */
  
  rt_max_prio = sched_get_priority_max(SCHED_FIFO);

  rt_max_prio = sched_get_priority_max(SCHED_FIFO);

  /* Set the priority of the calling thread to max. */
  rc = sched_setscheduler(getpid(), SCHED_FIFO, &main_param);

 for(i=0; i <NUM_THREADS; i++)
  {
    /* intializing the pthread attributes for the three threads */
    rc=pthread_attr_init(&rt_sched_attr[i]);

    /* set to explicit schedule policy and later to SCHED_FIFO */
    rc=pthread_attr_setinheritsched(&rt_sched_attr[i], PTHREAD_EXPLICIT_SCHED);

    /* set the schedule policy of the three threads to SCHED_FIFO */
    rc=pthread_attr_setschedpolicy(&rt_sched_attr[i], SCHED_FIFO);	



    /* set the priorities of the three threads as 99,98 and 97 respectively */
    rt_param[i].sched_priority=99-i;

    /* set the scheduling parameters of the three pthreads */
    pthread_attr_setschedparam(&rt_sched_attr[i], &rt_param[i]);	

    /* fill the threadParams with their corresponding numbers */
//    threadParams[i].threadIdx=i;					
  }
  /* Create the scheduling thread */
  pthread_create( &threads[0],   
                  (void *)&rt_sched_attr[0],
                  heap_mq, 
                  NULL
              );

 pthread_join(threads[0], NULL);
 mq_close(mymq);


  // Create two communicating processes right here
//  sender();
//  receiver();
//  shutdown();   
}


