CFLAGS += -Wall -Wextra

all: 2048 fp sudoku

sudoku: sudoku/sudoku.exe

2048: 2048/2048.exe

fp: fp/fp.exe fp/mkimg.exe

sudoku/sudoku.exe: sudoku/sudoku.c
	$(CC) $(CFLAGS) -o sudoku/sudoku.exe $?

2048/2048.exe: 2048/2048.c
	$(CC) $(CFLAGS) -o 2048/2048.exe $?

fp/fp.exe: fp/fp.c
	$(CC) $(CFLAGS) -o fp/fp.exe $?

fp/mkimg.exe: fp/mkimg.c
	$(CC) $(CFLAGS) -o fp/mkimg.exe $?

clean:
	rm -f sudoku/sudoku.exe 2048/2048.exe fp/fp.exe
