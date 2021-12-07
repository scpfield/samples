/*

This is my implementation of a programming question I got during
an interview.  I did not provide a complete solution during the 
interview itself, because I got a little bit of brain-freeze, but it only 
took me a few minutes afterwards to finish it.

The question was, given a series of numbers increasing by 1,
but has gaps, detect and print the ranges of continuously-increasing
numbers, and the gaps.

For example, given: 1 2 3 4 5 10 20 21 22 23

Output:  1-5, 10, 20, 21-23

My simple approach is to walk the array of input numbers, using 
two pointers to track the start + end ranges of sequences of numbers 
that increase by 1.  

When a gap is found, then adjust the start pointer for the new sequence.  
It is similar to a C++ iterator concept, using memory addresses to refer 
to the underlying data.

At the end is sample output.

*/

#include <stdio.h>

int main()
{
	/* Test input array of integer numbers, with gaps */

	int numbers[] = {1,2,3,4,5,10,15,16,17,18,19,21,23,25,26,27,28,29,50,51};

	/* int pointer variables */

	int* start_seq = &numbers[0];
	int* end_seq = start_seq + 1;
	int len = (int) sizeof(numbers) / sizeof(int);

	/* Walk the input data using the pointers, referring by memory addresses */

	while (end_seq <= &numbers[len])
	{
		/* Look backwards to see if the numbers are increasing by 1, by
		dereferencing the pointers to get their actual values. */

		if ( (*end_seq - *(end_seq - 1) == 1))
		{
			/* If they are continuously increasing by 1, then move the 
			end-sequence pointer forward to the next memory address. */

			end_seq += 1;
		}
		else
		{
			/* If not, then we encountered a gap, so then deference the 
			pointers to show the start + end values of the sequence. */

			printf("%d through %d\n", *start_seq, *(end_seq-1));

			/* Reset the pointers for the next sequence. */

			start_seq = end_seq;
			end_seq = start_seq + 1;
		}

		/* Loop around until we're done */
	}

	return 0;

}

/* OUTPUT:

1 through 5
10 through 10
15 through 19
21 through 21
23 through 23
25 through 29
50 through 51

*/