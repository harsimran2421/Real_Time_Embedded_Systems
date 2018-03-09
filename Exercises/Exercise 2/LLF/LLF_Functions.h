/********************************************
 * *   Date Edited: 02/23/2018
 * *
 * *   File: EDF_Functions.h
 * *
 * *   Description: Header file for pthread scheduling code
 * *     -void Fetch_process(uint32_t Number_of_services, uint32_t ex_period[], uint32_t ex_wcet[], uint32_t ex_deadline[]);
 * *     -void Calculate_LCM(uint32_t Number_of_services);
 * *     -float schedulability(uint32_t Number_of_services);
 * *     -void Schedule_process(uint32_t Number_of_services);
 * *
 * *THIS CODE CONTAINS CODE FROM SAM SIEWERT'S EXAMPLE FROM THE DESCRIPTION AND https://www.youtube.com/watch?v=XrwqY205Zfo
 * *
 * * VxWorks code provided were used as references.
 * ********************************************************/

#include <math.h>
#include<string.h>
#include<stdlib.h>
#include <stdio.h>
#include<stdint.h>

int ProcessesExecutionTime[10];
int ProcessesDeadline[10];
int ProcessesPeriods[10];
int PeriodsLCM;


void Fetch_process(uint32_t Number_of_services, uint32_t ex_period[], uint32_t ex_wcet[], uint32_t ex_deadline[]);

//Calculate the LCM of the set of Services
void Calculate_LCM(uint32_t Number_of_services);

//Check the Utilization
float schedulability(uint32_t Number_of_services);

//Check whether the given set of services is schedulable over LCM
void Schedule_process(uint32_t Number_of_services);
