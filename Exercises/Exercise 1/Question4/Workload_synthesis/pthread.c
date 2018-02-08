/********************************************
 * *   Authors: Vidur Sarin, Harsimransingh Bindra
 * *   Date Edited: 02/07/2018
 * *
 * *   File: pthread.c
 * *
 * *   Description: source file for pthread scheduling code
 * *     -delta_t
 * *     -*fib10
 * *     -*fib20
 * *     -*scheduler
 * *     -print_scheduler
 * *     -main
 * *THIS CODE CONTAINS CODE FROM SAM SIEWERT'S EXAMPLE FROM THE DESCRIPTION AND CODE FROM THE INDEPENDENT STUDY OF ALSO PROVIDED IN THE DESCRIPTION
 * *
 * *This program contains three threads. Scheulder thread is given the highest priority of 99. Fib10 Fib20 have priorites 98 and 97 respectively as Fib10 generate workload of 10ms and fib 20 generate  * *a workload of 20ms
 * *
 * * VxWorks code provided were used as references.
 * ********************************************************/
#define _GNU_SOURCE
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>

#define NUM_THREADS (3)
#define NUM_CPUS (1)
#define NSEC_PER_SEC (1000000000)
#define NSEC_PER_MSEC (1000000)
#define NSEC_PER_MICROSEC (1000)
#define DELAY_TICKS (1)
#define ERROR (-1)
#define OK (0)

#define fib10_count reqIterations
#define fib20_count fib10_count*2

volatile unsigned int fib = 0, fib0 = 0, fib1 = 1;
unsigned int idx = 0, jdx = 1;
int abortTest_10=0, abortTest_20=0, abortTest, abortIdle = 0;

/* Constant values for the synthetic load generation of 10ms and 20ms using the fib_test() */
unsigned int seqIterations = 110;
unsigned int reqIterations = 5500000;

struct timespec start_time = {0, 0};

sem_t semF10, semF20, SemIdle;

/* FIB_TEST is borrowed from sam siewert's vx works example, from the link provided in the description of question 4. */
#define FIB_TEST(seqCnt, iterCnt)      \
   for(idx=0; idx < iterCnt; idx++)    \
   {                                   \
      fib = fib0 + fib1;               \
      while(jdx < seqCnt)              \
      {                                \
         fib0 = fib1;                  \
         fib1 = fib;                   \
         fib = fib0 + fib1;            \
         jdx++;                        \
      }                                \
   }                                   \

/* struct creation of pthreads */
typedef struct
{
    int threadIdx;
} threadParams_t;


pthread_t threads[NUM_THREADS], schedularThread;
threadParams_t threadParams[NUM_THREADS];
pthread_attr_t rt_sched_attr[NUM_THREADS];
pthread_attr_t main_attr;
int rt_max_prio1, rt_min_prio1;
struct sched_param rt_param[NUM_THREADS];
struct sched_param main_param,main_param1;
struct sched_param thrd1,thrd2;
pid_t mainpid,mainpid1;

/* delta_t() borrowed from sam siewert's example codes */
int delta_t(struct timespec *stop, struct timespec *start, struct timespec *delta_t)
{
  int dt_sec=stop->tv_sec - start->tv_sec;
  int dt_nsec=stop->tv_nsec - start->tv_nsec;

  if(dt_nsec >= 0)
  {
    delta_t->tv_sec=dt_sec;
    delta_t->tv_nsec=dt_nsec;
  }
  else
  {
    delta_t->tv_sec=dt_sec-1;
    delta_t->tv_nsec=NSEC_PER_SEC+dt_nsec;
  }

  return(1);
}

