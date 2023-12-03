#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(void)
{
	char *inputpath = "./input";
	FILE *input = fopen(inputpath, "r");
	if (input == NULL) {
		perror(inputpath);
		return errno;
	}

	u_int sum = 0;
	int ascii[128];
	char *lineptr = malloc(256);
	size_t linesize = 256;
	while(getline(&lineptr, &linesize, input) != -1) {
		char *line = strchr(lineptr, ':') + 1;
		line[-1] = 0;
		char *sets[9];
		u_short setsize = 0;
		char *set;
		ascii['r'] = 0;
		ascii['g'] = 0;
		ascii['b'] = 0;

		while ((set = strsep(&line, ";")) != NULL) {
			sets[setsize++] = set;
		}

		for (u_short i = 0; i < setsize; i++) {
			while (sets[i] != NULL) {
				int count = atoi(sets[i]);
				while (sets[i][0] >= 48 && sets[i][0] <= 57 || sets[i][0] == ' ') sets[i]++;
				if (count > ascii[sets[i][0]]) ascii[sets[i][0]] = count;
				strsep(&sets[i], ",");
			}
		}

		sum += ascii['r'] * ascii['g'] * ascii['b'];
	}
	if (errno) {
		perror(inputpath);
		return errno;
	}

	printf("%d\n", sum);

	free(lineptr);
	fclose(input);
	return EXIT_SUCCESS;
}
