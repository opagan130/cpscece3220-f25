
#include <sys/queue.h>
#include <assert.h>
#include <stdio.h>
#include "sjf.h"

scheduler_t sjf_scheduler = {
	.name = "SJF",
	.init = sjf_init,
	.ready = sjf_burst_ready,
	.next = sjf_schedule_next_burst,
	.done = sjf_burst_done
};



//add SJF code down below

void sjf_init()
{
	//initialize any scheduler data structures
}

void sjf_burst_ready(int quantum, burst_t *burst)
{
	//a new burst is ready to schedule
}

burst_t* sjf_schedule_next_burst(int quantum, burst_t **bursts, int num_bursts)
{
	
}

void sjf_burst_done(int quantum, burst_t *burst)
{
	//just letting you know that burst is finished, in case you want
	//to do any cleanup
}