void *fib10(void *threadp)
{
    int sum=0, i;
    pthread_t thread;
    cpu_set_t cpuset;
    struct timespec finish_time = {0, 0};
    struct timespec thread_dt = {0, 0};
    threadParams_t *threadParams = (threadParams_t *)threadp;
    struct sched_param param;
    int policy;
    CPU_ZERO(&cpuset); /* setting CPU affinity to core zero */

    while(!abortTest_10)
    {
      /* locking the semaphore from the calling thread */
      sem_wait(&semF10);				
      
      /* synthetic load generation of 10ms */
      FIB_TEST(seqIterations, fib10_count);	
      
      /* get the scheduling parameters form the calling thread */
      pthread_getschedparam(threads[1], &policy, &param);

      clock_gettime(CLOCK_REALTIME, &finish_time);
      delta_t(&finish_time, &start_time, &thread_dt);	
      
      printf("(%s):Thread Id=%d | Priority: %d | run time: %ld sec, %lf msec\n",\
              __func__, threadParams->threadIdx,param.sched_priority, thread_dt.tv_sec,\
              (double)((double)thread_dt.tv_nsec / NSEC_PER_MSEC));
    }
}

void *fib20(void *threadp)
{
    int sum=0, i;
    pthread_t thread;
    cpu_set_t cpuset;
    struct timespec finish_time = {0, 0};
    struct timespec thread_dt = {0, 0};
    threadParams_t *threadParams = (threadParams_t *)threadp;
    struct sched_param param;
    int policy;

    CPU_ZERO(&cpuset);

while (!abortTest_20)
{
    /*locking the semaphore frrom the calling thread */
    sem_wait(&semF20);			

    /* synthetic load generation of 20ms */
    FIB_TEST(seqIterations, fib20_count); 

    pthread_getschedparam(threads[2], &policy, &param);
    clock_gettime(CLOCK_REALTIME, &finish_time);

    delta_t(&finish_time, &start_time, &thread_dt);	
    printf("(%s):Thread Id=%d | Priority: %d | run time: %ld sec, %lf msec\n",\
        __func__, threadParams->threadIdx,param.sched_priority, thread_dt.tv_sec,
        (double)((double)thread_dt.tv_nsec / NSEC_PER_MSEC));

   }
 }

/* The scheduler function has the max. priority and calls the fib10 and fib20 functions that calc. the fib for 10 and 20ms respectively */
void *scheduler(void *threadp)
{

    printf("\n----------------------- Start Scheduler Test -------------------------------\n");
    printf("Iteration Settings:\nreqIterations: %u\nseqIterations: %u\n\n", reqIterations, seqIterations);

    int policy;
    struct sched_param param;
    int sum = 0, i,rc;
    pthread_t thread;
    cpu_set_t cpuset;
    struct timespec finish_time = {0, 0};
    struct timespec thread_dt = {0, 0};
    threadParams_t *threadParams = (threadParams_t *)threadp;
    
    /* initialize semaphores for the fib10 and fib20 threads */
    sem_init(&semF10, 0, 1);
    sem_init(&semF20, 0 ,1);
    CPU_ZERO(&cpuset);	

    /* Create the fib10 and fib20 threads */
    pthread_create(&threads[1],   
                      (void *)&rt_sched_attr[1],
                      fib10, 
                      (void *)&(threadParams[1])
                     );
    pthread_create(&threads[2],
                      (void *)&rt_sched_attr[2],
                      fib20, 
                      (void *)&(threadParams[2])
                     );

    /* Get the start time for the load */
    clock_gettime(CLOCK_REALTIME, &start_time);	

    /* Sleep for the period of the task and accquire the semaphores in turns to create 
     * the required scheduling output
     */
    usleep(20000);					
    sem_post(&semF10);			
    usleep(20000);
    sem_post(&semF10);
    usleep(10000);
    abortTest_20=1;
    sem_post(&semF20);
    usleep(10000);
    sem_post(&semF10);
    usleep(20000);
    abortTest_10=1;
    sem_post(&semF10);
    usleep(10000);
    usleep(10000);

    /* Join the created threads */
    pthread_join(threads[1],NULL);
    pthread_join(threads[2],NULL);

    /* Get the time posted by the realtime clock */
    clock_gettime(CLOCK_REALTIME, &finish_time);

    /* get the scheduling parameters from the calling thread */
    pthread_getschedparam(threads[0],&policy,&param);	

    /* Calc. the time delta between the start and stop times */
    delta_t(&finish_time, &start_time, &thread_dt);		
    printf("\nThread Id=%d | Priority: %d | run time: %ld sec, %lf msec, %lf usec\n",\
            threadParams->threadIdx,param.sched_priority,\
            thread_dt.tv_sec, (double)((double)thread_dt.tv_nsec / NSEC_PER_MSEC),\
            (double)(thread_dt.tv_nsec / NSEC_PER_MICROSEC));
}

