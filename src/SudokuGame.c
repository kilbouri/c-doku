#include <stdlib.h>
#include <stdio.h>

#include "SudokuGame.h"

#define EMPTY  ' '
#define CORNER '+'
#define VSEP   '-'
#define HSEP   '|'

// DO NOT MODIFY
#define WIDTH 9
// DO NOT MODIFY
#define HEIGHT 9

// This name is probably misleading. It represents
// the number of EMPTY spaces on BOTH sides of the value
// in a cell.
#define CELL_W 1

int getBoardIndex(SudokuGame, char, char);
void __putCell(char);
void __drawBoardVSepLine(char w);
char __checkRow(SudokuGame, char);
char __checkCol(SudokuGame, char);
char __checkBox(SudokuGame, char);

SudokuGame init() {
    char* board = (char*) malloc(WIDTH * HEIGHT);

    // init all board cells to be empty
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        board[i] = EMPTY;
    }

    return (SudokuGame) {
        board, 0, 0
    };
}

void freeGame(SudokuGame* state) {
    free(state->board);
}

void drawBoard(SudokuGame state) {
    for (int i = 0; i < HEIGHT; i++) {
        // draw top cell border line
        __drawBoardVSepLine(WIDTH);
        putchar('\n');

        // first cell left border HSEP
        putchar(HSEP);

        // draw each cell with its contents
        for (int j = 0; j < WIDTH; j++) {
            int idx = getBoardIndex(state, j, i);
            __putCell(state.board[idx]);
            putchar(HSEP);
        }

        putchar('\n');
    }

    // draw a final VSEP line
    __drawBoardVSepLine(WIDTH);
    putchar('\n');
}


char isComplete(SudokuGame state) {
    for (int i = 0; i < HEIGHT; i++) {
        if (!__checkRow(state, i))
            return 0;
    }

    for (int i = 0; i < WIDTH; i++) {
        if (!__checkCol(state, i))
            return 0;
    }

    for (int y = 0; y < HEIGHT; y += 3) {
        for (int x = 0; x < WIDTH; x += 3) {
            if (!__checkBox(state, getBoardIndex(state, x, y)))
                return 0;
        }
    }
    return 1;
}

int getBoardIndex(SudokuGame state, char x, char y) {
    return x + WIDTH * y;
}

void moveUp(SudokuGame* state) {
    if (state->ySel >= 1) {
        state->ySel--;
    }
}

void moveDown(SudokuGame* state) {
    if (state->ySel + 1 < HEIGHT) {
        state->ySel++;
    }
}

void moveLeft(SudokuGame* state) {
    if (state->xSel >= 1) {
        state->xSel--;
    }
}

void moveRight(SudokuGame* state) {
    if (state->xSel + 1 < WIDTH) {
        state->xSel++;
    }
}

void putDigit(SudokuGame* state, char digit) {
    int index = getBoardIndex(*state, state->xSel, state->ySel);
    state->board[index] = digit;
}

void clearDigit(SudokuGame* state) {
    putDigit(state, EMPTY);
}

int getExpectedCursorX(SudokuGame state) {
    /**
     * An example cell adjacency:
     * +---+---+
     * | x | y |
     * +---+---+
     *
     * We can see that to get to y from x, we need to move...
     * CELL_W, 1, CELL_W, 1.
     *
     * We must do this state.xSel times. Hence,
     * 2 * (state.xSel + 1) * (CELL_W + 1).
     *
     * Now, we must also push ourselves into the first cell
     * even when xSel is 0, so we add CELL_W + 2.
     */

    return 2 * (state.xSel) * (CELL_W + 1) + CELL_W + 2;
}

int getExpectedCursorY(SudokuGame state) {
    return 2 + state.ySel * 2;
}

// "Private" functions for SudokuGame
void __drawBoardVSepLine(char w) {
    for (int i = 0; i < w; i++) {
        putchar(CORNER);
        for (int i = 0; i <= 2 * CELL_W; i++)
            putchar(VSEP);
    }
    putchar(CORNER);
}

void __putCell(char content) {
    for (int i = 0; i < CELL_W; i++)
        putchar(EMPTY);

    putchar(content);

    for (int i = 0; i < CELL_W; i++)
        putchar(EMPTY);
}

/**
 * Traverses a row of the board, making sure all numbers appear
 * exactly once.
 */
char __checkRow(SudokuGame state, char row) {
    char visited[WIDTH] = { 0 };

    // count occurences of each digit
    char* rowArr = state.board + getBoardIndex(state, 0, row);
    for (int i = 0; i < WIDTH; i++) {
        char element = *(rowArr + i);
        if ('1' <= element && element <= '9') {
            visited[element - '1']++;
        } else return 0;
    }

    // ensure all counts are exactly 1
    for (int i = 0; i < WIDTH; i++) {
        if (visited[i] != 1)
            return 0;
    }

    return 1;
}

/**
 * Traverses a column of the board, making sure all numbers appear
 * exactly once.
 */
char __checkCol(SudokuGame state, char col) {
    char visited[HEIGHT] = { 0 };

    // count occurences of each digit
    for (int i = 0; i < HEIGHT; i++) {
        int colIdx = getBoardIndex(state, col, i);
        char element = *(state.board + colIdx);
        if ('1' <= element && element <= '9') {
            visited[element - '1']++;
        } else return 0;
    }

    // ensure all counts are exactly 1
    for (int i = 0; i < HEIGHT; i++) {
        if (visited[i] != 1)
            return 0;
    }

    return 1;
}

/**
 * Checks the 3x3 "box" whose top left corner is index topLeft.
 * Example (x = checked, o = ignored):
 * o o o o o
 * o x x x o <- the first x is index 6, so topLeft = 6
 * o x x x o
 * o x x x o
 * o o o o o
 */
char __checkBox(SudokuGame state, char topLeft) {
    int visited[9] = { 0 };

    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            // 3y + topLeft + x
            int checkIndex = (WIDTH * y) + topLeft + x;

            char element = *(state.board + checkIndex);
            if ('1' <= element && element <= '9') {
                visited[element - '1']++;
            } else return 0;
        }
    }

    // ensure all counts are exactly 1
    for (int i = 0; i < 9; i++) {
        if (visited[i] != 1)
            return 0;
    }

    return 1;
}
