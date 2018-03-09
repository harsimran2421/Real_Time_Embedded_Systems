/**
 * @file posix_mq_pthread.c
 * @brief This file contains one parent thread and two child threads sharing data through message   queues.
 *
 * @author Harsimransingh Bindra, Nagarjuna Reddy
 * @date Mar 7, 2018
 *
 */

/
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

#define SNDRCV_MQ "/send_receive_mq1"
#define ERROR (-1)
#define MAX_MSG_SIZE 1024
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

void *receiver(void *arg)
{
  char buffer[MAX_MSG_SIZE];
  int prio;
  int nbytes;

  /* note that VxWorks does not deal with permissions? */
  /* read oldest, highest priority msg from the message queue */
  if((nbytes = mq_receive(mymq, buffer, MAX_MSG_SIZE, &prio)) == ERROR)
  {
    perror("mq_receive");
  }
  else
  {
    buffer[nbytes] = '\0';
    printf("receive: msg %s received with priority = %d, length = %d\n",
           buffer, prio, nbytes);
    printf("Message Recieved Successfully\n");
  }
    
}

static char canned_msg[] = "this is a test, and only a test, in the event of a real emergency, you would be instructed ...";

void *sender(void *arg)
{
  int prio;
  int nbytes;

  /* note that VxWorks does not deal with permissions? */
//  mymq = mq_open(SNDRCV_MQ, O_RDWR, 0666, &mq_attr);
//
//  if(mymq == (mqd_t)ERROR)
//    perror("mq_open");
  printf("\nSending Message\n");
  /* send message with priority=30 */
  if((nbytes = mq_send(mymq, canned_msg, sizeof(canned_msg), 30)) == ERROR)
  {
    printf("\n%d\n", nbytes);
    perror("mq_send");
  }
  else
  {
    printf("send: message successfully sent\n");
  }
  
}

void *posix_mq_thread(void *arg)
{

  mq_attr.mq_msgsize = 1024;  
  mq_attr.mq_maxmsg = 100;
  mq_attr.mq_flags = 0;

  mymq = mq_open(SNDRCV_MQ, O_CREAT|O_RDWR, 0666, &mq_attr);

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

void main()
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
                  posix_mq_thread, 
                  NULL
              );

 pthread_join(threads[0], NULL);
 
}
