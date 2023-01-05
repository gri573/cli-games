#include <stdlib.h>
#include "solitaire.h"

int abs(int x) {
	if (x < 0) return -x;
	return x;
}

void shuffle(int cards[52]) {
	for (int k = 0; k < 2000; k++) {
		int i1 = abs(rand()) % 52, i2 = abs(rand()) % 52;
		int c = cards[i1];
		cards[i1] = cards[i2];
		cards[i2] = c;
	}
	return;
}

int** deal(int cards[52]) {
	int** stacks = calloc(STC, sizeof(int*));
	for (int i = 0; i < 9; i++) {
		stacks[i] = calloc(52, sizeof(int));
	}
	stacks[9] = calloc(4, sizeof(int));
	stacks[10] = calloc(7, sizeof(int));
	int n = 0;
	for(int j = 0; j < 7; j++) {
		for (int i = j; i < 7; i++) {
			stacks[i][j] = cards[n];
			stacks[STC-1][i] = j;
			n++;
		}
	}
	int i = 0;
	for (; n < 52; i++) {
		stacks[7][i] = cards[n];
		n++;
	}
	return stacks;
}
