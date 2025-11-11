
#include <sys/queue.h>
#include <assert.h>
#include <stdio.h>
#include "priority.h"

scheduler_t pr_scheduler = {
	.name = "PRIORITY",
	.init = pr_init,
	.ready = pr_burst_ready,
	.next = pr_schedule_next_burst,
	.done = pr_burst_done
};



void pr_init()
{
	//nothing to do
}

void pr_burst_ready(int quantum, burst_t *burst)
{
	//nothing to do here, either
}

burst_t* pr_schedule_next_burst(int quantum, 
								burst_t **bursts, 
								int num_bursts)
{
	int max_priority = -1;
	int max_priority_idx = -1;

	for (int i=0; i < num_bursts; i++)
	{
		if (bursts[i]->priority > max_priority)
		{
			max_priority_idx = i;
			max_priority = bursts[i]->priority;
		}
	}
	if (max_priority < 0 || max_priority_idx < 0)
		return NULL;

	return bursts[max_priority_idx];
}

void pr_burst_done(int quantum, burst_t *burst)
{
	//nothing to do here.
}
