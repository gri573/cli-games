#include <ncurses.h>
#include <stdlib.h>
#include <tgmath.h>

int printframe(int width, int height, char frame[width][height], int color[width][height]);

float clamp(float val, float min, float max);

float fract(float val);
int main() {
	initscr();
	start_color();
	if (!has_colors()) {
		endwin();
		return -1;
	} else {
		init_pair(1, COLOR_WHITE, COLOR_WHITE);
		init_pair(2, COLOR_GREEN, COLOR_GREEN);
		init_pair(3, COLOR_WHITE, COLOR_BLUE);
		init_pair(4, COLOR_BLUE, COLOR_BLUE);
		init_pair(5, COLOR_BLACK, COLOR_RED);
	}
	char frame[COLS][LINES];
	int colors[COLS][LINES];
	nodelay(stdscr, TRUE);
	float pos[2] = {0, 0};
	float heights[10] = {0};
	float sizes[10] = {10, 10, 10, 10, 10};
	for (int k = 5; k < 10; k++) {
		heights[k] = 0.002 * (rand() % 1000) - 1;
		sizes[k] = 0.0007 * (rand() % 1000) + 0.5;
	}
	#define DT 50
	#define V 2.0
	#define BARWIDTH 0.21
	int xpos0 = 0;
	for (int k = 0; 1; k++) {
		float xpos = 0.001 * DT * V * k;
		if ((int) xpos - 1.5 > xpos0) {
			for (int k = 0; k < 9; k++) {
				heights[k] = heights[k+1];
				sizes[k] = sizes[k+1];
			}
			heights[9] = 0.002 * (rand() % 1000) - 1;
			sizes[9] = 0.0007 * (rand() % 1000) + 0.5;
			xpos0 += 3;
		}
		char c = getch();
		switch(c) {
		case ' ':
		case '\t':
			pos[1] = 3;
			break;
		case 'q':
			endwin();
			return 0;
		}
		// draw image:
		for (int x = 0; x < COLS; x++)
			for (int y = 0; y < LINES; y++) {
				frame[x][y] = ' ';
				colors[x][y] = 3;
				float local_pos[2] = {x / (LINES - 1.0) * 2 - 1, y / (LINES - 1.0) * 4 - 2};
				float global_pos[2] = {local_pos[0] + xpos, local_pos[1]};
				if (fmax(fabs(local_pos[0]), fabs(global_pos[1] + pos[0])) < 0.212)
					colors[x][y] = 5;
			    if (fract(global_pos[0] / 3.0 - 0.1) < BARWIDTH) {
					int i = (int) (global_pos[0] / 3.0  - xpos0 / 3 + 1);
					if (fabs(global_pos[1] - heights[i]) > sizes[i]) {
						colors[x][y] = 2;
					}
				}
			}
		if (fract(xpos / 3.0 - 0.1) < BARWIDTH) {
			if (fabs(pos[0] + heights[1]) > sizes[1] - 0.1) {
			    for (int k = 0; k < 10; k++) {
					frame[k+2][5] = "Game Over!"[k];
					colors[k+2][5] = 0;
				}
				printframe(COLS, LINES, frame, colors);
				while ((c = getch()) != '\n') {
					refresh();
					napms(50);
				}
				endwin();
				return 0;
			}
		}
		printframe(COLS, LINES, frame, colors);
		refresh();
		napms(DT);
		pos[0] += 0.001 * DT * pos[1];
		pos[1] -= 10 * 0.001 * DT;
		pos[0] = clamp(pos[0], -2, 2);
		if (pos[0] > 1.99)
			pos[1] = fmin(pos[1], 0);
		if (pos[0] <-1.99)
			pos[1] = fmax(pos[1], 0);
	}
	endwin();
	return 0;
}

int printframe(int width, int height, char frame[width][height], int color[width][height]) {
  for (int x = 0; x < width; x++) {
	for (int y = 0; y < height; y++) {
	  if (color[x][y] < 0) {
		color_set(-color[x][y], NULL);
		attron(A_BOLD);
	  } else color_set(color[x][y], NULL);
	  mvaddch(y, x, frame[x][y]);
	  if (color[x][y] < 0) attroff(A_BOLD);
	}
  }
  color_set(0, NULL);
  return 0;
}

float clamp(float val, float min, float max) {
	if (val < min)
		return min;
	if (val > max)
		return max;
	return val;
}

float fract(float val) {
	if (val > 0)
		return val - (int) val;
	return val - (int) val - 1;
}
