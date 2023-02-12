#include <stdio.h>
#include <stdlib.h>
#include "solitaire.h"
#define IMHEIGHT 30
#define IMWIDTH 300

void printcards(int** stacks) {
	char*** cardfaces = calloc(5, sizeof(char**));
	for (int i = 0; i < 5; i++) {
		cardfaces[i] = calloc(6, sizeof(char*));
	}

	cardfaces[0][0] = " ---- ";
	cardfaces[0][1] = "| \033[1;31m/\\\033[1;0m |";
	cardfaces[0][2] = "|\033[1;31m(  )\033[1;0m|";
	cardfaces[0][3] = "| \033[1;31m\\/\033[1;0m |";
	cardfaces[0][4] = "|    |";
	cardfaces[0][5] = " ---- ";

	cardfaces[1][0] = " ---- ";
	cardfaces[1][1] = "|\033[1;31m^ ^\033[1;0m |";
	cardfaces[1][2] = "|\033[1;31m\\ /\033[1;0m |";
	cardfaces[1][3] = "| \033[1;31mV\033[1;0m  |";
	cardfaces[1][4] = "|    |";
	cardfaces[1][5] = " ---- ";

	cardfaces[2][0] = " ---- ";
	cardfaces[2][1] = "| O  |";
	cardfaces[2][2] = "|O O |";
	cardfaces[2][3] = "| ^  |";
	cardfaces[2][4] = "|    |";
	cardfaces[2][5] = " ---- ";

	cardfaces[3][0] = " ---- ";
	cardfaces[3][1] = "| A  |";
	cardfaces[3][2] = "|/ \\ |";
	cardfaces[3][3] = "|°^° |";
	cardfaces[3][4] = "|    |";
	cardfaces[3][5] = " ---- ";

	cardfaces[4][0] = " ---- ";
	cardfaces[4][1] = "|    |";
	cardfaces[4][2] = "|    |";
	cardfaces[4][3] = "|    |";
	cardfaces[4][4] = "|    |";
	cardfaces[4][5] = " ---- ";

	char** valnames = calloc(14, sizeof(char*));
	valnames[0] = "";
	valnames[1] = "A";
	valnames[2] = "2";
	valnames[3] = "3";
	valnames[4] = "4";
	valnames[5] = "5";
	valnames[6] = "6";
	valnames[7] = "7";
	valnames[8] = "8";
	valnames[9] = "9";
	valnames[10] = "10";
	valnames[11] = "J";
	valnames[12] = "Q";
	valnames[13] = "K";
	
	char** image = calloc(IMHEIGHT, sizeof(char*));
	for (int i = 0; i < IMHEIGHT; i++) {
		image[i] = calloc(IMWIDTH + 1, sizeof(char));
	}
	int* ns = calloc(IMHEIGHT, sizeof(int));
	// buildup stacks
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			for (int k = 0; k < 3; k++) {
				image[j][ns[j]] = ' ';
				ns[j]++;
			}
			for (int k = 0; cardfaces[i][j][k] != 0; k++) {
				image[j][ns[j]] = cardfaces[i][j][k];
				ns[j]++;
			}
		}
		for (int k = 0; valnames[stacks[9][i]][k] != 0; k++) {
			image[4][ns[4]+k-3] = valnames[stacks[9][i]][k]; 
		}
	}
	// to-remove stacks
	for (int i = 0; i < 7; i++) {
		int stackheight = -1;
		for (int j = 8; j < IMHEIGHT; j++) {
			int j0 = j - 8;
			for (int k = 0; k < 3; k++) {
				image[j][ns[j]] = ' ';
				ns[j]++;
			}
			if (stacks[i][j0]) {
				stackheight = j0;
				if (j0 < stacks[STC-1][i]) {
					for (int k = 0; "\033[1;34m"[k]; k++) {
						image[j][ns[j]] = "\033[1;34m"[k];
						ns[j]++;
					}
				}
				if (j0 == 0) {
					for (int k = 0; k < 6; k++) {
						image[j][ns[j]] = " ---- "[k];
						ns[j]++;
					}
				} else {
					if (j0 == stacks[STC-1][i]) {
						for (int k = 0; "\033[1;34m|\033[1;0m----\033[1;34m|\033[1;0m"[k]; k++) {
							image[j][ns[j]] = "\033[1;34m|\033[1;0m----\033[1;34m|\033[1;0m"[k];
							ns[j]++;
						}
					} else {
						for (int k = 0; k < 6; k++) {
							image[j][ns[j]] = "|----|"[k];
							ns[j]++;
						}
					}
				}
				if (j0 < stacks[STC-1][i]) {
					for (int k = 0; "\033[1;0m"[k]; k++) {
						image[j][ns[j]] = "\033[1;0m"[k];
						ns[j]++;
					}
				}
				} else {
				int j1 = j0 - stackheight;
				if (stackheight < 0) j1--;
				if (j1 < 6) {
					if (stackheight >= 0) {
						for (int k = 0; cardfaces[(stacks[i][stackheight]-1)/13][j1][k]; k++) {
							image[j][ns[j]] = cardfaces[(stacks[i][stackheight]-1)/13][j1][k];
							ns[j]++;
						}
					} else {
						for (int k = 0; cardfaces[4][j1][k]; k++) {
							image[j][ns[j]] = cardfaces[4][j1][k];
							ns[j]++;
						}
					}
				} else {
					for (int k = 0; k < 6; k++) {
						image[j][ns[j]] = ' ';
						ns[j]++;
					}
				}
			}
		}

		int j = 8 + stackheight + 4;
		for (int k = 0; valnames[(stacks[i][stackheight]-1)%13 + 1][k]; k++) image[j][ns[j]-3+k] = valnames[(stacks[i][stackheight]-1)%13 + 1][k];
	}
	// open deck
	int deckcol = 4;
	int deckval = 0;
	for (int j = 0; j < 52 && stacks[8][j]; j++) {
		deckcol = (stacks[8][j]-1) / 13;
		deckval = (stacks[8][j]-1) % 13 + 1;
	}
	for (int j = 0; j < 6; j++) {
		for (int k = 0; k < 8; k++) {
			image[j][ns[j]] = ' ';
			ns[j]++;
			}
		for (int k = 0; cardfaces[deckcol][j][k]; k++) {
			image[j][ns[j]] = cardfaces[deckcol][j][k];
			ns[j]++;
		}
	}
	for (int k = 0; valnames[deckval][k]; k++) image[4][ns[4]+k-3] = valnames[deckval][k];
	// closed deck
	for (int j = 0; j < 6; j++) {
		for (int k = 0; k < 5; k++) {
			image[j][ns[j]] = ' ';
			ns[j]++;
			}
		for (int k = 0; "\033[1;34m"[k]; k++) {
			image[j][ns[j]] = "\033[1;34m"[k];
			ns[j]++;
		}
		for (int k = 0; cardfaces[4][j][k]; k++) {
			image[j][ns[j]] = cardfaces[4][j][k];
			ns[j]++;
		}
		for (int k = 0; "\033[1;0m"[k]; k++) {
			image[j][ns[j]] = "\033[1;0m"[k];
			ns[j]++;
		}
	}
	// stack symbols
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 5; k++) {
			image[6][ns[6]] = ' ';
			ns[6]++;
		}
		image[6][ns[6]] = i + 69;
		ns[6]++;
		for (int k = 0; k < 3; k++) {
			image[6][ns[6]] = ' ';
			ns[6]++;
		}
	}
	for (int k = 0; k < 10; k++) {
		image[6][ns[6]] = ' ';
		ns[6]++;
	}
	image[6][ns[6]] = 'O';
	ns[6]++;
	for (int k = 0; k < 10; k++) {
		image[6][ns[6]] = ' ';
		ns[6]++;
	}
	image[6][ns[6]] = 'D';
	ns[6]++;
	for (int i = 0; i < 7; i++) {
		for (int k = 0; k < 5; k++) {
			image[7][ns[7]] = ' ';
			ns[7]++;
		}
		image[7][ns[7]] = i + 49;
		ns[7]++;
		for (int k = 0; k < 3; k++) {
			image[7][ns[7]] = ' ';
			ns[7]++;
		}
	}
	for (int k = 0; k < 10; k++) {
		image[6][ns[6]] = ' ';
		ns[6]++;
	}
	for (int k = 0; k < 5; k++) {
		image[7][ns[7]] = ' ';
		ns[7]++;
	}
	for (int k = 0; "C: Cancel Input"[k]; k++) {
		image[6][ns[6]] = "C: Cancel Input"[k];
		ns[6]++;
	}
	for (int k = 0; "Q: Quit Game"[k]; k++) {
		image[7][ns[7]] = "Q: Quit Game"[k];
		ns[7]++;
	}
	// print to stdout and free everything
	for (int i = 0; i < IMHEIGHT; i++) {
		printf("%s\n\r", image[i]);
		free(image[i]);
	}
	free(image);
	
	for (int i = 0; i < 5; i++) {
		free(cardfaces[i]);
	}
	free(cardfaces);

	free(valnames);
	
	free(ns);
}
