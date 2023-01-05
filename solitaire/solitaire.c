#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "solitaire.h"

// stacks = {to-remove stacks (7)[52], deck in hand[52], deck on table[52], build-up stacks (highest cards each)[4], number of unknown cards in to-remove stacks[7]}

// MAIN LOOP
int main() {
	// make rand() a little more random
	int now = (int) time(0) % 10000;
	for (int i = 0; i < now; i++) rand();

	// initialise cards
	int cards[52];
	for (int i = 0; i < 52; i++) cards[i] = i + 1;
	shuffle(cards);

	// get the dealt card stacks
	int** stacks = deal(cards);

	// actual game
	system("stty raw");
	int win = 0, quit = 0;
	while (!win && !quit) {
		printcards(stacks);
		quit = move(stacks);
		if (stacks[7][0] == 0 && stacks[8][0] == 0) {
			win = 1;
			for (int i = 0; i < 7; i++) if (stacks[STC-1][i] > 0) win = 0;
		}
	}
	system("stty cooked");
	if (win) printf("Won!\n");
	else if (quit) printf("Quit!\n");
	//free card stacks from heap
	for (int i = 0; i < STC; i++) free((void*) stacks[i]);
	free((void*) stacks);
	return 0;
}
