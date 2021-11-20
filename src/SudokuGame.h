#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

// "Public" types/functions for SudokuGame

typedef struct {
    char* board;
    char xSel;
    char ySel;
} SudokuGame;

SudokuGame init();
void freeGame(SudokuGame* state);

void drawBoard(SudokuGame);
char isComplete(SudokuGame);

void moveUp(SudokuGame*);
void moveDown(SudokuGame*);
void moveLeft(SudokuGame*);
void moveRight(SudokuGame*);

int getExpectedCursorX(SudokuGame);
int getExpectedCursorY(SudokuGame);

void putDigit(SudokuGame*, char digit);
void clearDigit(SudokuGame*);
#endif
