#include <stdio.h>
#include <stdlib.h>

int checkaround(signed char* data, int width, int height, int x, int y, signed char searched) {
	int n = 0;
	int currentfield = x + y * width;
	for (int dx = -(x > 0); dx < 2 && dx + x < width; dx++) {
		for (int dy = -(y > 0); dy < 2 && dy + y < height; dy++) {
			if (*(data + currentfield + dx + dy * width) == searched) {
				n++;
			}
		}
	}
	return n;
}
int fillaround(signed char* data, int width, int height, int x, int y, signed char value) {
	int filled = 0;
	int currentfield = x + y * width;
	for (int dx = -(x > 0); dx < 2 && dx + x < width; dx++) {
		for (int dy = -(y > 0); dy < 2 && dy + y < height; dy++) {
			if (*(data + currentfield + dx + dy * width) != '0' && *(data + currentfield + dx + dy * width) != '7') {
				*(data + currentfield + dx + dy * width) = value;
				filled++;
			}
		}
	}
	return filled;
}

int solve(signed char* numfield, signed char* solution, int width, int height) {
	int unsolvable = 0;
	int solved = 0;
	while (!unsolvable && !solved) {
		solved = 1;
		unsolvable = 1;
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				int loc = x + width * y;
				signed char need = *(numfield + loc);
				signed char have = checkaround(solution, width, height, x, y, '0');
				signed char nothave = checkaround(solution, width, height, x, y, '7');
				signed char around = 9;
				if (x == 0 || y == 0 || x == width - 1 || y == height - 1) around = 6;
				if ((x == 0 || x == width - 1) && (y == 0 || y == height - 1)) around = 4;
				
				if (need == have && fillaround(solution, width, height, x, y, '7')) unsolvable = 0;
				else if (around - need == nothave && fillaround(solution, width, height, x, y, '0')) unsolvable = 0;
				else solved = 0;
			}
		}
	}
	unsolvable = 0;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			if (*(solution + x + width * y) == 0) unsolvable = 1;
		}
	}
	return unsolvable;
}

