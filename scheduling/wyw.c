
#include <sys/queue.h>
#include <assert.h>
#include <stdio.h>
#include "wyw.h"

scheduler_t wyw_scheduler = {
	.name = "WYW",
	.init = wyw_init,
	.ready = wyw_burst_ready,
	.next = wyw_schedule_next_burst,
	.done = wyw_burst_done
};



//add SJF code down below

void wyw_init()
{
	//initialize any scheduler data structures
}

void wyw_burst_ready(int quantum, burst_t *burst)
{
	//a new burst is ready to schedule
}

burst_t* wyw_schedule_next_burst(int quantum, burst_t **bursts, int num_bursts)
{
	if (num_bursts > 0)
	{
		//whatever...roll the dice
		return bursts[rand()%num_bursts];
	}
	return NULL; 
}

void wyw_burst_done(int quantum, burst_t *burst)
{
	//just letting you know that burst is finished, in case you want
	//to do any cleanup
}
