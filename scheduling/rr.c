
#include <sys/queue.h>
#include <assert.h>
#include <stdio.h>
#include "rr.h"

scheduler_t rr_scheduler = {
	.name = "RR",
	.init = rr_init,
	.ready = rr_burst_ready,
	.next = rr_schedule_next_burst,
	.done = rr_burst_done
};

static TAILQ_HEAD(burstQ, entry) ready_queue;

struct entry {
	burst_t* burst; 
	TAILQ_ENTRY(entry) entries;         /* Tail queue. */
};


void rr_init()
{
	TAILQ_INIT(&ready_queue);

}

void rr_burst_ready(int quantum, burst_t *burst)
{
	struct entry *newentry = malloc(sizeof(struct entry));
	newentry->burst = burst;
	TAILQ_INSERT_TAIL(&ready_queue, newentry, entries);

	//printf("insert tail -> %d : %d\n", quantum, burst->length);
}

burst_t* rr_schedule_next_burst(int quantum, burst_t **bursts, int num_bursts)
{
	if (ready_queue.tqh_first== NULL) return NULL; //nothing to schedule

	struct entry *head = ready_queue.tqh_first;
	//move the head to the tail, and return the next head
	TAILQ_REMOVE(&ready_queue, ready_queue.tqh_first, entries);
	TAILQ_INSERT_TAIL(&ready_queue, head, entries);

	return ready_queue.tqh_first->burst;
}

void rr_burst_done(int quantum, burst_t *burst)
{
	assert (ready_queue.tqh_first->burst == burst); //it's rr, so the burst that is done should always be at the front of the queue
	TAILQ_REMOVE(&ready_queue, ready_queue.tqh_first, entries);
}
