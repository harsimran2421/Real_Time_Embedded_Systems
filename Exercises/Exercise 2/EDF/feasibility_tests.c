/********************************************
 * *   Date Edited: 02/23/2018
 * *
 * *   File: feasibilty_tests.c
 * *
 * *   Description: source file for EDF scheduling code
 * *     -main
 * *
 * *THIS CODE CONTAINS CODE FROM SAM SIEWERT'S EXAMPLE FROM THE DESCRIPTION AND https://www.youtube.com/watch?v=XrwqY205Zfo
 * *
 * *
 * * VxWorks code provided were also used as references.
 * ********************************************************/

#include <math.h>
#include<string.h>
#include<stdlib.h>
#include <stdio.h>
#include "EDF_Functions.h"
#include <stdint.h>
#define TRUE 1
#define FALSE 0

#define DEADLINE_EX6

// int processesNumber;
char help[255];
int i,j,h;
float f;

/* Example 0 -> U=0.7333 */
uint32_t example0_period[] = {2, 10, 15};
uint32_t example0_wcet[] = {1, 1, 2};

/* Example 1 -> U=0.9857 */
uint32_t example1_period[] = {2, 5, 7};
uint32_t example1_wcet[] = {1, 1, 2};

/* Example 2 -> U=0.996 */
uint32_t example2_period[] = {2, 5, 7, 13};
uint32_t example2_wcet[] = {1, 1, 1, 2};

/* Example 3 -> U=0.93 */
uint32_t example3_period[] = {3, 5, 15};
uint32_t example3_wcet[] = {1, 2, 3};

/* Example 4 -> U=1.0 */
uint32_t example4_period[] = {2, 4, 16};
uint32_t example4_wcet[] = {1, 1, 4};

/* Additional feasibility tests */
/* Example 5 -> U=1.0 */
uint32_t example5_period[] = {2, 5, 10};
uint32_t example5_wcet[] = {1, 2, 1};

/* Example 6 -> U=0.9967033 */
uint32_t example6_period[] = {2, 5, 7, 13};
uint32_t example6_wcet[] = {1, 1, 1, 2};
uint32_t example6_deadline[] = {2, 3, 7, 15};

/* Example 7 -> U=1.00 */
uint32_t example7_period[] = {3, 5, 15};
uint32_t example7_wcet[] = {1, 2, 4};

/* Example 8 -> U=0.9967033 */
uint32_t example8_period[] = {2, 5, 7, 13};
uint32_t example8_wcet[] = {1, 1, 1, 2};

/* Example 9 -> U= 1.00 */
uint32_t example9_period[] = {6, 8, 12, 24};
uint32_t example9_wcet[] = {1, 2, 4, 6};



