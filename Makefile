CFLAGS += -ggdb -Wall -Wextra
CFLAGS += -fsanitize=address
#ncurses is only needed for jumper
LDFLAGS += -lncurses
LDFLAGS += -lm

all: 2048 fp sudoku solitaire jumper colorsort

sudoku: sudoku/sudoku.exe

2048: 2048/2048.exe

fp: fp/fp.exe fp/mkimg.exe

solitaire: solitaire/solitaire.exe

jumper : jumper/jumper.exe

colorsort : colorsort/colorsort.exe

puyo : puyo/puyo.exe

sudoku/sudoku.exe: sudoku/sudoku.c
	$(CC) $(CFLAGS) -o sudoku/sudoku.exe $?

2048/2048.exe: 2048/2048.c
	$(CC) $(CFLAGS) -o 2048/2048.exe $?

fp/fp.exe: fp/fp.c
	$(CC) $(CFLAGS) -o fp/fp.exe $?

fp/mkimg.exe: fp/mkimg.c
	$(CC) $(CFLAGS) -o $@ $?

SOLITAIRE_C = $(wildcard solitaire/*.c)
SOLITAIRE_O = $(SOLITAIRE_C:.c=.o)

solitaire/solitaire.exe: $(SOLITAIRE_O)
	$(CC) $(CFLAGS) -o $@ $(SOLITAIRE_O)

jumper/jumper.exe: jumper/jumper.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

colorsort/colorsort.exe: colorsort/colorsort.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

puyo/puyo.exe: puyo/puyo.c
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

clean:
	rm -f */*.exe */*.o
