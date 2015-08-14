#include "image_inverter.h"
#include <stdio.h>

int array_match(int *real, int *gold, int elems)
{
	int match = 1;
	for(int i=0; i<elems; i++, gold++, real++)
	{
		if(*real != *gold)
		{
			printf("Expected: %X Got: %X\n", *gold, *real);
			match = 0;
		}
	}
	return match;
}

int main()
{
	// Data input
	int in[35];

	// Expected result
	int gold[35];
	unsigned int i;
	for(i=0; i<35; i++)
	{
		in[i] = i;
		gold[i] = ~in[i];
	}

	int elems = 35;
	int *mem=0;
	image_inverter(in, 0, 0, elems);

	if(array_match(in, gold, elems) == 1)
	{
		printf("Test passed\n");
		return 0;
	}
	else
	{
		printf("Test failed\n");
		return 1;
	}
}
