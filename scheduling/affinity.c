#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

int main()
{
	pid_t p = getpid();
	cpu_set_t mask;
	uint8_t *bytes = (uint8_t *)&mask;

	sched_getaffinity(p, sizeof(mask), &mask);

	printf("mask is...\n");
	for (int i=0; i < sizeof(mask); i++)
	{
		printf("%02X",bytes[i]);
	}
	printf("\n");
	
}