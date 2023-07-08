#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

void int_to_str(char* str, int x, int len) {
	str[len] = 0;
    int negative = (x < 0);
	if (!negative)
		len--;
	for (int k = 0; k <= len; k++) {
		str[len-k] = x % 10 + 48;
		x /= 10;
	}
	if (negative)
		str[0] = '-';
}

void drawstack(int n, char stacks[][5]) {
	int screenpos = 4 * n + 2;
	color_set(0, NULL);
	char numstr[] = "aa";
	int_to_str(numstr, n + 1, 2);
	mvaddstr(LINES - 1, screenpos, numstr);
	for (int i = 0; i < 4 && stacks[n][i]; i++) {
		color_set(stacks[n][i], NULL);
		mvaddch(LINES - 2 - i, screenpos, ' ');
		mvaddch(LINES - 2 - i, screenpos + 1, ' ');
	}
}


int main(int argc, char** argv) {
	int N_STACKS = 10;
	if (argc > 1) {
		N_STACKS = atoi(argv[1]);
	}
	int now = (int) time(0);
	for (int k = 0; k < now % 1000; k++) rand();
	initscr();
	start_color();
	if (!has_colors()) {
		endwin();
		printf("No colour support!\n");
		return -1;
	} else {
		init_pair(1, COLOR_RED,    COLOR_RED);
		init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
		init_pair(3, COLOR_GREEN,  COLOR_GREEN);
		init_pair(4, COLOR_BLUE,   COLOR_BLUE);
		init_pair(5, COLOR_WHITE,  COLOR_WHITE);
		init_pair(6, COLOR_CYAN,   COLOR_CYAN);
	}
	char continue_loop = 1;
	char stacks[N_STACKS][5];
	for (int i = 0; i < N_STACKS - 1; i++) {
		char col = rand() % 6 + 1;
		for (int j = 0; j < 4; j++) {
			stacks[i][j] = col;
		}
		stacks[i][4] = 0;
	}
	for (int j = 0; j < 5; j++) {
		stacks[N_STACKS-1][j] = 0;
	}
	for (int i = 0; i < 1000 * N_STACKS * N_STACKS; i++) {
		int n1 = rand() % N_STACKS;
		int n2 = rand() % N_STACKS;
		int j1, j2;
		for (j1 = 0; stacks[n1][j1]; j1++);
		for (j2 = 0; stacks[n2][j2]; j2++);
		if (j1 > 0 && j2 < 4) {
			stacks[n2][j2] = stacks[n1][j1-1];
			stacks[n1][j1-1] = 0;
		}
	}
	while(continue_loop) {
		clear();
		for (int n = 0; n < N_STACKS; n++)
			drawstack(n, stacks);
		color_set(0, NULL);
		signed char c = getch();
		switch(c) {
		case 'q':
		case 'Q':
			mvaddstr(0, 0, "Really Quit? [y/N]");
			if ((c = getch()) == 'y' || c == 'Y') {
				continue_loop = 0;
			}
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			int n1 = c - 48;
			if ((c = getch() - 48) >= 0 && c < 10) {
				n1 = 10 * n1 + c ;
			}
			if (c != '\n' - 48 && c != '\t' - 48) {
				if (c == EOF - 48) {
					continue_loop = 0;
					break;
				}
				while ((c = getch()) != '\n' && c != '\t' && c != EOF);
				if (c == EOF) {
					continue_loop = 0;
					break;
				}
			}
			color_set(0, NULL);
			mvaddstr(LINES - 8, 4 * n1 - 2, "\\/");
			c = getch();
			int n2 = c - 48;
			if ((c = getch() - 48) >= 0 && c < 10) {
				n2 = 10 * n2 + c ;
			}
			if (c != '\n' - 48 && c != '\t' - 48) {
				if (c == EOF - 48) {
					continue_loop = 0;
					break;
				}
				while ((c = getch()) != '\n' && c != '\t' && c != EOF);
				if (c == EOF) {
					continue_loop = 0;
					break;
				}
			}
			if (n1 <= 0 || n2 <= 0 || n1 > N_STACKS || n2 > N_STACKS || n1 == n2) {
				break;
			}
			n1--;
			n2--;
			int j1, j2;
			for (j1 = 0; stacks[n1][j1]; j1++);
			for (j2 = 0; stacks[n2][j2]; j2++);
			char col = stacks[n1][j1-1];
			while (j1 > 0 && j2 < 4 && stacks[n1][j1-1] == col) {
				stacks[n2][j2] = stacks[n1][j1-1];
				stacks[n1][j1-1] = 0;
				j1--;
				j2++;
			}
			break;
		case EOF:
			continue_loop = 0;
			break;
		}
	}
	endwin();
	return 0;
}