void print_scheduler(void)
{
   int schedType;

   schedType = sched_getscheduler(getpid());

   switch(schedType)
   {
     case SCHED_FIFO:
           printf("Pthread Policy is SCHED_FIFO\n");
           break;
     case SCHED_OTHER:
           printf("Pthread Policy is SCHED_OTHER\n");
       break;
     case SCHED_RR:
           printf("Pthread Policy is SCHED_OTHER\n");
           break;
     default:
       printf("Pthread Policy is UNKNOWN\n");
   }
}

int main (int argc, char *argv[])
{
  struct timespec start = {0, 0};
  struct timespec finish = {0, 0};
  struct timespec dt = {0, 0};
  int abortTest = 0;
  int rc = 0;
  int i = 0;
  int scope = 0;
  int rt_max_prio;

  /* Set CPU affinity to cpu set */
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);	
  for(i=0; i < NUM_CPUS; i++)
    CPU_SET(i, &cpuset);

  mainpid = getpid();

  rt_max_prio = sched_get_priority_max(SCHED_FIFO);

  /* Set the priority of the calling thread to max. */
  rc = sched_getparam(mainpid, &main_param);
  main_param.sched_priority = rt_max_prio;
  rc = sched_setscheduler(getpid(), SCHED_FIFO, &main_param);

  /* Print some information about the scheduling policy */
  printf("\nScheduling Policy: ");
  print_scheduler();
  pthread_attr_getscope(&main_attr, &scope);
  if(scope == PTHREAD_SCOPE_SYSTEM)
    printf("System Scope Type: PTHREAD SCOPE SYSTEM\n");
  else if (scope == PTHREAD_SCOPE_PROCESS)
    printf("System Scope Type: PTHREAD SCOPE PROCESS\n");
  else
    printf("System Scope Type: PTHREAD SCOPE UNKNOWN\n");

  /* Set the attribute values for all the threads created and initialize them */
  for(i=0; i <NUM_THREADS; i++)
  {
    /* intializing the pthread attributes for the three threads */
    rc=pthread_attr_init(&rt_sched_attr[i]);

    /* set to explicit schedule policy and later to SCHED_FIFO */
    rc=pthread_attr_setinheritsched(&rt_sched_attr[i], PTHREAD_EXPLICIT_SCHED);

    /* set the schedule policy of the three threads to SCHED_FIFO */
    rc=pthread_attr_setschedpolicy(&rt_sched_attr[i], SCHED_FIFO);	

    /* set the affinity of the CPU cores to zero */
    rc=pthread_attr_setaffinity_np(&rt_sched_attr[i], sizeof(cpu_set_t), &cpuset);

    /* set the priorities of the three threads as 99,98 and 97 respectively */
    rt_param[i].sched_priority=rt_max_prio-i;

    /* set the scheduling parameters of the three pthreads */
    pthread_attr_setschedparam(&rt_sched_attr[i], &rt_param[i]);	

    /* fill the threadParams with their corresponding numbers */
    threadParams[i].threadIdx=i;					
  }

  /* Create the scheduling thread */
  pthread_create( &threads[0],   
                  (void *)&rt_sched_attr[0],
                  scheduler, 
                  (void *)&(threadParams[0])
              );

  pthread_join(threads[0], NULL);

  printf("------------------------- End of Scheduler Test -----------------------------\n");
}
