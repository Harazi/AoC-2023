#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int readlist(char **line, size_t *linesize, FILE *input, int list[]) {
	if (getline(line, linesize, input) == -1) {
		fprintf(stderr, "Couldn't read from input\n");
		if (errno)
			perror(NULL);
		exit(errno || EXIT_FAILURE);
	}

	char *str = strchr(*line, ':');
	if (str == NULL) {
		fprintf(stderr, "Malformed line: %s", *line);
		exit(EXIT_FAILURE);
	}
	str++; // Skip ':'
	while (str[0] == ' ') str++;

	int x = 0;
	while(!(str[0] == '\n' || str[0] == '\0')) {
		list[x++] = atoi(str);
		while (str[0] >= '0' && str[0] <= '9') str++;
		while (str[0] == ' ') str++;
	}
	return x - 1;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s FILE\n", argv[0]);
		return EXIT_FAILURE;
	}

	char *inputpath = argv[1];
	FILE *input = fopen(inputpath, "r");
	if (input == NULL) {
		perror(inputpath);
		return errno;
	}

	size_t linesize = 0;
	char *line = NULL;

	int timelist[32] = { 0 };
	int x = readlist(&line, &linesize, input, timelist);
	int distancelist[32] = { 0 };
	int y = readlist(&line, &linesize, input, distancelist);
	if (x != y) {
		fprintf(stderr, "Time and Distance lists aren't of the same legnth\n");
		return EXIT_FAILURE;
	}

	size_t sum = 1;
	for (int i = 0; i <= x; i++) {
		int waystowin = 0;
		for (int j = 0; j <= timelist[i]; j++) {
			if ((timelist[i] - j) * j > distancelist[i])
				waystowin++;
		}
		if (waystowin)
			sum *= waystowin;
	}

	printf("%zu\n", sum);

	free(line);
	fclose(input);
	return EXIT_SUCCESS;
}
