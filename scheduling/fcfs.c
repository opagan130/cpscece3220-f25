
#include <sys/queue.h>
#include <assert.h>
#include <stdio.h>
#include "fcfs.h"

scheduler_t fcfs_scheduler = {
	.name = "FCFS",
	.init = fcfs_init,
	.ready = fcfs_burst_ready,
	.next = fcfs_schedule_next_burst,
	.done = fcfs_burst_done
};

static TAILQ_HEAD(burstQ, entry) ready_queue;

struct entry {
	burst_t* burst; 
	TAILQ_ENTRY(entry) entries;         /* Tail queue. */
};


void fcfs_init()
{
	TAILQ_INIT(&ready_queue);

}

void fcfs_burst_ready(int quantum, burst_t *burst)
{
	struct entry *newentry = malloc(sizeof(struct entry));
	newentry->burst = burst;
	TAILQ_INSERT_TAIL(&ready_queue, newentry, entries);

	//printf("insert tail -> %d : %d\n", quantum, burst->length);
}

burst_t* fcfs_schedule_next_burst(int quantum, 
							burst_t **bursts, 
							int num_bursts)
{
	if (ready_queue.tqh_first== NULL) return NULL; //nothing to schedule

	return ready_queue.tqh_first->burst;
}

void fcfs_burst_done(int quantum, burst_t *burst)
{
	assert (ready_queue.tqh_first->burst == burst); //it's fcfs, so the burst that is done should always be at the front of the queue
	TAILQ_REMOVE(&ready_queue, ready_queue.tqh_first, entries);
}
