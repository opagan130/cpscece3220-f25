#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "schedsim.h"
#include <assert.h>

#define INITIAL_SIZE 1
#define SIZE_INCREMENT 1
#define MAX_NULLS 100



static scheduler_t **schedulers = NULL;
int num_schedulers = 0;

void allocate_schedulers(int number)
{
	//printf("a_s(%d) -> (%zu)\n",number,sizeof(scheduler_t*)*number);
	schedulers = realloc(schedulers, sizeof(scheduler_t*)*number);
	if (num_schedulers < number)
	{
		for (int i=num_schedulers; i < number; i++)
		{
			schedulers[i] = NULL;
		}
	}

	num_schedulers = number;
}

void sim_init(){
	allocate_schedulers(INITIAL_SIZE);
	srand(time(NULL));
}

int sim_add_scheduler(scheduler_t *scheduler)
{
	assert(schedulers != NULL);

	int i;
	for (i=0; i < num_schedulers; i++)
	{
		if (schedulers[i] == NULL) break;
	}
	if (i >= num_schedulers || schedulers[i] != NULL)
	{
		allocate_schedulers(num_schedulers+SIZE_INCREMENT);
	}
	printf("Adding scheduler(%s) at index %d\n",scheduler->name, i);
	schedulers[i] = scheduler;
	return 0;
}

burst_t* load_burst_list(char *input_filename, int* num_bursts)
{
	FILE* fp = fopen(input_filename, "r");
	if (fp == NULL) return NULL;

	burst_t* bursts = NULL;
	*num_bursts = 0;

	char *line = NULL;
	size_t len;

	while (getline(&line, &len, fp) != -1) {
        //printf("Retrieved line of length %zu :\n", len);
        //printf("%s", line);

		if (line[0] == '#') continue;

        char *saveptr = NULL;
        const char* delim = ",\t\n ";
        char *s_burst_time = strtok_r(line, delim, &saveptr);
        char *s_burst_length = strtok_r(NULL, delim, &saveptr);
        char *s_burst_priority = strtok_r(NULL, delim, &saveptr);

        int burst_time = atoi(s_burst_time);
        int burst_length = atoi(s_burst_length);
        int burst_priority = atoi(s_burst_priority);

        //make room for one more
        size_t newsize = sizeof(burst_t)*(*num_bursts+1);
        //printf("pre (p=%p) newsize=%zu\n",bursts,newsize);
        bursts = realloc(bursts, newsize);
        //printf("post (p=%p) newsize=%zu\n",bursts,newsize);
        
        bursts[*num_bursts].arrived = burst_time;
        bursts[*num_bursts].length = burst_length;
        bursts[*num_bursts].priority = burst_priority;
        bursts[*num_bursts].remaining = burst_length; //it's all remaining
    
    	//printf("%d, %d, %d\n",burst_time, burst_length, burst_priority);
        (*num_bursts)++;

        free(line);
        line = NULL;
    }
    return bursts;
}

int is_sim_done(burst_t* bursts, int num_bursts, long sim_time)
{
	for (int i=0; i < num_bursts; i++)
	{
		if (bursts[i].remaining > 0) return FALSE;
	}
	return TRUE;
}

int sim_run(char *input_filename)
{
	int num_bursts;
	burst_t *original_burst_list = load_burst_list(input_filename, &num_bursts);

	printf("Loaded %d bursts from \"%s\"\n",num_bursts, input_filename);

	for (int i=0; i < num_schedulers; i++)
	{
		int nulls_in_a_row = 0;

		//simulation is destructive, so make a copy of the burst list
		burst_t* burst_list = malloc(sizeof(burst_t)*num_bursts);
		memcpy(burst_list, original_burst_list, sizeof(burst_t)*num_bursts);

		//for each scheduler
		assert(schedulers[i] != NULL);

		printf("Running, scheduler -- %s\n",schedulers[i]->name);
		
		long sum_turn = 0;
		long count_turn = 0;

		long sum_wait = 0;
		long count_wait = 0;
		


		//initialize the scheduler
		schedulers[i]->init();

		//open file for csv output
		char raw_output_filename[1024];
		sprintf(raw_output_filename,"%s.csv",schedulers[i]->name);
		FILE* raw_output = fopen(raw_output_filename,"w");
		fprintf(raw_output, "Arrived,Finished,Turnaround,Wait\n");

		long quantum = 0;
		while (!is_sim_done(burst_list, num_bursts, quantum))
		{
			burst_t **in_flight = NULL;
			int num_in_flight = 0;

			//announce any arrivals and collect bursts that are in-flight
			for (int b=0; b < num_bursts; b++)
			{
				if (burst_list[b].arrived == quantum) schedulers[i]->ready(quantum, &(burst_list[b]));
				if (burst_list[b].arrived <= quantum && burst_list[b].remaining > 0)
				{
					num_in_flight++;
					in_flight = realloc(in_flight, sizeof(burst_t*)*num_in_flight);
					in_flight[num_in_flight-1] = &(burst_list[b]);
				}
			}

			burst_t* next_burst = schedulers[i]->next(quantum, in_flight, num_in_flight);
			quantum++;
			if (next_burst != NULL)
			{
				nulls_in_a_row = 0;
				next_burst->remaining--;
				if (next_burst->remaining <= 0)
				{
					schedulers[i]->done(quantum, next_burst);
					int turn = quantum-next_burst->arrived;
					int wait = turn - next_burst->length;
					fprintf(raw_output, "%d,%ld,%d,%d\n",next_burst->arrived,quantum,turn,wait);

					sum_turn += turn;
					count_turn++;

					sum_wait += wait;
					count_wait++;

					//printf("\t%zu -> len(%d) turn (%d), wait (%d)\n", quantum, next_burst->length, turn, wait);
				}
			} else {
				nulls_in_a_row++;
				if (nulls_in_a_row > MAX_NULLS)
				{
					printf("\tError: More than %d unused quanta in a row...I'm giving up on this scheduler.\n",nulls_in_a_row);
					break;
				}
			}
			

		}
		double mean_turn = ((double)sum_turn)/((double)count_turn);
		double mean_wait = ((double)sum_wait)/((double)count_wait);
		
		printf("\tscheduler -- %s -- done.\n\t(%lu quanta, %.2lf avg turn, %.2lf avg wait)\n",schedulers[i]->name, quantum, mean_turn, mean_wait);
		fclose(raw_output);

	}
	return 0;
}


