#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int solve(char field[9][9]) {
	int madeProgress = 1;
	while (madeProgress) {
		madeProgress = 0;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (field[i][j] == 0) {
					char invalid[9] = {0};
					for (int k = 0; k < 9; k++) {
						if (field[i][k] > 0) invalid[field[i][k] - 1] = 1;
						if (field[k][j] > 0) invalid[field[k][j] - 1] = 1;
						int squareEntry[2] = {3 * (i / 3) + k % 3, 3 * (j / 3) + k / 3};
						if (field[squareEntry[0]][squareEntry[1]] > 0) invalid[field[squareEntry[0]][squareEntry[1]] - 1] = 1;
					}
					signed char thisNum = 0;
					for (int k = 0; k < 9; k++) {
						if (!invalid[k]) {
							if (thisNum == 0) thisNum = k + 1;
							else thisNum = -1;
						}
					}
					if (thisNum == 0) return -1;
					if (thisNum > 0) {
						field[i][j] = thisNum;
						madeProgress = 1;
					}
				}
			}
		}
	}
	char full = 1;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (field[i][j] == 0) full = 0;
		}
	}
	if (full) return 1;
	return 0;
}

int printSudoku(char field[9][9], int cursor[2]) {
	printf("\r");
	char* col[2] = {"\033[1;30m\033[1;47m", "\033[1;37m\033[1;40m"};
	char* highlightCol[2] = {"\033[1;30m\033[1;41m", "\033[1;30m\033[1;41m"};
	char* wrongCol[2] = {"\033[1;31m\033[1;47m", "\033[1;31m\033[1;40m"};
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			int wrong = 0;
			if (field[i][j] > 0) {
				for (int k = 0; k < 9 && !wrong; k++) {
					if (i != k && field[k][j] == field[i][j]) wrong = 1;
					if (j != k && field[i][k] == field[i][j]) wrong = 1;
					int offsetPos[2] = {3 * (i / 3) + k % 3, 3 * (j / 3) + k / 3};
					if (!(offsetPos[0] == i && offsetPos[1] == j) && field[offsetPos[0]][offsetPos[1]] == field[i][j]) wrong = 1;
				}
			}
			if (i == cursor[0] && j == cursor[1]) printf("%s", highlightCol[(i / 3 + j / 3) % 2]);
			else if (wrong) printf("%s", wrongCol[(i / 3 + j / 3) % 2]);
			else printf("%s", col[(i / 3 + j / 3) % 2]);
			if (field[i][j] > 0) printf("%d ", field[i][j]);
			else printf("  ");
		}
		printf("\033[1;0m\n\r");
        }
	return 0;
}

int genSudoku(char field[9][9]) {
	int now = (int) time(0);
	for (int k = 0; k < now % 1000; k++) rand();
	char solveField[9][9];
	int done = 0;
	int tryCount = 0;
	while (1) {
		for (int i = 0; i < 9; i++) for (int j = 0; j < 9; j++) solveField[i][j] = field[i][j];
		done = solve(solveField);
		if (done == 1) break;
		int i = rand() % 9;
		int j = rand() % 9;
		if (done == -1) field[i][j] = 0;
		else if (done == 0) {
			char invalid[9] = {0};
			for (int k = 0; k < 9; k++) {
				if (k != j && field[i][k] > 0) invalid[field[i][k] - 1] = 1;
				if (k != i && field[k][j] > 0) invalid[field[k][j] - 1] = 1;
				int squareEntry[2] = {3 * (i / 3) + k % 3, 3 * (j / 3) + k / 3};
				if (!(squareEntry[0] == i && squareEntry[1] == j) && field[squareEntry[0]][squareEntry[1]] > 0) invalid[field[squareEntry[0]][squareEntry[1]] - 1] = 1;
			}
			int num = rand() % 9;
			int k = 0;
			while (k < 10 && invalid[num]) {
				num = (num + 1) % 9;
				k++;
			}
			if (k == 10) num = 0;
			else num++;
			field[i][j] = num;
		}
		tryCount++;
	}
	printf("found valid sudoku in %d iterations\n", tryCount);
	return 0;
}

int main() {
	char field[9][9] = {{0}};
	genSudoku(field);
	char orig[9][9];
	char solution[9][9];
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			solution[i][j] = field[i][j];
			orig[i][j] = field[i][j];
		}
	}
	solve(solution);
	int solved = 0;
	int cursorLoc[2] = {4, 4};
	int numcount[9] = {0};
	system("stty raw");
	while (!solved) {
		printf("\r");
		for (int k = 0; k < 9; k++) {
			if (numcount[k] > 9) printf("\033[1;31m");
			else printf("\033[1;0m");
			if (numcount[k] >= 9) printf("%d ", k+1);
			else printf("  ");
		}
		printf("\033[1;0m\n\r");
		printSudoku(field, cursorLoc);
		printf("\n\r");
		char action = getchar();
		while (action == 27 || action == '[') {
			action = getchar();
		}
		switch (action) {
			case 'A':
			case 'w':
				if (cursorLoc[0] > 0) cursorLoc[0]--;
				break;
			case 'B':
			case 's':
				if (cursorLoc[0] < 8) cursorLoc[0]++;
				break;
			case 'C':
			case 'd':
				if (cursorLoc[1] < 8) cursorLoc[1]++;
				break;
			case 'D':
			case 'a':
				if (cursorLoc[1] > 0) cursorLoc[1]--;
				break;
			case 'q':
				printf("\n\rreally quit? (y/N)\n\r");
				if (getchar() == 'y') {
					system("stty cooked");
					printf("\nok\n");
					return 0;
				}
				break;
			case 'r':
				printf("\n\rreally reset? (y/N)\n\r");
				if (getchar() == 'y') {
					for (int i = 0; i < 9; i++) for (int j = 0; j < 9; j++) field[i][j] = orig[i][j];
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
				if (!orig[cursorLoc[0]][cursorLoc[1]]) field[cursorLoc[0]][cursorLoc[1]] = action - 48;
				break;
			case ' ':
			case '0':
				field[cursorLoc[0]][cursorLoc[1]] = orig[cursorLoc[0]][cursorLoc[1]];
				break;
		}
		solved = 1;
		for (int k = 0; k < 9; k++) numcount[k] = 0;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (field[i][j] > 0) numcount[field[i][j] - 1]++;
				if (field[i][j] != solution[i][j]) solved = 0;
			}
		}
	}
	system("stty cooked");
	cursorLoc[0] = -1;
	cursorLoc[1] = -1;
	printf("\rSudoku Solved, congratulations!\n");
	printSudoku(solution, cursorLoc);
	return 0;
}
