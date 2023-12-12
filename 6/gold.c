#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

size_t readlist(char **line, size_t *linesize, FILE *input) {
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

	char numascii[32];
	int n = 0;
	for(int i = 0; !(str[i] == '\n' || str[i] == '\0'); i++) {
		if (isdigit(str[i]))
			numascii[n++] = str[i];
	}
	numascii[n] = '\0';

	return atoll(numascii);
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

	size_t time = readlist(&line, &linesize, input);
	size_t distance = readlist(&line, &linesize, input);
	size_t hodl = time;

	size_t waystowin = 0;
	while (hodl) {
		if ((time - hodl) * hodl > distance)
			waystowin++;
		hodl--;
	}

	printf("%zu\n", waystowin);

	free(line);
	fclose(input);
	return EXIT_SUCCESS;
}
