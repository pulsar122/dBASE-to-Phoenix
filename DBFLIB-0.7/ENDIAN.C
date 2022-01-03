/*
 * test for little/big endian system type
 * rasca, berlin 1996
 */

#include <stdio.h>

int main(void) {
	short int w=0x0001;

	if (*((char *)&w)) {
		printf("\nbyte order     : LSB...MSB (little endian)\n");
	} else {
		printf("\nbyte order     : MSB...LSB (big endian)\n");
	}
	printf ("size of pointer: %d\n", sizeof (void*));
	printf ("size of 'int'  : %d\n\n", sizeof(int));
	return 0;
}

