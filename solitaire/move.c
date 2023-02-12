#include <stdlib.h>
#include <stdio.h>
#include "solitaire.h"

int move(int** stacks) {
	int quit = 0;
	printf("Please enter the stack from which you want to move cards (1-7, E-H, O, D, C, Q): ");
	char c = getchar();
	if (c > 96) c -= 32;
	switch (c) {
	case 'Q':
		{
			quit = 1;
			break;
		}
	case 'D':
		{
			int ndeck = -1;
			int nopen = 0;
			while (ndeck < 51 && stacks[7][ndeck+1]) ndeck++;
			while (nopen < 52 && stacks[8][nopen]) nopen++;
			if (ndeck == -1) {
				printf("\n\rNo card left in Deck! reshuffle?(Y/n) ");
				c = getchar();
				if (c == 'n' || c == 'N') {
					printf("\r");
					return move(stacks);
				}
				for (nopen--; nopen >= 0; nopen--) {
					ndeck++;
					stacks[7][ndeck] = stacks[8][nopen];
					stacks[8][nopen] = 0;
				}
			} else {
				for (int k = 0; k < 3; k++) {
					if (ndeck >= 0) {
						stacks[8][nopen] = stacks[7][ndeck];
						stacks[7][ndeck] = 0;
						ndeck--;
						nopen++;
					}
				}
			}
			break;
		}
	case 'O':
		{
			int nopen = -1;
			while (nopen < 51 && stacks[8][nopen+1]) nopen++;
			if (nopen == -1) {
				printcards(stacks);
				printf("\n\rNo card available in open deck!\n\r");
				return move(stacks);
			}
			char* possiblelocs = calloc(10, sizeof(char));
			int n = 0;
			for (int i = 0; i < 7; i++) {
				int j;
				for (j = -1; stacks[i][j+1]; j++);
				if (j == -1) {
					if ((stacks[8][nopen] - 1) % 13 == 12) {
						possiblelocs[n] = i + 49;
						n++;
					}
				} else if ((stacks[i][j] - 1) / 26 != (stacks[8][nopen] - 1) / 26 && (stacks[i][j] - 1) % 13 - (stacks[8][nopen] - 1) % 13 == 1) {
					possiblelocs[n] = i + 49;
					n++;
				}
			}
		    if ((stacks[8][nopen] - 1)%13 + 1 - stacks[9][(stacks[8][nopen] - 1)/13] == 1) possiblelocs[n] = (stacks[8][nopen] - 1)/13 + 69;
			if (!possiblelocs[0]) {
				printcards(stacks);
				printf("\n\rThis card doesn't fit anywhere!\n\r");
				return move(stacks);
			}
			printf("\n\rWhere do you want to move this card? (");
			for (int i = 0; possiblelocs[i]; i++) printf("%c, ", possiblelocs[i]);
			printf("C, Q)");
			int wrong = 1;
			while (wrong) {
				c = getchar();
				if (c > 96) c -= 32;
				if (c == 'C') {
					printf("\r");
					free(possiblelocs);
					printcards(stacks);
					return move(stacks);
				}
				if (c == 'Q') {
					quit = 1;
					break;
				}
				for (int i = 0; possiblelocs[i]; i++) if (c == possiblelocs[i]) wrong = 0;
				if (wrong) {
					printf("\n\rInvalid Input! please enter ");
					for (int i = 0; possiblelocs[i]; i++) printf("%c, ", possiblelocs[i]);
					printf("C or Q: ");
				}
			}
			if (quit == 1) break;
			if (c > '0' && c < '8') {
				int j = 0;
				for (; stacks[c-49][j]; j++);
				stacks[c-49][j] = stacks[8][nopen];
				stacks[8][nopen] = 0;
			}
			if (c > 64) {
				stacks[9][(stacks[8][nopen] - 1)/13]++;
				stacks[8][nopen] = 0;
			}
			free(possiblelocs);
			break;
		}
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
		{
			int stackn = c - 49;
			int stackh = -1;
			for (; stacks[stackn][stackh+1]; stackh++);
			if (stackh == -1) {
				printcards(stacks);
				printf("\n\rThere are no cards in that stack!\n\r");
				return move(stacks);
			}
			char cnum;
			int maxcnum = stackh - stacks[STC-1][stackn] + 1;
			if (maxcnum <= 0) {
				printf("\n\rOops, something went wrong, no open cards in non-empty stack!\n\r");
				return -1;
			}
			if (maxcnum == 1) cnum = 1;
			else {
				int wrong = 1;
				while (wrong) {
					printf("\n\rHow many cards do you want to move? (hexadecimal, 1-%d)", maxcnum);
					cnum = getchar();
					if (cnum > 96) cnum -= 32;
					while ((cnum < 49 || cnum > 57) && (cnum < 65 || cnum > 67)) {
						printf("\n\rInvalid input! please enter 1-9 or A-C (A=10, B=11, C=12): ");
						cnum = getchar();
						if (cnum > 96) cnum -= 32;
					}
					if (cnum > 64) cnum -= 7;
					cnum -= 48;
					if (cnum > maxcnum) printf("\n\rThere aren't enough open cards in that stack!");
					else wrong = 0;
				}
			}
			char* possiblelocs = calloc(10, sizeof(char));
			int card = stacks[stackn][stackh-cnum+1];
			int n = 0;
			for (int i = 0; i < 7; i++) {
				if (i == stackn) continue;
				int j = -1;
				for (; stacks[i][j+1]; j++);
				if (j == -1) {
					if ((card - 1) % 13 == 12) {
						possiblelocs[n] = i + 49;
						n++;
					}
				} else if ((stacks[i][j] - 1) / 26 != (card - 1) / 26 && (stacks[i][j] - 1) % 13 - (card - 1) % 13 == 1) {
					possiblelocs[n] = i + 49;
					n++;
				}
			}
		    if (cnum == 1 && (card - 1)%13 + 1 - stacks[9][(card - 1)/13] == 1) possiblelocs[n] = (card - 1)/13 + 69;
			if (!possiblelocs[0]) {
				printcards(stacks);
				printf("\n\rThis card stack cannot validly be moved anywhere!\n\r");
				return move(stacks);
			}
			if (cnum == 1) printf("\n\rWhere do you want to move this card? (");
			else printf("\n\rWhere do you want to move these cards? (");
			for (int k = 0; possiblelocs[k]; k++) printf("%c, ", possiblelocs[k]);
			printf("C, Q) :");
			int wrong = 1;
			while (wrong) {
				c = getchar();
				if (c > 96) c -= 32;
				for (int k = 0; possiblelocs[k]; k++) if (c == possiblelocs[k]) wrong = 0;
				if (c == 'C' || c == 'Q') wrong = 0;
				if (wrong) {
					printf("\n\rInvalid input! Please enter ");
					for (int k = 0; possiblelocs[k]; k++) printf("%c, ", possiblelocs[k]);
					printf("C or Q: ");
				}
			}
			if (c == 'Q') quit = 1;
			else if (c == 'C') {
				printf("\r");
				printcards(stacks);
				free(possiblelocs);
				return move(stacks);
			} else if (c > 48 && c < 56) {
				int dest = c - 49;
				int j = 0;
				for (; stacks[dest][j]; j++);
				for (int k = 0; k < cnum; k++) {
					stacks[dest][j+k] = stacks[stackn][stackh+k+1 - cnum];
					stacks[stackn][stackh+k+1 - cnum] = 0;
				}
			} else if (c > 68 && c < 73) {
				int dest = c - 69;
				stacks[9][dest]++;
				stacks[stackn][stackh] = 0;
			}
			if (stacks[STC-1][stackn] > stackh - cnum && stackh - cnum >= 0) {
				stacks[STC-1][stackn] = stackh - cnum;
				printf("\n\rNumber of closed cards in stack %d is now %d", stackn + 1, stackh - cnum);
			}
			free(possiblelocs);
			break;
		}
	case 'E':
	case 'F':
	case 'G':
	case 'H':
		{
			int stackn = c - 69;
			if (stacks[9][stackn] == 0) {
				printcards(stacks);
				printf("\n\rThat stack is empty!\n\r");
				return move(stacks);
			}
			char* possiblelocs = calloc(8, sizeof(char));
			int n = 0;
			for (int i = 0; i < 7; i++) {
				int j = -1;
				for (; stacks[i][j+1]; j++);
				if (j >= 0 && (stacks[i][j] - 1) / 26 != stackn / 2 && (stacks[i][j] - 1) % 13 + 1 - stacks[9][stackn] == 1) {
					possiblelocs[n] = i + 49;
					n++;
				}
			}
			printf("\n\rWhere do you want to move this card? (");
			for (int k = 0; possiblelocs[k]; k++) printf("%c, ", possiblelocs[k]);
			printf("C, Q) ");
			int wrong = 1;
			while (wrong) {
				c = getchar();
				if (c > 96) c -= 32;
				for (int k = 0; possiblelocs[k]; k++) if (c == possiblelocs[k]) wrong = 0;
				if (c == 'C' || c == 'Q') wrong = 0;
				if (wrong) {
					printf("\n\rInvalid Input! please enter ");
					for (int k = 0; possiblelocs[k]; k++) printf("%c, ", possiblelocs[k]);
					printf("C or Q: ");
				}
			}
			int j = 0;
			if (c == 'C') {
				printf("\r");
				return move(stacks);
			}
			if (c == 'Q') {
				printf("\n\r");
				quit = 1;
				break;
			}
			for (;stacks[c-49][j]; j++);
			stacks[c-49][j] = 13 * stackn + stacks[9][stackn];
			stacks[9][stackn]--;
			free(possiblelocs);
			break;
		}
	case 'P':
		{
			printf("\n\rCheat key pressed!! Do you really want to cheat? (y/N)");
			c = getchar();
			if (c > 96) c -= 32;
			if (c != 'Y') break;
			int j = 0;
			for (; stacks[7][j]; j++);
			if (j > 1) {
				int i1 = abs(rand()) % j;
				int i2 = abs(rand()) % j;
				int c0 = stacks[7][i1];
				stacks[7][i1] = stacks[7][i2];
				stacks[7][i2] = c0;
			}
			break;
		}
	}
	printf("\n\r");
	return quit;
}