int main(void)
{
	uint32_t Number_of_services;
	printf("EDF Policy Feasibility Tests \n");
	for(i=0; i<102400; i++);
	printf("done\n");

	printf("******** Test for Feasibility ********\n\n");
 	printf("\nSchedulability Tests for all Examples \n");
    /* Example 0 */
	Number_of_services = sizeof(example0_period)/sizeof(uint32_t);
	Fetch_process(Number_of_services, example0_period, example0_wcet, example0_period);
	Calculate_LCM(Number_of_services);
	f = schedulability(Number_of_services);
	if(f <= 1)
	{
        printf("Example-0 F U=%4.2f (C1=1, C2=1, C3=2; T1=2, T2=10, T3=15; T=D):The Set is FEASIBLE\n\n",((1.0/2.0) + (1.0/10.0) + (2.0/15.0)));
		Schedule_process(Number_of_services);
	}
    else
        printf("Example-0 F U=%4.2f (C1=1, C2=1, C3=2; T1=2, T2=10, T3=15; T=D):The Set is INFEASIBLE\n\n",((1.0/2.0) + (1.0/10.0) + (2.0/15.0)));

	/* Example 1 */
	Number_of_services = sizeof(example1_period)/sizeof(uint32_t);
	Fetch_process(Number_of_services, example1_period, example1_wcet, example1_period);
	Calculate_LCM(Number_of_services);
	f = schedulability(Number_of_services);
	if(f <= 1)
	{
        printf("Example-1 F U=%4.2f (C1=1, C2=1, C3=2; T1=2, T2=5, T3=7; T=D):The Set is FEASIBLE\n\n",((1.0/2.0) + (1.0/5.0) + (2.0/7.0)));
		    Schedule_process(Number_of_services);
	}
    else
        	printf("Example-1 F U=%4.2f (C1=1, C2=1, C3=2; T1=2, T2=5, T3=7; T=D):The Set is INFEASIBLE\n\n",((1.0/2.0) + (1.0/5.0) + (2.0/7.0)));

/* Example 2 */
	Number_of_services = sizeof(example2_period)/sizeof(uint32_t);
	Fetch_process(Number_of_services, example2_period, example2_wcet, example2_period);
	Calculate_LCM(Number_of_services);
	f = schedulability(Number_of_services);
	if(f <= 1)
	{
        printf("Example-2 U=%4.2f (C1=1, C2=1, C3=1, C4=2; T1=2, T2=5, T3=7, T4=13; T=D):The set is FEASIBLE \n\n",((1.0/2.0) + (1.0/5.0) + (1.0/7.0) + (2.0/13.0)));
		Schedule_process(Number_of_services);
	}
    else
        printf("Example-2 U=%4.2f (C1=1, C2=1, C3=1, C4=2; T1=2, T2=5, T3=7, T4=13; T=D):The set is INFEASIBLE \n\n",((1.0/2.0) + (1.0/5.0) + (1.0/7.0) + (2.0/13.0)));


	/* Example 3 */
	Number_of_services = sizeof(example3_period)/sizeof(uint32_t);
	Fetch_process(Number_of_services, example3_period, example3_wcet, example3_period);
	Calculate_LCM(Number_of_services);
	f = schedulability(Number_of_services);
	if(f <= 1)
	{

    printf("Example-3 U=%4.2f (C1=1, C2=2, C3=3; T1=3, T2=5, T3=15; T=D): The Set is FEASIBLE\n\n",((1.0/3.0) + (2.0/5.0) + (3.0/15.0)));
  	Schedule_process(Number_of_services);
	}
    else
  printf("Example-3 U=%4.2f (C1=1, C2=2, C3=3; T1=3, T2=5, T3=15; T=D): The Set is INFEASIBLE\n\n",((1.0/3.0) + (2.0/5.0) + (3.0/15.0)));

	/* Example 4 */
	Number_of_services = sizeof(example4_period)/sizeof(uint32_t);
	Fetch_process(Number_of_services, example4_period, example4_wcet, example4_period);
	Calculate_LCM(Number_of_services);
	f = schedulability(Number_of_services);
	if(f <= 1)
	{
    printf("Example-4 U=%4.2f (C1=1, C2=1, C3=4; T1=2, T2=4, T3=16; T=D): The Set is FEASIBLE \n\n",((1.0/2.0) + (1.0/4.0) + (4.0/16.0)));
    Schedule_process(Number_of_services);
	}
    else
	  printf("Example-4 U=%4.2f (C1=1, C2=1, C3=4; T1=2, T2=4, T3=16; T=D): The Set is INFEASIBLE \n\n",((1.0/2.0) + (1.0/4.0) + (4.0/16.0)));

/* Example 5 */
	Number_of_services = sizeof(example5_period)/sizeof(uint32_t);
	Fetch_process(Number_of_services, example5_period, example5_wcet, example5_period);
	Calculate_LCM(Number_of_services);
	f = schedulability(Number_of_services);
	if(f <= 1)
	{
    printf("Example-5 U=%4.2f (C1=1, C2=2, C3=1; T1=2, T2=5, T3=10; T=D): The Set is FEASIBLE \n\n",((1.0/2.0) + (2.0/5.0) + (1.0/10.0)));
		Schedule_process(Number_of_services);
	}
    else
	    printf("Example-5 U=%4.2f (C1=1, C2=2, C3=1; T1=2, T2=5, T3=10; T=D): The Set is INFEASIBLE \n\n",((1.0/2.0) + (2.0/5.0) + (1.0/10.0)));

/* Example 6 */
	Number_of_services = sizeof(example6_period)/sizeof(uint32_t);
	Fetch_process(Number_of_services, example6_period, example6_wcet, example6_deadline);
	Calculate_LCM(Number_of_services);
	f = schedulability(Number_of_services);
	if(f <= 1)
	{
    printf("Example-6 U=%4.2f (C1=1, C2=1, C3=1, C4=2; T1=2, T2=5, T3=7, T4=13; T=D): The Set is FEASIBLE \n\n",((1.0/2.0) + (1.0/5.0) + (1.0/7.0) + (2.0/13.0)));
    Schedule_process(Number_of_services);
	}
    else
	printf("Example-6 U=%4.2f (C1=1, C2=1, C3=1, C4=2; T1=2, T2=5, T3=7, T4=13; T=D): The Set is INFEASIBLE \n\n",((1.0/2.0) + (1.0/5.0) + (1.0/7.0) + (2.0/13.0)));

/* Example 7 */
	Number_of_services = sizeof(example7_period)/sizeof(uint32_t);
	Fetch_process(Number_of_services, example7_period, example7_wcet, example7_period);
	Calculate_LCM(Number_of_services);
	f = schedulability(Number_of_services);
	if(f <= 1)
	{
   printf("Example-7 U=%4.2f (C1=1, C2=2, C3=4; T1=3, T2=5, T3=15; T=D):The Set is FEASIBLE \n\n", ((1.0/3.0) + (2.0/5.0) + (4.0/15.0)));
		Schedule_process(Number_of_services);
	}
    else
 printf("Example-7 U=%4.2f (C1=1, C2=2, C3=4; T1=3, T2=5, T3=15; T=D):The Set is INFEASIBLE \n\n", ((1.0/3.0) + (2.0/5.0) + (4.0/15.0)));

	/* Example 8 */
	Number_of_services = sizeof(example8_period)/sizeof(uint32_t);
	Fetch_process(Number_of_services, example8_period, example8_wcet, example8_period);
	Calculate_LCM(Number_of_services);
	f = schedulability(Number_of_services);
	if(f <= 1)
	{
  printf("Example-8 U=%4.2f (C1=1, C2=1, C3=1, C4=2; T1=2, T2=5, T3=7, T4=13; T=D): The Set is FEASIBLE \n\n",((1.0/2.0) + (1.0/5.0) + (1.0/7.0) + (2.0/13.0)));
	Schedule_process(Number_of_services);
	}
    else
   printf("Example-8 U=%4.2f (C1=1, C2=1, C3=1, C4=2; T1=2, T2=5, T3=7, T4=13; T=D): The Set is INFEASIBLE \n\n",((1.0/2.0) + (1.0/5.0) + (1.0/7.0) + (2.0/13.0)));

	/* Example 9 */
	Number_of_services = sizeof(example9_period)/sizeof(uint32_t);
	Fetch_process(Number_of_services, example9_period, example9_wcet, example9_period);
	Calculate_LCM(Number_of_services);
	f = schedulability(Number_of_services);
	if(f <= 1)
	{
  printf("Example-9 U=%4.2f (C1=1, C2=2, C3=4, C4=6; T1=6, T2=8, T3=12, T4=24; T=D):The Set is FEASIBLE \n\n",((1.0/6.0) + (2.0/8.0) + (4.0/12.0)+ (6.0/24.0)));
		Schedule_process(Number_of_services);
	}
    else
  printf("Example-9 U=%4.2f (C1=1, C2=2, C3=4, C4=6; T1=6, T2=8, T3=12, T4=24; T=D):The Set is INFEASIBLE \n\n",((1.0/6.0) + (2.0/8.0) + (4.0/12.0)+ (6.0/24.0)));

    // if(completion_time_feasibility(Number_of_services, example0_period, example0_wcet, example0_period) == TRUE)
        // printf("FEASIBLE\n");
    // else
        // printf("INFEASIBLE\n");

	// Fetch_process(Number_of_services);

	// Calculate_LCM();

	// printf("\n\nSchedulability Test: \n);
	// f = schedulability();

	// if(f <= 1)
	// {
		// printf("This Real Time System is schedulable because %f <= 1\n", f);
		// //Schedule_process();
	// }
	// else
	// {
		// printf("This Real Time System is not schedulable because %f > 1\n", f);
		// //exit(0);
	// }
}


