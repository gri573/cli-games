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
	system("stty raw");
	printf("Welcome to Solitaire!\n\rPress R for rules, Q to quit, any other key to begin the game.\n\r");
	char c = getchar();
	if (c > 96) c -= 32;
	if (c == 'Q') {
		system("stty cooked");
		return 0;
	}
	if (c == 'R') {
		printf("\n\rSolitaire is a single-player card sorting game.\n\rYou will begin with 7 stacks of cards, a closed stack, an initially empty open stack and 4 initially empty sorted stacks.\n\rThe goal of the game is to sort all cards into the sorted stacks.\n\n\rStack Properties:\n\r  Ordinary stacks - there may be any amount of covered cards at the bottom of these stacks.\n\r  They are followed by open cards in strictly descending order (e.g. 5, 4, 3, not 5, 4, 2 or 5, 5, 4)\n\r  the open cards must have alternating red and black colours.\n\r  If the topmost card of an ordinary stack is closed, it is immediately flipped open.\n\r  Sorted stacks - These stacks contain cards of the same suit in strictly ascending order, starting with ace (A).\n\r  There is one sorted stack for each suit.\n\r  Open stack - can only accept cards from the closed stack\n\r  Closed stack - Can only be refilled if empty, by flipping the open stack\n\n\rDuring each turn, you may move cards as follows:\n\r  1. Cycle through deck:\n\r     Transfer three cards from the closed stack to the open stack.\n\r     If less than three cards are available in the open stack, only transfer the available cards.\n\r     If no cards are available, optionally recycle (flip) open stack to become the closed stack.\n\r  2. Move one card from open stack\n\r     This card may go to any stack where it fulfils the stacking rules\n\r  3. Move any amount of open cards from an ordinary stack\n\r     These cards may go to any stack where they fulfil the stacking rules.\n\r     If more than one card is being moved,\n\r     the destination must be another ordinary stack as per stacking rules.\n\r  4. Move one card from a sorted stack to an ordinary stack.\n\n\rPress any key to continue: ");
		getchar();
	}
	printf("\n\r");
	// actual game
	int win = 0, quit = 0;
	while (!win && !quit) {
		printcards(stacks);
		quit = move(stacks);
		if (quit) {
			printf("\n\rReally quit? (y/N): ");
			char c = getchar();
			if (c > 96) c -= 32;
			if (c != 'Y') quit = 0;
			printf("\n\r");
		}
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
