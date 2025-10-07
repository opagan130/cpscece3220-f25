#include <stdio.h>
#include <unistd.h>

#define BIG 100000000L
#define DIVIDER 500


void do_busy_things(int niceval)
{
	nice(niceval); //set the nice value for the process

	for (long i=0; i < BIG; i++)
	{
		if (i % (BIG/DIVIDER) == 0)
		{
			printf("nice(%d) --> %ld\n",niceval,i);
		}
	}
}

int main()
{

	if (fork() == 0)
	{
		do_busy_things(0);
	} else {
		do_busy_things(19);
	}
}
