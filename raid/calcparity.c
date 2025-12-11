#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define EVEN_PARITY
#define NUM_IMGS 4

int bytes = -1;

uint8_t *getimg(char *fname)
{
	int fd = open(fname,O_RDWR, S_IRUSR);
	struct stat sb;

	if (fstat(fd,&sb) == -1)
	{
		perror("couldn't get file size.\n");
	} 
	if (bytes != -1 && bytes != sb.st_size)
	{	
		perror("img sizes don't match.");
	}
	if (bytes == -1) {
		bytes = sb.st_size;
	}
	return mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
}

int main(int argc, char **argv)
{
	if (argc != NUM_IMGS+1)
	{
		printf("Usage: %s <img1> <img2> <img3> <img4>\n", argv[0]);
		exit(2);
	}

	uint8_t *imgs[NUM_IMGS];
	for (int i=0; i < NUM_IMGS; i++) {
		imgs[i] = getimg(argv[i+1]);
	}

	FILE *fpe = fopen("parity_even.img", "wb");
	FILE *fpo = fopen("parity_odd.img", "wb");
	for (int i=0; i < bytes; i++)
	{
		uint8_t parity_byte = 0;
		for (int j=0; j < NUM_IMGS; j++)
		{
			parity_byte = parity_byte ^ imgs[j][i];
		}
		fwrite(&parity_byte, 1, 1, fpe);
		uint8_t parity_byte_odd =~parity_byte;
		fwrite(&parity_byte_odd, 1, 1, fpo);
	}
	fclose(fpe);
	fclose(fpo);
}