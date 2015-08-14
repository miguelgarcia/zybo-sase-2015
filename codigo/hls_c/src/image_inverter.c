#include "image_inverter.h"
#include <stdio.h>
#include <string.h>

// BUFFER in Words
#define BUF_SIZE 16

void image_inverter(volatile int mem[1024*1024*1024*4], unsigned int ptrInput, unsigned int ptrOutput, unsigned int pixelsN)
{
	int i, j;
	unsigned int wordInput = ptrInput / 4;
	unsigned int wordOutput = ptrOutput / 4;
	int buf[BUF_SIZE];
	for(i=0; i < ((int) pixelsN)-BUF_SIZE; i+=BUF_SIZE)
	{
		memcpy(buf, (const void *) &mem[wordInput+ i], BUF_SIZE * sizeof(int));
		for(j=0;j<BUF_SIZE;j++)
		{
			buf[j] = ~buf[j];
		}
		memcpy((void *) &mem[wordOutput+ i], buf, BUF_SIZE * sizeof(int));
	}
	for(; i < pixelsN; i++)
	{
		mem[wordOutput+i] = ~mem[wordInput+i];
	}

}
