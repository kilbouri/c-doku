CFLAGS=-Wall --pedantic

${NAME}: main.c src/SudokuGame.c
	$(CC) $(CFLAGS) -o SudokuGame.o src/SudokuGame.c -c -lm
	$(CC) ${CFLAGS} -o ${NAME}.exe main.c SudokuGame.o -lm

clean:
	rm -f ${NAME}.exe
	rm -f SudokuGame.o
