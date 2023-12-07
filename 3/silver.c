#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

bool issym(char x) {
	return !isdigit(x) && x != '.';
}

int processheader(char *firstline, char *secondline)
{
	size_t sum = 0;
	int processing = 0;
	bool summed = false;
	for (int i = 0; !(firstline[i] == '\0' || firstline[i] == '\n'); i++) {
		if (!processing) {
			if (isdigit(firstline[i]))
				processing = atoi(&firstline[i]);
			else
				continue;

			// Started processing, check one character before i
			if (i && (issym(firstline[i-1]) || issym(secondline[i-1]))) {
				sum += processing;
				summed = true;
				continue;
			}
		}

		if (!isdigit(firstline[i])) {
			if (!summed) {
				if (issym(firstline[i]) || issym(secondline[i])) {
					sum += processing;
				}
			}
			processing = 0;
			summed = false;
			continue;
		}

		if (!summed && issym(secondline[i])) {
			sum += processing;
			summed = true;
		}

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
		int processing = 0;
		bool summed = false;
		for (int i = 0; !(linesbuf[1][i] == '\0' || linesbuf[1][i] == '\n'); i++) {
			if (!processing) {
				if (isdigit(linesbuf[1][i]))
					processing = atoi(&linesbuf[1][i]);
				else
					continue;

				// Started processing, check one character before i
				if (i && (issym(linesbuf[0][i-1]) || issym(linesbuf[1][i-1]) || issym(linesbuf[2][i-1]))) {
					sum += processing;
					summed = true;
					continue;
				}
			}

			if (!isdigit(linesbuf[1][i])) {
				if (!summed) {
					if (issym(linesbuf[0][i]) || issym(linesbuf[1][i]) || issym(linesbuf[2][i])) {
						sum += processing;
					}
				}
				processing = 0;
				summed = false;
				continue;
			}

			if (!summed && (issym(linesbuf[0][i]) || issym(linesbuf[2][i]))) {
				sum += processing;
				summed = true;
			}

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