int main(int argc, char* argv[]) {
	FILE* imfile = NULL;
	char fname[64];
	if (argc == 1) {
		printf("please enter file name: ");
		signed char c = 0;
		int i;
		for (i = 0; i < 64 && (c = getchar()) != EOF && c != '\n'; i++) fname[i] = c;
		fname[i] = 0;
		imfile = fopen(fname, "r");
	} else {
		imfile = fopen(argv[1], "r");
	}
	if (imfile == NULL) {
		printf("invalid file name \"%s\"!\n", fname);
		return -1;
	}
	int width = 0;
	signed char c = 0;
	while ((c = fgetc(imfile)) != '\n' && c != EOF) width++;
	int height = 0;
	while (c != EOF) {
		while ((c = fgetc(imfile)) != EOF && c != '\n');
		height++;
	}
	printf("%dx%d\n", width, height);
	rewind(imfile);
	signed char* img = malloc(width * height * sizeof(signed char));
	signed char* nums = calloc(width * height, sizeof(signed char));
	signed char* solving = calloc(width * height, sizeof(signed char));
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			c = fgetc(imfile);
			if (c == '\n' || c == EOF) {
				printf("invalid pixelart (inconsistent line length)!\n");
				return -1;
			}
			if (c == 'Q') c = '.';
			if (c == ' ') c = '#';
			*(img + i * width + j) = c;
		}
		fgetc(imfile);
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			*(nums + i * width + j) = checkaround(img, width, height, j, i, '#');
		}
	}
	int x = 0, y = 0;
	while (y < height) {
		signed char current = *(nums + x + width * y);
		int ambiguous = 1;
		if (current != '.') {
			*(nums + x + width * y) = '.';
			signed char* solution0 = calloc(width * height, sizeof(signed char));
			ambiguous = solve(nums, solution0, width, height);
			free(solution0);
			if (ambiguous) *(nums + x + width * y) = current;
		}
		x++;
		if (x == width) {
			x = 0;
			y++;
		}
	}
	int solved = 0;
	int cursorx = height / 2;
	int cursory = width / 2;
	system("stty raw");
	while (!solved) {
		solved = 1;
		printf("\n\r");
		for (int i = 0; i < height; i++) {
			printf("\033[1;37m\033[1;43m");
			for (int j = 0; j < width; j++) {
				int loc = i * width + j;
				if (*(solving + loc) == '0') {
					printf("\033[1;37m\033[1;40m");
				} else if (*(solving + loc) == '7') {
					printf("\033[1;30m\033[1;47m");
				} else {
					printf("\033[1;30m\033[1;43m");
					solved = 0;
				}
				if (i == cursorx && j == cursory) printf("\033[1;31m");
				if (*(nums + loc) != '.') printf("%d ", *(nums + loc));
				else if (i == cursorx && j == cursory) printf("><");
				else printf(". ");
				if (i == cursorx && j == cursory) printf("\033[1;37m");
			}
			printf("\033[1;0m");
			printf("\n\r");
		}
		if (solved) {
			for (int x = 0; x < width; x++) {
				for (int y = 0; y < height; y++) {
					int loc = x + y * width;
					if ((*(solving + loc) == '0' && *(img + loc) == '.') || (*(solving + loc) == '7' && *(img + loc) == '#')) {
						solved = 0;
						break;
					}
				}
			}
		}
		if (!solved) {
			signed char control = getchar();
			if (control == 27) {
				printf("\rPause menu: press q to quit, any other key except '[' to continue: ");
				control = getchar();
				printf("\r");
				if (control == 91) {
					control = getchar();
					switch (control) {
						case 65: control = 'w'; break;
						case 68: control = 'a'; break;
						case 66: control = 's'; break;
						case 67: control = 'd'; break;
					}
				} else if (control == 'q') {
					system("stty cooked");
					printf("\nExiting (quit)\n");
					return 0;
				}
				
			}
			switch (control) {
				case 'w': if (cursorx > 0) cursorx--; break;
				case 'a': if (cursory > 0) cursory--; break;
				case 's': if (cursorx < height - 1) cursorx++; break;
				case 'd': if (cursory < width - 1) cursory++; break;
				case ' ':
				case '\t':
					signed char* c = solving + cursorx * width + cursory;
					if (*c == '0') *c = '7';
					else if (*c == '7') *c = 0;
					else *c = '0';
					break;
				case 'b': {
					signed char* selection = calloc(2048, sizeof(signed char));
					for (int k = 0; k < 2048; k++) selection[k] = -1;
					for (int k = 0; k < 1024; k++) {
						int new = 1;
						for (int m = 0; m < k; m++) if (selection[2 * m] == cursorx && selection[2 * m + 1] == cursory) new = 0;
						if (new) {
							selection[2 * k] = cursorx;
							selection[2 * k + 1] = cursory;
						}
						printf("\n\r");
						for (int i = 0; i < height; i++) {
							printf("\033[1;37m\033[1;43m");
							for (int j = 0; j < width; j++) {
								int loc = i * width + j;
								if (*(solving + loc) == '0') {
									printf("\033[1;37m\033[1;40m");
								} else if (*(solving + loc) == '7') {
									printf("\033[1;30m\033[1;47m");
								} else {
									printf("\033[1;30m\033[1;43m");
								}
								signed char selected = 0;
								for (int l = 0; l <= k; l++) if (selection[2 * l] == i && selection[2 * l + 1] == j) selected = 1;
								if (selected) printf("\033[1;34m");
								if (i == cursorx && j == cursory) printf("\033[1;31m");
								if (*(nums + loc) != '.') printf("%d ", *(nums + loc));
								else if (selected) printf("><");
								else printf(". ");
								if (selected) printf("\033[1;37m");
							}
							printf("\033[1;0m");
							printf("\n\r");
						}
						signed char control0 = getchar();
						if (control0 == 27) {
							printf("\rPause menu: press q to quit, any other key except '[' to continue: ");
							control0 = getchar();
							printf("\n\r");
							if (control0 == '[') {
								control0 = getchar();
								switch (control0) {
									case 65: control0 = 'w'; break;
									case 68: control0 = 'a'; break;
									case 66: control0 = 's'; break;
									case 67: control0 = 'd'; break;
								}
							} else if (control0 == 'q') {
								system("stty cooked");
								printf("\nExiting (quit)\n");
								return 0;
							}
						}
						switch (control0) {
							case 'w': if (cursorx > 0) cursorx--; break;
							case 'a': if (cursory > 0) cursory--; break;
							case 's': if (cursorx < height - 1) cursorx++; break;
							case 'd': if (cursory < width - 1) cursory++; break;
						}
						if (control0 == ' ') break;
					}
					do {
						for (int k = 0; k < 1024; k++)//(selection[2*k] != -1 && selection[2*k+1] != -1) && k < 1024; k++) {
						if (selection[2*k] != -1 && selection[2*k+1] != -1) {
							signed char* c = solving + selection[2 * k] * width + selection[2 * k + 1];
							if (*c == '0') *c = '7';
							else if (*c == '7') *c = 0;
							else *c = '0';
						}
						printf("\n\r");
						for (int i = 0; i < height; i++) {
							printf("\033[1;37m\033[1;43m");
							for (int j = 0; j < width; j++) {
								int loc = i * width + j;
								if (*(solving + loc) == '0') {
									printf("\033[1;37m\033[1;40m");
								} else if (*(solving + loc) == '7') {
									printf("\033[1;30m\033[1;47m");
								} else {
									printf("\033[1;30m\033[1;43m");
								}
								signed char selected = 0;
								for (int l = 0; l < 1024; l++) if (selection[2 * l] == i && selection[2 * l + 1] == j) selected = 1;
								if (selected) printf("\033[1;34m");
								if (*(nums + loc) != '.') printf("%d ", *(nums + loc));
								else if (selected) printf("><");
								else printf(". ");
								if (selected) printf("\033[1;37m");
							}
							printf("\033[1;0m");
							printf("\n\r");
						}
					} while ((control = getchar()) == ' ');
					switch(control) {
						case 'w': if (cursorx > 0) cursorx--; break;
						case 'a': if (cursory > 0) cursory--; break;
						case 's': if (cursorx < height - 1) cursorx++; break;
						case 'd': if (cursory < width - 1) cursory++; break;
						case 'q': {
							system("stty cooked");
							printf("\nExiting (quit)\n");
							return 0;
						}
					}
					free(selection);
					break;
				}
				case 'q': {
					system("stty cooked");
					printf("\nExiting (quit)\n");
					return 0;
				}
			}
		}
	}
	printf("\n\rPuzzle solved!\n\rResult:\n\r");
	for (int i = 0; i < height; i++) {
		printf("\033[1;37m\033[1;43m");
		for (int j = 0; j < width; j++) {
			int loc = i * width + j;
			if (*(solving + loc) == '0') {
				printf("\033[1;37m\033[1;40m");
			} else if (*(solving + loc) == '7') {
				printf("\033[1;30m\033[1;47m");
			} else {
				printf("\033[1;30m\033[1;43m");
			}
			printf("  ");
		}
		printf("\033[1;0m");
		printf("\n\r");
	}
	system("stty cooked");
	
	free(img);
	free(nums);
	free(solving);
	return 0;
}
