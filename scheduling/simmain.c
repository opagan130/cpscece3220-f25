#include <stdio.h>

#include "schedsim.h"
#include "fcfs.h"
#include "wyw.h"
#include "rr.h"
#include "priority.h"
#include "sjf.h"



int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Usage: %s <filename>\n",argv[0]);
		exit(1);
	}

	sim_init();

	sim_add_scheduler(&fcfs_scheduler);
	sim_add_scheduler(&rr_scheduler);
	sim_add_scheduler(&wyw_scheduler);
	//sim_add_scheduler(&pr_scheduler);
	//sim_add_scheduler(&sjf_scheduler);

	sim_run(argv[1]);

}