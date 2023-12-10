#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

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

	size_t linessize = 0;
	char *line = NULL;
	size_t sum = 0;
	int winningnumbers[128];
	int numbersinhand[128];
	while(getline(&line, &linessize, input) != -1) {
		memset(winningnumbers, 0, sizeof(int) * 128);
		memset(numbersinhand, 0, sizeof(int) * 128);

		int score = 0;
		char *str = strchr(line, ':');
		if (str == NULL) {
			fprintf(stderr, "Malformed line: %s", line);
			return EXIT_FAILURE;
		}
		str++; // Skip ':'
		while (str[0] == ' ') str++;

		int x = 0;
		while(str[0] != '|') {
			winningnumbers[x] = atoi(str);
			while (str[0] >= '0' && str[0] <= '9') str++;
			while (str[0] == ' ') str++;
			x++;
		}
		x--;
		str++; // Skip '|'
		while (str[0] == ' ') str++;

		while (str[0] != '\n') {
			int inhand = atoi(str);
			while (str[0] >= '0' && str[0] <= '9') str++;
			while (str[0] == ' ') str++;

			for (int y = x; y >= 0; y--) {
				if (winningnumbers[y] == inhand) {
					score = score == 0 ? 1 : score * 2;
					break;
				}
			}
		}

		sum += score;
	}
	if (errno) {
		perror(inputpath);
		return errno;
	}

	printf("%zu\n", sum);

	free(line);
	fclose(input);
	return EXIT_SUCCESS;
}
