#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(void)
{
	char *inputpath = "./input.txt";
	FILE *input = fopen(inputpath, "r");
	if (input == NULL) {
		perror(inputpath);
		return errno;
	}

	u_int sum = 0;
	char *lineptr = malloc(128);
	size_t linesize = 128;
	while(getline(&lineptr, &linesize, input) != -1) {
		char first = 0;
		char last = 0;
		for (ushort i = 0; lineptr[i] != 0 && lineptr[i] != '\n'; i++) {
			if (lineptr[i] >= 48 && lineptr[i] <= 57) {
				if (first)
					last = lineptr[i];
				else
					first = lineptr[i];
			}
		}
		char digits[3];
		digits[0] = first;
		if (last)
			digits[1] = last;
		else
			digits[1] = first;
		digits[2] = 0;

		sum = sum + atoi(digits);
	}
	if (errno) {
		perror(inputpath);
		return errno;
	}

	printf("%u\n", sum);

	free(lineptr);
	fclose(input);
	return EXIT_SUCCESS;
}
