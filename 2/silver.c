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
	char *lineptr = malloc(256);
	size_t linesize = 256;
	while(getline(&lineptr, &linesize, input) != -1) {
		char *line = strchr(lineptr, ':') + 1;
		line[-1] = 0;
		char *gameID = lineptr+5;
		char *sets[9];
		u_short setsize = 0;
		char *set;

		while ((set = strsep(&line, ";")) != NULL) {
			sets[setsize++] = set;
		}

		for (u_short i = 0; i < setsize; i++) {
			while (sets[i] != NULL) {
				u_short limit = 0;
				int count = atoi(sets[i]);
				while (sets[i][0] >= 48 && sets[i][0] <= 57 || sets[i][0] == ' ') sets[i]++;
				switch (sets[i][0]) {
					case 'r':
						limit = 12;
					break;
					case 'g':
						limit = 13;
					break;
					case 'b':
						limit = 14;
					break;
					default:
						fprintf(stderr, "Unknown cube color: %s", sets[i]);
						return EXIT_FAILURE;
				}
				if (count > limit) {
					setsize = 0;
					break;
				}
				strsep(&sets[i], ",");
			}
		}

		if (setsize){
			sum += atoi(gameID);
		}
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
