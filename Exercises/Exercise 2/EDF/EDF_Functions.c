/********************************************
 * *   Date Edited: 02/23/2018
 * *
 * *   File: EDF_Functions.c
 * *
 * *   Description: source file for pthread scheduling code
 * *     -void Fetch_process(uint32_t Number_of_services, uint32_t ex_period[], uint32_t ex_wcet[], uint32_t ex_deadline[]);
 * *     -void Calculate_LCM(uint32_t Number_of_services);
 * *     -float schedulability(uint32_t Number_of_services);
 * *     -void Schedule_process(uint32_t Number_of_services);
 * *
 * *THIS CODE CONTAINS CODE FROM SAM SIEWERT'S EXAMPLE FROM THE DESCRIPTION AND https://www.youtube.com/watch?v=XrwqY205Zfo
 * *
 * * VxWorks code provided were used as references.
 * ********************************************************/
#include "EDF_Functions.h"


void Fetch_process(uint32_t Number_of_services, uint32_t ex_period[], uint32_t ex_wcet[], uint32_t ex_deadline[])
{
	int i;
	for(i=0; i<Number_of_services; i++)
		ProcessesPeriods[i] = ex_period[i];

	for(i=0; i<Number_of_services; i++)
		ProcessesExecutionTime[i] = ex_wcet[i];

	for(i=0; i<Number_of_services; i++)
		ProcessesDeadline[i] = ex_deadline[i];
}

float schedulability(uint32_t Number_of_services)
{
	float Condition = 0;
	float x,y;
  int i=0;
	for(i=0; i<Number_of_services; i++)
	{
		x = ProcessesExecutionTime[i];
		y = ProcessesPeriods[i];
		Condition = Condition + (x/y);
	}
	return Condition;
}

/*Function to Calculate the LCM of all the Period*/
void Calculate_LCM(uint32_t Number_of_services)
{
	int help[1000];
	int i=0;
	for(i=0; i<Number_of_services; i++)
	{
		help[i] = ProcessesPeriods[i];
	}
	int h=0;
	int theLowestValueInHelp;
	int theLowestValueInIndex;
	while(h == 0)
	{
		h=1;
		theLowestValueInHelp = help[0];
		theLowestValueInIndex = 0;

		for(i=0; i<Number_of_services; i++)
		{
			if(theLowestValueInHelp != help[i])
			{
				h=0;
				if(theLowestValueInHelp > help[i])
				{
					theLowestValueInHelp = help[i];
					theLowestValueInIndex = i;
				}
			}
		}
		if(h == 0)
		{
			help[theLowestValueInIndex] = theLowestValueInHelp + ProcessesPeriods[theLowestValueInIndex];
		}
	}
	PeriodsLCM = help[0];
}

void Schedule_process(uint32_t Number_of_services)
{
	int ED;
	int EDIndex;
	int schedulingTable[PeriodsLCM];
	int Remaining_capacity[1000];
	int nextDeadline[1000];
	int processOnlineNewPeriod[1000];
	int i=0,j=0;
	for(i=0; i<Number_of_services; i++)
	{
		nextDeadline[i] = ProcessesDeadline[i];
		Remaining_capacity[i] = ProcessesExecutionTime[i];
		processOnlineNewPeriod[i] = 0;
	}

	//output to filelength

	for(i=0; i<PeriodsLCM; i++)
	{
		//getting the earliest deadline
		ED = PeriodsLCM;
		EDIndex = -1;
		for(j=0; j<Number_of_services; j++)
		{
			if(Remaining_capacity[j] > 0)
			{
				if(ED > nextDeadline[j])
				{
					ED = nextDeadline[j];
					EDIndex = j;
				}
			}
		}
		Remaining_capacity[EDIndex]--;

		//get the next deadline distance
		for(j=0; j<Number_of_services; j++)
		{
			if(processOnlineNewPeriod[j] == (ProcessesPeriods[j] - 1))
			{
				nextDeadline[j] = ProcessesDeadline[j];
				Remaining_capacity[j] = ProcessesExecutionTime[j];
				processOnlineNewPeriod[j] = 0;
			}
			else
			{
				if(nextDeadline[j] > 0)
				{
					nextDeadline[j]--;
				}
				else
				{
					if(Remaining_capacity[j] > 0)
					{
						printf("Process cannot be completed\n");
					}
				}
				processOnlineNewPeriod[j]++;
			}
		}
	}
}
