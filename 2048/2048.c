#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int FS = 3;

int ipow(int x, int n) {
	int p = 1;
	for (int k = 0; k < n; k++) p *= x;
	return p;
}

int abs(int x) {
	if (x < 0) return -x;
	return x;
}

int printfield(char field[FS][FS], int win) {
	char printstring[3*FS][13*FS];
	char text[5][17] = {
		{1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,1},
		{1,0,0,0,0,0,1,0,0,1,0,0,1,1,0,0,1},
		{1,0,0,0,0,0,1,0,0,1,0,0,1,0,1,0,1},
		{0,1,0,1,0,1,0,0,0,1,0,0,1,0,0,1,1},
		{0,0,1,0,1,0,0,0,0,1,0,0,1,0,0,0,1}
	};
	for (int i = 0; i < 39*FS*FS; i++) printstring[0][i] = ' ';
	for (int i = 0; i < FS; i++) {
		for (int j = 0; j < FS; j++) {
			int val = 1<<field[i][j];
			char color[8] = "\033[1;40m";
			if (val > 1) color[5] = field[i][j] % 6 + 49;
			for (int k = 0; k < 3; k++) for (int l = 0; l < 7; l++) printstring[3*i+k][13*j+l] = color[l];
			if (val > 1) for (int k = 0; k < 4; k++) if (ipow(10, k) <= val) printstring[3*i+1][13*j+11-k] = 48 + (val / ipow(10, k)) % 10;
		}
	}
	printf("\r");
	for (int i = 0; i < 3*FS; i++) {
		char thisstring[13*FS];
		for (int j = 0; j < 13*FS; j++) thisstring[j] = printstring[i][j];
		for (int k = 0; k < 13*FS; k++) {
			char sparkle = 0;
			if (win && k % 13 >= 7 && abs(rand()) % 20 < 2) sparkle = rand() % 6 + 49;
			if (sparkle) {
				char color[8] = "\033[1;40m";
				color[5] = sparkle;
				printf("%s", color);
			}
			if (win && k % 13 >= 7) {
				int vpos = i - 3*FS/2 + 2;
				int hpos = (6 * (k/13) + k%13 - 7) - 3*FS + 9;
				if (vpos >= 0 && vpos < 5 && hpos >= 0 && hpos < 17 && text[vpos][hpos]) {
					sparkle = win + 48;
					char color[8] = "\033[1;40m";
					color[5] = sparkle;
					printf("%s", color);
					thisstring[k] = 'O';
				}
			}
			putchar(thisstring[k]);
			if (sparkle) {
				thisstring[13 * (k / 13) + 7] = 0;
				printf("%s", thisstring + 13 * (k / 13));
			}
		}
		printf("\033[1;0m\n\r");
	}
	printf("\n\r");
	return 0;
}

int movefield(int swipeAxis, int dir, char field[FS][FS]) {
	char wasCompressed[FS][FS];
	for (int i = 0; i < FS * FS; i++) wasCompressed[0][i] = 0;
	int changed = 0;
	if (swipeAxis < 0 || swipeAxis > 1 || (dir != -1 && dir != 1)) return -1;
	int ij[2];
	for (int k = 0; k < FS; k++) {
		for (ij[0] = (FS-1) * (1 + dir) / 2; ij[0] != (FS+1) * (1 - dir) / 2 - 1; ij[0] -= dir) {
			for (ij[1] = (FS-1) * (1 + dir) / 2; ij[1] != (FS+1) * (1 - dir) / 2 - 1; ij[1] -= dir) {
				int lastij[2];
				lastij[0] = ij[0];
				lastij[1] = ij[1];
				lastij[swipeAxis] -= dir;
				if (lastij[swipeAxis] < 0 || lastij[swipeAxis] >= FS) continue;
				if (field[ij[0]][ij[1]] == 0) {
					if (field[lastij[0]][lastij[1]] != 0) {
						field[ij[0]][ij[1]] = field[lastij[0]][lastij[1]];
						field[lastij[0]][lastij[1]] = 0;
						changed = 1;
					}
				}
				else if (!wasCompressed[lastij[0]][lastij[1]] && !wasCompressed[ij[0]][ij[1]] && field[ij[0]][ij[1]] == field[lastij[0]][lastij[1]]) {
					field[ij[0]][ij[1]] += 1;
					field[lastij[0]][lastij[1]] = 0;
					wasCompressed[ij[0]][ij[1]] = 1;
					changed = 1;
				}
			}
		}
	}
	return changed;
}

