#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int ipow(int x, int n) {
	int p = 1;
	for (int k = 0; k < n; k++) p *= x;
	return p;
}

int printfield(char field[4][4]) {
	
	char printstring[12][52];
	for (int i = 0; i < 624; i++) printstring[0][i] = ' ';
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int val = 1<<field[i][j];
			char color[8] = "\033[1;40m";
			if (val == 1) color[5] = 48;
			else color[5] = field[i][j] % 6 + 49;
			for (int k = 0; k < 3; k++) for (int l = 0; l < 7; l++) printstring[3*i+k][13*j+l] = color[l];
			if (val > 1) for (int k = 0; k < 4; k++) if (ipow(10, k) <= val) printstring[3*i+1][13*j+11-k] = 48 + (val / ipow(10, k)) % 10;
		}
	}
	printf("\r");
	for (int i = 0; i < 12; i++) {
		char thisstring[61];
		for (int j = 0; j < 52; j++) thisstring[j] = printstring[i][j];
		const char clearcol[7] = "\033[1;0m";
		for (int j = 0; j < 6; j++) thisstring[j+52] = clearcol[j];
		thisstring[58] = '\n';
		thisstring[59] = '\r';
		thisstring[60] = 0;
		printf(thisstring);
	}
	printf("\n\r");
	return 0;
}

int movefield(int swipeAxis, int dir, char field[4][4]) {
	char wasCompressed[4][4] = {{0}};
	int changed = 0;
	if (swipeAxis < 0 || swipeAxis > 1 || (dir != -1 && dir != 1)) return -1;
	int ij[2];
	for (int k = 0; k < 4; k++) {
		for (ij[0] = 3 * (1 + dir) / 2; ij[0] != 5 * (1 - dir) / 2 - 1; ij[0] -= dir) {
			for (ij[1] = 3 * (1 + dir) / 2; ij[1] != 5 * (1 - dir) / 2 - 1; ij[1] -= dir) {
				int lastij[2];
				lastij[0] = ij[0];
				lastij[1] = ij[1];
				lastij[swipeAxis] -= dir;
				if (lastij[swipeAxis] < 0 || lastij[swipeAxis] > 3) continue;
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

int main() {
	char field[4][4] = {{0}};
	char freespaces[16][2];
	int ij[2] = {0};
	for (; ij[0] < 4; ij[0]++) for (; ij[1] < 4; ij[1]++) for (int l = 0; l < 2; l++) freespaces[4*ij[0]+ij[1]][l] = ij[l];
	int freecount = 16;
	int changed = 1;
	system("stty raw");
	for (int k = 0; freecount; k++) {
		if (changed > 0) {
			char* new = freespaces[rand()%freecount];
			field[new[0]][new[1]] = 1;
		}
		printfield(field);
		char c;
		while ((c = getchar()) == 27 || c == '[');
		switch(c) {
		case 'q':
			printf("\nReally Quit?(y/N)");
			if (getchar() == 'y') {
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
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (field[i][j] == 0) {
					freespaces[freecount][0] = i;
					freespaces[freecount][1] = j;
					freecount++;
				}
			}
		}
	}
	system("stty cooked");
	int score = 0;
	for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) if (field[i][j] > 0) score += 1<<field[i][j];
	printf("\rGAME OVER\n\nScore: %d\n", score);
	return 0;
}

