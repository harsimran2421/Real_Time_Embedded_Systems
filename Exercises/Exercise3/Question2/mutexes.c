/**
 * @file mutexes.c
 * @brief This file contains one parent thread and two child threads sharing global 
 * data protectd by mutex lock synchronization.
 *
 * @author Kovuru Nagarjuna Reddy, Harsimransingh
 * @date Mar 7, 2018
 *
 */

//***********************************************************************************
// Include files
//***********************************************************************************
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <math.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

//***********************************************************************************
// Global variables/structures and Macros
//***********************************************************************************
#define NUM_THREADS    2

typedef struct timespec timespec_t;

/* attitude structure */
typedef struct
{
  double      X;
  double      Y;
  double      Z;
  double      Acceleration;
  double      Roll;
  double      Pitch;
  double      Yaw;
  timespec_t  Sample_time;
}attitude_t;
/* Shared Attitude */
attitude_t attitude;

/* Shared Mutex */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void write_thread();
void read_thread();
const char *threadfun_name[]= { "write_thread", "read_thread" };
static void (*thread_fun[NUM_THREADS]) = { write_thread, read_thread };
bool data_valid=0;

//***********************************************************************************
//Function Definitions
//***********************************************************************************
/******************************************************************//**********
 * @brief write_thread()
 * This thread writes the attitude data to be read by another thread and also 
 * implements mutex lock protection on shared data.
 *****************************************************************************/
void write_thread (void *ptr) 
{
  time_t t = 0;

  /***** Acquiring Mutex lock ******/
  printf("\n[WRITE_THREAD] [INFO] Waiting for lock\n");
  pthread_mutex_lock(&mutex);
  printf("[WRITE_THREAD] [SUCCESS] Lock acquired successfully\n");
        
  /* Initialize the random sequence */
  srand(time(&t));

  /***** Write Attitude data ********/
  attitude.X = rand();
  attitude.Y = rand();
  attitude.Z = rand();
  attitude.Acceleration = rand();
  attitude.Roll = rand();
  attitude.Pitch = rand();
  attitude.Yaw = rand();
  clock_gettime(0, &(attitude.Sample_time));
  data_valid=1;
  printf("[WRITE_THREAD] [INFO] Writing completed\n");

  /**** print the written data to compare with the read data *****/
  printf("[WRITE_THREAD] [INFO] Data Written\n");
  printf("Attitude Data \n{\n"
         "\tX:            %lf\n"
         "\tY:            %lf\n"
         "\tZ:            %lf\n"
         "\tAcceleration: %lf\n"
         "\tRoll:         %lf\n"
         "\tPitch:        %lf\n"
         "\tYaw:          %lf\n"
         "\tTimestamp:    %ld\n}\n",
         attitude.X,
         attitude.Y,
         attitude.Z,
         attitude.Acceleration,
         attitude.Roll,
         attitude.Pitch,
         attitude.Yaw,
         attitude.Sample_time.tv_nsec); 

  /***** Release the Mutex Lock ******/
  pthread_mutex_unlock(&mutex);
  printf("[WRITE_THREAD] [INFO] Released Lock\n\n");
        
  /*delay*/
  sleep(1);
}

/******************************************************************//**********
 * @brief read_thread()
 * This thread reads the attitude data written by another thread and also 
 * implements mutex lock protection on shared data.
 *****************************************************************************/
void read_thread(void) 
{
  int read=0;

  while(read==0)
  {
    /***** Acquiring Mutex lock ******/
    printf("[READ_THREAD] [INFO] Waiting for lock\n");
    pthread_mutex_lock(&mutex);
    printf("[READ_THREAD] [SUCCESS] Lock acquired successfully\n");
    if(data_valid ==1)
    {
      /***** Reading the data **********/
      printf("[READ_THREAD] [INFO] Data Read\n");
      printf("Attitude Data \n{\n"
         "\tX:            %lf\n"
         "\tY:            %lf\n"
         "\tZ:            %lf\n"
         "\tAcceleration: %lf\n"
         "\tRoll:         %lf\n"
         "\tPitch:        %lf\n"
         "\tYaw:          %lf\n"
         "\tTimestamp:    %ld\n}\n",
         attitude.X,
         attitude.Y,
         attitude.Z,
         attitude.Acceleration,
         attitude.Roll,
         attitude.Pitch,
         attitude.Yaw,
         attitude.Sample_time.tv_nsec); 
      read=1;        
      printf("[READ_THREAD] [INFO] Reading completed\n");
    }
    /***** Release the Mutex Lock ******/
    pthread_mutex_unlock(&mutex);
    printf("[READ_THREAD] [INFO] Released Lock\n");

    /*delay*/
    sleep(1);
  }
}

/******************************************************************//**********
 * @brief main()
 * This function creates two threads and waits until their completion.
 *****************************************************************************/
int main( int argc, char** argv )
{
  pthread_t threads[NUM_THREADS];
  int ret;
  int index;

  /***** Mutex Initialization ****/
  printf("[MAIN] [INFO] Initializing the mutex lock \n");
  /* initialize the mutex */
  //ret=pthread_mutex_init(&mutex, NULL);
  if(ret==0)
    printf("[MAIN] [SUCCESS] Mutex Initialized \n");
  else
    printf("[MAIN] [ERROR] Mutex not initialized \n");

  /**** Thread Creation *******/
  printf("[MAIN] [INFO] Creating Threads \n");
  /* run loop required thread number of times */
  for (index = 0; index<NUM_THREADS; index++) 
  {
    ret=pthread_create(&threads[index], NULL, thread_fun[index], NULL);
    if(ret==0)
      printf("[MAIN] [SUCCESS] Thread %s created successfully\n", threadfun_name[index]);
    else
      printf("[MAIN] [ERROR] Thread %s not created\n", threadfun_name[index]);
  }

  /***** Waiting for threads to complete ******/
  printf("[MAIN] [INFO] Waiting for Threads completion \n");
  /* run loop required thread number of times */
  for (index = 0; index<NUM_THREADS; index++) 
  {
    ret=pthread_join(threads[index], NULL);
    if(ret==0)
      printf("[MAIN] [SUCCESS] Thread %s has completed\n", threadfun_name[index]);
    else
      printf("[MAIN] [ERROR] Thread %s not completed\n", threadfun_name[index]);
  }

  exit(EXIT_SUCCESS);
}
