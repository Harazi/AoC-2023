#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

inline void setnum(char num, char *first, char *last) {
	if (*first)
		*last = num;
	else
		*first = num;
}

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
		for (u_short i = 0; lineptr[i] != 0 && lineptr[i] != '\n'; i++) {
			if (lineptr[i] >= 48 && lineptr[i] <= 57) setnum(lineptr[i], &first, &last);
			else if (strstr(lineptr+i, "one"  ) == lineptr+i) setnum(1+48, &first, &last);
			else if (strstr(lineptr+i, "two"  ) == lineptr+i) setnum(2+48, &first, &last);
			else if (strstr(lineptr+i, "three") == lineptr+i) setnum(3+48, &first, &last);
			else if (strstr(lineptr+i, "four" ) == lineptr+i) setnum(4+48, &first, &last);
			else if (strstr(lineptr+i, "five" ) == lineptr+i) setnum(5+48, &first, &last);
			else if (strstr(lineptr+i, "six"  ) == lineptr+i) setnum(6+48, &first, &last);
			else if (strstr(lineptr+i, "seven") == lineptr+i) setnum(7+48, &first, &last);
			else if (strstr(lineptr+i, "eight") == lineptr+i) setnum(8+48, &first, &last);
			else if (strstr(lineptr+i, "nine" ) == lineptr+i) setnum(9+48, &first, &last);
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
