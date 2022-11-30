CFLAGS += -Wall -Wextra

all: 2048 fp sudoku

sudoku: sudoku/sudoku.exe

2048: 2048/2048.exe

fp: fp/fp.exe

sudoku/sudoku.exe: sudoku/sudoku.c
	$(CC) $(CFLAGS) -o sudoku/sudoku.exe $?

2048/2048.exe: 2048/2048.c
	$(CC) $(CFLAGS) -o 2048/2048.exe $?

fp/fp.exe: fp/fp.c
	$(CC) $(CFLAGS) -o fp/fp.exe $?

clean:
	rm -f sudoku/sudoku.exe 2048/2048.exe fp/fp.exe
