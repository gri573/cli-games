#ifndef SOLITAIRE_H
#define SOLITAIRE_H

#define STC 11

int abs(int x);

void shuffle(int cards[52]);

int** deal(int cards[52]);

void printcards(int** stacks);

int move(int** stacks);

#endif
