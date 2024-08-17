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
	int N_COLORS = 6;
	char end_when_done = 0;
	char strict_mode = 0;
	char skip_next = 0;
	for (int k = 1; k < argc; k++) {
		if (skip_next) {
			skip_next = 0;
			continue;
		}
		if (!argv[k][0])
			continue;
		if (argv[k][0] == '-')
			switch (argv[k][1]) {
			case 'S':
			case 's':
				strict_mode = 1;
				break;
			case 'T':
			case 't':
				end_when_done = 1;
				break;
			case 'H':
			case 'h':
				printf("Colour sorting game:\n\nthe goal is that all stacks should contain either nothing\nor four identical colours.\n\nMoving colours from  one stack to another is  achieved by\nentering the  number of the first  stack followed  by the\nnumber of  the second stack,  separated by TAB  or ENTER.\nAlternatively, one can navigate  to stacks using left and\nright arrow keys.\n\nOptions:\n\n-s\tcolours can only be  placed on other instances of\n  \tthe same colour\n-t\tcheck  whether the  puzzle is solved and  end the\n  \tgame if it is, print the solving time on exit\n-h\tPrint this message and exit\n");
				return 0;
			case 'C':
			case 'c':
				if (argc > k+1) {
					N_COLORS = atoi(argv[k+1]);
					if (N_COLORS <= 0) {
						fprintf(stderr, "At least one colour is required!\n");
						N_COLORS = 1;
					}
					if (N_COLORS > 6) {
						fprintf(stderr, "At most 6 colours are supported!\n");
						N_COLORS = 6;
					}
					skip_next = 1;
				}
			}
		else
			N_STACKS = atoi(argv[k]);
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
	char* end_message = "quit";
	char continue_loop = 1;
	char stacks[N_STACKS][5];
	for (int i = 0; i < N_STACKS - 1; i++) {
		char col = rand() % N_COLORS + 1;
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
		if (j1 > 0 && j2 < 4 && n1 != n2) {
			stacks[n2][j2] = stacks[n1][j1-1];
			stacks[n1][j1-1] = 0;
		}
	}
	int pos = N_STACKS/2;
	time_t time_start = time(0);
	while(continue_loop) {
		clear();
		for (int n = 0; n < N_STACKS; n++)
			drawstack(n, stacks);
		color_set(0, NULL);
		signed char c = getch();
		int n1 = 0;
		int n2 = 0;
		switch(c) {
		case 'q':
		case 'Q': {
			mvaddstr(0, 0, "Really Quit? [y/N]");
			if ((c = getch()) == 'y' || c == 'Y') {
				continue_loop = 0;
			}
			break;
		}
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': {
			n1 = c - 48;
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
			n2 = c - 48;
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
			if (n1 <= 0 || n2 <= 0 || n1 > N_STACKS || n2 > N_STACKS) {
				n1 = n2;
				break;
			}
			n1--;
			n2--;
			break;
		}
		case 27: {
			c = getch();
			if (c == '[') {
				while ((c = getch()) != '\t' && c != '\n' && c != EOF) {
					if (c == 27 || c == '[')
						c = getch();
					if (c == 'C' && pos < N_STACKS - 1) {
						mvaddstr(LINES - 8, 4 * pos + 2, "        ");
						pos++;
						mvaddstr(LINES - 8, 4 * pos + 2, "\\/");
					}
					if (c == 'D' && pos > 0) {
						mvaddstr(LINES - 8, 4 * pos + 2, "        ");
						pos--;
						mvaddstr(LINES - 8, 4 * pos + 2, "\\/");
					}
				}
				if (c == EOF) {
					continue_loop = 0;
					break;
				}
				n1 = pos;
				while ((c = getch()) != '\t' && c != '\n' && c != EOF) {
					if (c == 27)
						c = getch();
					if (c == 'C' && pos < N_STACKS - 1) {
						mvaddstr(LINES - 8, 4 * pos + 2, "        ");
						pos++;
						mvaddstr(LINES - 8, 4 * pos + 2, "\\/");
					}
					if (c == 'D' && pos > 0) {
						mvaddstr(LINES - 8, 4 * pos + 2, "        ");
						pos--;
						mvaddstr(LINES - 8, 4 * pos + 2, "\\/");
					}
				}
				n2 = pos;
			}
			break;
		}
		case EOF: {
			continue_loop = 0;
			break;
		}
		}
		if (n1 == n2) continue;
		int j1, j2;
		for (j1 = 0; j1 < 4 && stacks[n1][j1]; j1++);
		for (j2 = 0; j2 < 4 && stacks[n2][j2]; j2++);
		signed char col = j1 > 0 ? stacks[n1][j1-1] : 0;
		if (strict_mode && j2 > 0 && stacks[n2][j2-1] != col)
			continue;
		while (j1 > 0 && j2 < 4 && stacks[n1][j1-1] == col) {
			stacks[n2][j2] = stacks[n1][j1-1];
			stacks[n1][j1-1] = 0;
			j1--;
			j2++;
		}
		if (end_when_done) {
			char done = 1;
			for (int k = 0; done && k < N_STACKS; k++) {
				char color0 = stacks[k][0];
				for (int j = 1; j < 4; j++)
					if (color0 != stacks[k][j])
						done = 0;
			}
			if (done) {
				end_message = "solved";
				continue_loop = 0;
			}
		}

	}
	endwin();
	if (end_when_done)
		printf("[%s] Finished after %d seconds\n", end_message, (int) (time(0) - time_start));
	return 0;
}
