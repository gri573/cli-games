all: 2048 fp sudoku

sudoku: sudoku/sudoku.c
	$(CC) $(CFLAGS) -o sudoku/sudoku.exe $?

2048: 2048/2048.c
	$(CC) $(CFLAGS) -o 2048/2048.exe $?

fp: fp/fp.c
	$(CC) $(CFLAGS) -o fp/fp.exe $?
