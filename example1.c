#include <stdio.h>

/*
The following is my implementation of the core part of a base64
encoder, which is used in MIME email formats for binary attachments,
and used in other protocols.

The base64 specification calls for taking 3 bytes of 8-bit data, and
converting it into 4 bytes of 6-bit data, and using each 6-bit value
as an index into an array of printable characters.  The reason it is 
called base64 encoding is because the number 64 is the largest number 
that 6 bits will give you.

Of course, using this method of encoding on a file means your files 
expand in size since each encoded byte has 2 bits of unused "dead space"
because the minimum data length of a CPU is 8 bits.

In my implementation below, the main loop simply walks an input array of
8-bit data, taking batches of 3 bytes as input and making 4 bytes output, 
which is how the spec is defined.

Normally you're supposed to handle cases at the end, for when your input
data isn't divisible evenly by 3 bytes and you have leftovers, so in that case
you are supposed to represent those using the "=" character, at the very end.

For this example, I didn't bother, and didn't bother with the final step of
using a the lookup table of printable ASCII characters.  There is a standard one
everyone uses.  This just shows the core part of converting the data.

At the end of this C file there is sample output.

*/

/* function declaration to console print a byte array in "1" and "0" format */
void print_bits(const unsigned char* bytes, int len);

int main()
{
	/* First, declare a small array of 8-bit bytes as our test data
	to convert into "6-bit" bytes, to demonstrate how base64
	encoding works.  I declared them as hex values.  */

	const unsigned char bytes[6] = { 0xA3, 0x05, 0x99, 0xCD, 0x81, 0x0F };
	int len = sizeof(bytes);

	/* console print input bytes as "1"s and "0"s for reference later */

	print_bits(bytes, 6);
	printf("\n");

	/* Walk the bytes of the input array in batches of 3 and convert them
	into 4 6-bit values.  This may appear 'complicated' but it's actually
	pretty simple.	It's just filtering the bits you want, and moving them 
	around. */

	for (int i = 0; i < len; i += 3)
	{
		/* First, start by getting 6 bits from the the 1st array element,
		beginning from the high bits, and shift them to the right
		by 2 bits to get them into proper position. */

		unsigned char c1 = (bytes[i] & 0xFC) >> 2;

		/* Next, get the two remaining bits from the 1st element,
		and the first 4 bits from the 2nd array element (starting
		from the high side again), and shift them both into position */

		unsigned char c2 = ((bytes[i] & 0x03) << 4) | ((bytes[i + 1] & 0xF0) >> 4);

		/* Next, get the next 4 bits from the 2nd array element,
		and the first 2 bits from the 3rd array element, and shift
		them into position */

		unsigned char c3 = ((bytes[i + 1] & 0x0F) << 2) | ((bytes[i + 2] & 0xC0) >> 6);

		/* Finally, get the last 6 bits from the 3rd array element,
		and we're done. No need to shift it, it's already positioned correctly. */

		unsigned char c4 = (bytes[i + 2] & 0x3F);

		/* At this point, you would use the 4 new values as index pointers
		into a lookup table of printable ASCII characters, to build a
		string buffer which is the final data, but for this example I did not
		include it.
		For this, just print the output bits. */

		unsigned char tmp[4] = { c1, c2, c3, c4 };
		print_bits(tmp, 4);

		/* End of loop -- around to the next 3 bytes of the test data */
	}

	/* all done */
	printf("\n");

	return 0;
}

/* Bit-printing helper function */
void print_bits(const unsigned char* bytes, int len)
{
	/* print bytes as regular numbers first */

	for (int j = 0; j < len; j++) {
		printf("%d ", bytes[j]);
	}
	printf("\n");

	/* print the bytes as "1" and "0" values for easy reading */

	for (int j = 0; j < len; j++) {
		for (int i = 7; 0 <= i; i--) {
			printf("%c", (bytes[j] & (1 << i)) ? '1' : '0');
		}

		if ((j + 1) < len) {
			printf("-"); }
	}

	printf("\n\n");
}

/*
Test results.  First is using an array entirely composed of 0xFF bytes,
and the before-and-after conversion.  First showing in integer
representation and the bits.

Original input data with 6, 8-bit bytes of 0xFF  (as ints, and as bits):
--------------------------------------
255 255 255 255 255 255
11111111-11111111-11111111-11111111-11111111-11111111

After base64 transformation to 8, 6-bit bytes, this shows how the
bits are re-arranged to 6-bit values.
------------------------------------------------------------
63 63 63 63 63 63 63 63
00111111-00111111-00111111-00111111-00111111-00111111-00111111-00111111

A second test, mixture of values:

Original input data
---------------------
163 5 153 205 129 15
10100011-00000101-10011001-11001101-10000001-00001111

After encoding
-----------------
40 48 22 25 51 24 4 15
00101000-00110000-00010110-00011001-00110011-00011000-00000100-00001111

*/