int main(int argc, char** argv) {
	int now = (int) time(0);
	for (int k = 0; k < now % 1000; k++) rand();
	int WC = 0;
	if (argc > 1) {
		if (argv[1][0] >= 48 && argv[1][0] < 58) FS = argv[1][0] - 48;
		else {
			printf("Invalid Argument!\n");
			return -1;
		}
		if (argv[1][1] >= 48 && argv[1][1] < 58) FS = 10*FS+argv[1][1] - 48;
		if (argc > 2) {
			WC = 0;
			for (int i = 0; argv[2][i] != 0; i++) WC = 10 * WC + argv[2][i] - 48;
		}
	}
	if (WC == 0) {
		WC = 1 << ((4 * FS * FS) / 5);
		if (WC > 2048) WC = 2048;
	}
	printf("%d\n", WC);
	char field[FS][FS];
	for (int i = 0; i < FS * FS; i++) field[0][i] = 0;
	char freespaces[FS*FS][2];
	for (int i = 0; i < FS; i++) {
		for (int j = 0; j < FS; j++) {
			freespaces[FS*i+j][0] = i;
			freespaces[FS*i+j][1] = j;
		}
	}
	int freecount = FS*FS;
	for (int k = 0; k < freecount; k++) printf("(%d, %d); ", freespaces[k][0], freespaces[k][1]);
	printf("\n");
	int changed = 1;
	int won = 0;
	system("stty raw");
	for (int k = 0; freecount; k++) {
		if (changed > 0) {
			char* new = freespaces[abs(rand()) % freecount];
			field[new[0]][new[1]] = 1;
			printfield(field, 0);
		}
		char c;
		while ((c = getchar()) == 27 || c == '[');
		switch(c) {
		case '?':
		case ' ':
			printfield(field, 0);
			changed = 0;
			break;
		case 'q':
			printf("\n\rReally Quit?(y/N)");
			if (getchar() == 'y') {
				printf("\n\r");
				system("stty cooked");
				return 0;
			}
			break;
		case 'a':
		case 'D':
			changed = movefield(1, -1, field);
			break;
		case 'w':
		case 'A':
			changed = movefield(0, -1, field);
			break;
		case 'd':
		case 'C':
			changed = movefield(1, 1, field);
			break;
		case 's':
		case 'B':
			changed = movefield(0, 1, field);
			break;
		default:
			changed = 0;
		}
		freecount = 0;
		int maxval = 1;
		for (int i = 0; i < FS; i++) {
			for (int j = 0; j < FS; j++) {
				if (1<<field[i][j] > maxval) maxval = 1<<field[i][j];
				if (field[i][j] == 0) {
					freespaces[freecount][0] = i;
					freespaces[freecount][1] = j;
					freecount++;
				}
			}
		}
		if (!won && maxval >= WC) {
			// system("stty cooked");
			won = 1;
			int score = 0;
			for (int i = 0; i < FS; i++) for (int j = 0; j < FS; j++) if (field[i][j] > 0) score += 1<<field[i][j];
			for (int j = 1; j < 7; j++) {
				putchar('\a');
				printfield(field, j);
				usleep(100000);
				printfield(field, j);
				usleep(100000);
			}
			//return 0;
		}
	}
	system("stty cooked");
	int score = 0;
	for (int i = 0; i < FS; i++) for (int j = 0; j < FS; j++) if (field[i][j] > 0) score += 1<<field[i][j];
	printf("\rGAME OVER\n\nScore: %d\n", score);
	return 0;
}

/*
Local Variables:
compile-command: "make -C .."
End:
*/
