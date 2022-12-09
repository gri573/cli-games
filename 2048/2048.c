#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int FS = 3;
#define WC 2048

int ipow(int x, int n) {
	int p = 1;
	for (int k = 0; k < n; k++) p *= x;
	return p;
}

int printfield(char field[FS][FS]) {
	
	char printstring[3*FS][13*FS];
	for (int i = 0; i < 39*FS*FS; i++) printstring[0][i] = ' ';
	for (int i = 0; i < FS; i++) {
		for (int j = 0; j < FS; j++) {
			int val = 1<<field[i][j];
			char color[8] = "\033[1;40m";
			if (val == 1) color[5] = 48;
			else color[5] = field[i][j] % 6 + 49;
			for (int k = 0; k < 3; k++) for (int l = 0; l < 7; l++) printstring[3*i+k][13*j+l] = color[l];
			if (val > 1) for (int k = 0; k < 4; k++) if (ipow(10, k) <= val) printstring[3*i+1][13*j+11-k] = 48 + (val / ipow(10, k)) % 10;
		}
	}
	printf("\r");
	for (int i = 0; i < 3*FS; i++) {
		char thisstring[13*FS+9];
		for (int j = 0; j < 13*FS; j++) thisstring[j] = printstring[i][j];
		const char clearcol[7] = "\033[1;0m";
		for (int j = 0; j < 6; j++) thisstring[j+13*FS] = clearcol[j];
		thisstring[13*FS+6] = '\n';
		thisstring[13*FS+7] = '\r';
		thisstring[13*FS+8] = 0;
		printf(thisstring);
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
	if (argc > 1) FS = argv[1][0] - 48;
	char field[FS][FS];
	for (int i = 0; i < FS * FS; i++) field[0][i] = 0;
	char freespaces[FS*FS][2];
	int ij[2] = {0};
	for (; ij[0] < FS; ij[0]++) for (; ij[1] < FS; ij[1]++) for (int l = 0; l < 2; l++) freespaces[FS*ij[0]+ij[1]][l] = ij[l];
	int freecount = FS*FS;
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
		if (maxval >= WC) {
			system("stty cooked");
			int score = 0;
			for (int i = 0; i < FS; i++) for (int j = 0; j < FS; j++) if (field[i][j] > 0) score += 1<<field[i][j];
			printfield(field);
			printf("\rYOU WON!\n\nScore: %d\n", score);
			return 0;
		}
	}
	system("stty cooked");
	int score = 0;
	for (int i = 0; i < FS; i++) for (int j = 0; j < FS; j++) if (field[i][j] > 0) score += 1<<field[i][j];
	printf("\rGAME OVER\n\nScore: %d\n", score);
	return 0;
}

