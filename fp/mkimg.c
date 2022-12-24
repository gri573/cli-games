#include <stdio.h>
#include <stdlib.h>

int printfield(signed char* solving, int width, int height, int cursorx, int cursory);

int main() {
	printf("please enter image dimensions (width, height)\n");
	int width;
	int height;
	scanf("%d, %d", &width, &height);
	getchar();
	signed char* img = calloc(width * height, sizeof(signed char));
	int cursorx = height / 2;
	int cursory = width / 2;
	system("stty raw");
	while (1) {
		printfield(img, width, height, cursorx, cursory);
		char c = getchar();
		if (c == 27 || c == '[') continue;
		switch(c) {
		case 'A':
			if (cursorx > 0) cursorx--;
			break;
		case 'B':
			if (cursorx < height - 1) cursorx++;
			break;
		case 'C':
			if (cursory < width - 1) cursory++;
			break;
		case 'D':
			if (cursory > 0) cursory--;
			break;
		case ' ':
			switch(img[width * cursorx + cursory]) {
			case 7:
				img[width * cursorx + cursory] = 0;
				break;
			case 0:
				img[width * cursorx + cursory] = 7;
				break;
			}
			break;
		case 's':
			system("stty cooked");
			printf("\rSave as (file name): ");
			char fname[64];
			scanf("%s", fname);
			FILE* outfile = fopen(fname, "w");
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					c = '.';
					if (img[i * width + j] == 7) c = '#';
					fputc(c, outfile);
				}
				fputc('\n', outfile);
			}
			fclose(outfile);
			printf("file saved\n");
			return 0;
		}
	}
	system("stty cooked");
	return 0;
}

int printfield(signed char* solving, int width, int height, int cursorx, int cursory) {
	for (int i = 0; i < height; i++) {
		printf("\r\033[1;37m\033[1;43m");
		for (int j = 0; j < width; j++) {
			int loc = i * width + j;
			switch (*(solving + loc)) {
			case 7:
				printf("\033[1;37m\033[1;40m");
				break;
			case 0:
				printf("\033[1;30m\033[1;47m");
				break;
			default:
				printf("\033[1;30m\033[1;43m");
				break;
			}
			if (cursorx == i && cursory == j) printf("><");
			else printf("  ");
		}
		printf("\033[1;0m");
		printf("\n\r");
	}
	printf("\n");
	return 0;
}
