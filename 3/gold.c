#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

static int getpartnum(const char line[], int i)
{
	for (; i >= 0 && isdigit(line[i]); i--);
	return atoi(&line[i+1]);
}

static int processheader(char *firstline, char *secondline)
{
	size_t sum = 0;
	int gearpartnums[2][3];
	int validgearpartnums[2*3];
	int validgearpartindex = 0;
	for (int i = 0; !(firstline[i] == '\0' || firstline[i] == '\n'); i++) {
		if (firstline[i] != '*')
			continue;

		validgearpartindex = 0;
		memset(gearpartnums, 0, sizeof(gearpartnums));
		memset(validgearpartnums, 0, sizeof(validgearpartnums));

		// Started processing, check one character before i
		if (i) {
			if (isdigit(firstline[i-1]))
				gearpartnums[0][0] = getpartnum(firstline, i-1);
			if (isdigit(secondline[i-1]))
				gearpartnums[1][0] = getpartnum(secondline, i-1);
		}

		if (isdigit(secondline[i]))
			gearpartnums[1][1] = gearpartnums[1][0] ? -1 : getpartnum(secondline, i);

		if (isdigit(firstline[i+1]))
			gearpartnums[0][2] = gearpartnums[0][1] ? -1 : getpartnum(firstline, i+1);
		if (isdigit(secondline[i+1]))
			gearpartnums[1][2] = gearpartnums[1][1] ? -1 : getpartnum(secondline, i+1);

		for (int y = 0; y < 2; y++)
			for (int z = 0; z < 3; z++)
				if (gearpartnums[y][z] > 0)
					validgearpartnums[validgearpartindex++] = gearpartnums[y][z];

		// Unknown edge case
		if (validgearpartindex > 2) {
			printf("hit edge case at line %s\n", firstline);
			continue;
		}

		// No need to check if validgearpartindex < 2
		sum += validgearpartnums[0] * validgearpartnums[1];
	}
	return sum;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s FILE\n", argv[0]);
		return EXIT_FAILURE;
	}

	char *inputpath = argv[1];
	FILE *input;
	if (inputpath[0] == '-' && inputpath[1] == '\0') {
		input = stdin;
	} else {
		input = fopen(inputpath, "r");
		if (input == NULL) {
			perror(inputpath);
			return errno;
		}
	}

	size_t sum = 0;
	size_t linessize[] = { 0, 0, 0 };
	char *linesbuf[] = { NULL, NULL, NULL };

	getline(&linesbuf[0], &linessize[0], input);
	getline(&linesbuf[1], &linessize[1], input);

	sum += processheader(linesbuf[0], linesbuf[1]);

	getline(&linesbuf[2], &linessize[2], input);

	while (true) {
		int gearpartnums[3][3];
		int validgearpartnums[3*3];
		int validgearpartindex = 0;
		for (int i = 0; !(linesbuf[1][i] == '\0' || linesbuf[1][i] == '\n'); i++) {
			if (linesbuf[1][i] != '*')
				continue;

			validgearpartindex = 0;
			memset(gearpartnums, 0, sizeof(gearpartnums));
			memset(validgearpartnums, 0, sizeof(validgearpartnums));

			// Started processing, check one character before i
			if (i) {
				if (isdigit(linesbuf[0][i-1]))
					gearpartnums[0][0] = getpartnum(linesbuf[0], i-1);
				if (isdigit(linesbuf[1][i-1]))
					gearpartnums[1][0] = getpartnum(linesbuf[1], i-1);
				if (isdigit(linesbuf[2][i-1]))
					gearpartnums[2][0] = getpartnum(linesbuf[2], i-1);
			}

			if (isdigit(linesbuf[0][i]))
				gearpartnums[0][1] = gearpartnums[0][0] ? -1 : getpartnum(linesbuf[0], i);
			if (isdigit(linesbuf[2][i]))
				gearpartnums[2][1] = gearpartnums[2][0] ? -1 : getpartnum(linesbuf[2], i);

			if (isdigit(linesbuf[0][i+1]))
				gearpartnums[0][2] = gearpartnums[0][1] ? -1 : getpartnum(linesbuf[0], i+1);
			if (isdigit(linesbuf[1][i+1]))
				gearpartnums[1][2] = gearpartnums[1][1] ? -1 : getpartnum(linesbuf[1], i+1);
			if (isdigit(linesbuf[2][i+1]))
				gearpartnums[2][2] = gearpartnums[2][1] ? -1 : getpartnum(linesbuf[2], i+1);

			for (int y = 0; y < 3; y++)
				for (int z = 0; z < 3; z++)
					if (gearpartnums[y][z] > 0)
						validgearpartnums[validgearpartindex++] = gearpartnums[y][z];

			// Unknown edge case
			if (validgearpartindex > 2) {
				printf("hit edge case at line %s\n", linesbuf[1]);
				continue;
			}

			// No need to check if validgearpartindex < 2
			sum += validgearpartnums[0] * validgearpartnums[1];
		}

		strcpy(linesbuf[0], linesbuf[1]);
		strcpy(linesbuf[1], linesbuf[2]);
		size_t r = getline(&linesbuf[2], &linessize[2], input);
		if (r == -1) {
			break;
		}
	}

	sum += processheader(linesbuf[1], linesbuf[0]); // Reversed intentionally
	printf("%zu\n", sum);

	fclose(input);
	return EXIT_SUCCESS;
}
