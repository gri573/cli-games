#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (field[i][j] == 0) return 0;
		}
	}
	return 1;
}

int solveHard(char field[9][9]) {
	unsigned int possibilities[9][9];
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			possibilities[i][j] = 0x1ffu;
		}
	}
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (field[i][j]) {
				unsigned int mask = ~(1<<(field[i][j]-1));
				int coarsei = i/3*3;
				int coarsej = j/3*3;
				for (int k = 0; k < 9; k++) {
					possibilities[i][k] &= mask;
					possibilities[k][j] &= mask;
					possibilities[coarsei+k%3][coarsej+k/3] &= mask;
				}
				possibilities[i][j] = ~mask;
			}
		}
	}
	int totalPossible = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			for (int k = 0; k < 9; k++) {
				totalPossible += !!(possibilities[i][j] & (1<<k));
			}
		}
	}
	for (int iter = 0; 1; iter++) {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (field[i][j]) continue;
				if (possibilities[i][j] == 0) {
					printf("no possibles at (%d, %d)!\n", i, j);
					int cursor[2];
					cursor[0] = i;
					cursor[1] = j;
					printSudoku(field, cursor);
					printf("\n");
					return -1;
				}
				unsigned int iMask = 0;
				unsigned int jMask = 0;
				unsigned int tileMask = 0;
				int coarsei = i/3*3;
				int coarsej = j/3*3;
				for (int k = 0; k < 9; k++) {
					if (k != j) {
						iMask |= possibilities[i][k];
					}
					if (k != i) {
						jMask |= possibilities[k][j];
					}
					int newi = coarsei + k%3;
					int newj = coarsej + k/3;
					if (newi != i || newj != j) {
						tileMask |= possibilities[newi][newj];
					}
				}
				unsigned int exclusivePossible = possibilities[i][j] & ~(iMask & jMask & tileMask);
				if (exclusivePossible) {
					char val = 0;
					for (int k = 0; k < 9; k++) {
						if (exclusivePossible & (1<<k)) {
							if (val) return -1;
							val = k+1;
						}
					}
					field[i][j] = val;
					possibilities[i][j] = 1<<(val-1);
				}
				signed char baseval = 0;
				for (int k = 0; k < 9; k++) {
					if (possibilities[i][j] & (1<<k)) {
						if (baseval) baseval = -1;
						else baseval = k+1;
					}
				}
				if (baseval > 0 && !field[i][j]) {
					field[i][j] = baseval;
				}
			}
		}
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (field[i][j]) {
					unsigned int mask = ~(1<<(field[i][j]-1));
					int coarsei = i/3*3;
					int coarsej = j/3*3;
					for (int k = 0; k < 9; k++) {
						possibilities[i][k] &= mask;
						possibilities[k][j] &= mask;
						possibilities[coarsei+k%3][coarsej+k/3] &= mask;
					}
					possibilities[i][j] = ~mask;
				}
			}
		}
		for (int j = 0; j < 9; j++) {
			for (int k = 0; k < 9; k++) {
				int coarsei = 3;
				for (int i = 0; i < 9; i++) {
					if (possibilities[i][j] & (1<<k)) {
						if (coarsei == 3) {
							coarsei = i/3;
						} else if (coarsei != i/3) {
							coarsei = -1;
						}
					}
				}
				if (coarsei == 3) return -1;
				if (coarsei >= 0) {
					int coarsej = j/3*3;
					coarsei *= 3;
					for (int l = 0; l < 6; l++) {
						int newj = coarsej + (j+l/3+1)%3;
						int newi = coarsei + l%3;
						if (field[newi][newj]) continue;

						possibilities[newi][newj] &= ~(1<<k);
					}
				}
			}
		}

		for (int i = 0; i < 9; i++) {
			for (int k = 0; k < 9; k++) {
				int coarsej = 3;
				for (int j = 0; j < 9; j++) {
					if (possibilities[i][j] & (1<<k)) {
						if (coarsej == 3) {
							coarsej = j/3;
						} else if (coarsej != j/3) {
							coarsej = -1;
						}
					}
				}
				if (coarsej == 3) return -1;
				if (coarsej >= 0) {
					int coarsei = i/3*3;
					coarsej *= 3;
					for (int l = 0; l < 6; l++) {
						int newi = coarsei + (i+l/3+1)%3;
						int newj = coarsej + l%3;
						if (field[newi][newj]) continue;

						possibilities[newi][newj] &= ~(1<<k);
					}
				}
			}
		}

		for (int coarsei = 0; coarsei < 9; coarsei += 3) {
			for (int coarsej = 0; coarsej < 9; coarsej += 3) {
				for (int k = 0; k < 9; k++) {
					int exclusivei = 9;
					int exclusivej = 9;
					for (int i = coarsei; i < coarsei + 3; i++) {
						for (int j = coarsej; j < coarsej + 3; j++) {
							if (possibilities[i][j] & (1<<k)) {
								if (exclusivei == 9) {
									exclusivei = i;
									exclusivej = j;
								}
								if (exclusivei != i) {
									exclusivei = -1;
								}
								if (exclusivej != j) {
									exclusivej = -1;
								}
							}
						}
					}
					if (exclusivei == 9) return -1;
					if (exclusivei >= 0) {
						for (int j = 0; j < 9; j++) {
							if (j/3*3 == coarsej || field[exclusivei][j]) continue;

							possibilities[exclusivei][j] &= ~(1<<k);
						}
					}
					if (exclusivej >= 0) {
						for (int i = 0; i < 9; i++) {
							if (i/3*3 == coarsei || field[i][exclusivej]) continue;

							possibilities[i][exclusivej] &= ~(1<<k);
						}
					}
				}
			}
		}
		int oldPossible = totalPossible;
		totalPossible = 0;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {

				for (int k = 0; k < 9; k++) {
					totalPossible += !!(possibilities[i][j] & (1<<k));
				}
			}

		}
		if (oldPossible == totalPossible) {
			break;
		}
	}
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (field[i][j] == 0) return 0;
		}
	}
	return 1;
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

int hardenSudoku(char field[9][9], int hardness) {
	char solveField[9][9];
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			char prevEntry = field[i][j];
			if (!prevEntry) continue;
			field[i][j] = 0;
			for (int k = 0; k < 9; k++) for (int l = 0; l < 9; l++) solveField[k][l] = field[k][l];
			int done;
			if (hardness == 0) done = solve(solveField);
			else done = solveHard(solveField);
			if (done != 1) field[i][j] = prevEntry;
			// else printf("removed %d at (%d, %d)\n", prevEntry, i, j);
		}
	}
	printf("Hardened Sudoku to level %d\n", hardness + 1);
	return 0;
}

int main(int argc, char** argv) {
	char field[9][9] = {{0}};
	genSudoku(field);
	char showCorrect = 1;
	for (int arg = 1; arg < argc; arg++) {
		char hard = 1;
		for (int i = 0; argv[1][i] && i < 3; i++) if (argv[1][i] != "-H"[i]) hard = 0;
		if (!hard) {
			hard = 1;
			for (int i = 0; argv[1][i] && i < 6; i++) if (argv[1][i] != "--hard"[i]) hard = 0;
		}
		char extraHard = 1;
		for (int i = 0; argv[1][i] && i < 4; i++) if (argv[1][i] != "-EH"[i]) extraHard = 0;
		if (!extraHard) {
			extraHard = 1;
			for (int i = 0; argv[1][i] && i < 11; i++) if (argv[1][i] != "--extrahard"[i]) extraHard = 0;
		}
		for (int i = 0; argv[1][i] && i < 4; i++) if (argv[1][i] != "-c"[i]) showCorrect = 0;
		if (!showCorrect) {
			showCorrect = 1;
			for (int i = 0; argv[1][i] && i < 11; i++) if (argv[1][i] != "--showcorrect"[i]) showCorrect = 0;
		}

		if (hard || extraHard) hardenSudoku(field, 0);
		if (extraHard) hardenSudoku(field, 1);
	}
	char orig[9][9];
	char solution[9][9];
	char lastCorrect[9][9];
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			solution[i][j] = field[i][j];
			orig[i][j] = field[i][j];
			lastCorrect[i][j] = field[i][j];
		}
	}
	solve(solution);
	int solved = 0;
	int correct = 1;
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
				printf("\n\rreally reset to last correct state? (y/N)\n\r");
				if (getchar() == 'y') {
					for (int i = 0; i < 9; i++) for (int j = 0; j < 9; j++) field[i][j] = lastCorrect[i][j];
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
				if (!orig[cursorLoc[0]][cursorLoc[1]]) {
					field[cursorLoc[0]][cursorLoc[1]] = action - '0';
					correct = 1;
					for (int x = 0; x < 9; x++) {
						for (int y = 0; y < 9; y++) {
							if (
								field[x][y] &&
								field[x][y] !=
								solution[x][y]
								)
								correct = 0;
							if (!correct) break;
						}
						if (!correct) break;
					}
					if (correct) {
						lastCorrect[cursorLoc[0]][cursorLoc[1]] = action - '0';
						if (showCorrect) printf("Correct!\n\r");
					} else if (showCorrect) printf("Not correct!\n\r");
				}
				break;
			case ' ':
			case '0':
				correct = 1;
				field[cursorLoc[0]][cursorLoc[1]] = orig[cursorLoc[0]][cursorLoc[1]];
				for (int x = 0; x < 9; x++) {
					for (int y = 0; y < 9; y++) {
						if (
							field[x][y] &&
							field[x][y] !=
							solution[x][y]
							)
							correct = 0;
						if (!correct) break;
					}
					if (!correct) break;
				}
				if (correct) {
					for (int x = 0; x < 9; x++) {
						for (int y = 0; y < 9; y++) {
							lastCorrect[x][y] = field[x][y];
						}
					}
				}
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
