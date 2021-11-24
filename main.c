#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#include "src/SudokuGame.h"

/**
 * TODO: Extract the input parsing code and raw mode code
 * into its own library
 */

// terminal cursor control
#define MOVE_TO(x,y) printf("\033[%d;%dH", y, x)
#define MOVE_00   "\033[H"
#define CLEAR     "\033[2J"
#define MOV_UP    "\033[A"
#define MOV_DOWN  "\033[B"
#define MOV_RIGHT "\033[C"
#define MOV_LEFT  "\033[D"

// input bindings (1-9 reserved for digits)
#define IPT_CLEAR 10
#define IPT_LEFT  11
#define IPT_RIGHT 12
#define IPT_UP    13
#define IPT_DOWN  14
#define IPT_QUIT  126
#define NON_IPT   127

void doFinishGame(SudokuGame*, char);
void awaitInput(char*, int);
char parseInput(char);
void performAction(SudokuGame*, char);
void redraw(SudokuGame);
void enableRawMode();
void disableRawMode();

// holds the initial termios state
struct termios termiosOriginalIn;

// game string recording file
FILE* recordFile;

int main(int argc, char const* argv[]) {
    // first, check if "--help" was requested
    if (argc == 2 && !strcmp("--help", argv[1])) {
        puts("C Sudoku (CDoku)");
        puts("Written by: Isaac Kilbourne\n");

        puts("Controls:");
        puts(" - WASD/HJKL to move between cells");
        puts(" - 1-9 to enter a digit");
        puts(" - Backspace/X to clear a space\n");

        puts("The game will end automatically when a valid solution is entered.");
        puts("Some presets are provided in the Quickstart section of the source repository.");

        exit(0);
    } else if (argc == 2 && !strcmp("--record", argv[1])) {
        // open or die
        recordFile = fopen("./recording.txt", "w");
        if (recordFile == NULL) {
            puts("Failed to open \"./recording.txt\". Terminating...");
            exit(0);
        }
    }

    enableRawMode();
    printf(CLEAR);

    // create a 9x9 game board
    SudokuGame gameState = init(9, 9);

    // sequence of actions for second argument if not --help and not --record
    if (argc == 2 && !recordFile) {
        for (int i = 0; *(argv[1] + i) != 0; i++) {
            performAction(&gameState, parseInput(*(argv[1] + i)));
        }
    }

    redraw(gameState);

    char input;
    for (;;) {
        awaitInput(&input, 1);
        performAction(&gameState, parseInput(input));
        redraw(gameState);
    }
}

/**
 * Called when the game should end.
 */
void doFinishGame(SudokuGame* state, char wasVictory) {
    printf("\x1b[?25l" MOVE_00);
    drawBoard(*state);
    printf("\x1b[?25h");

    if (wasVictory)
        printf("Congratulations, that's a valid solution!\n");
    else
        printf("Better luck next time.\n");
    printf("Thanks for playing my game!\n");

    freeGame(state);
    exit(0);
}

/**
 * Reads the first size bytes into buf. Assumes terminal is
 * in raw mode.
 */
void awaitInput(char* buf, int size) {
    int totalRead = 0;

    char* bufShifted;
    int remaining;
    while (totalRead < size) {
        bufShifted = buf + totalRead;
        remaining = size - totalRead;

        totalRead += read(STDIN_FILENO, bufShifted, remaining);
    }
}

/**
 * Determines the type of input given the character ipt.
 * Movement inputs may be WASD or HJKL
 */
char parseInput(char ipt) {
    char response;
    switch (tolower(ipt)) {
        case 'h':
        case 'a': response = IPT_LEFT;  break;

        case 'l':
        case 'd': response = IPT_RIGHT; break;

        case 'j':
        case 'w': response = IPT_UP;    break;

        case 'k':
        case 's': response = IPT_DOWN;  break;

        case 'q': response = IPT_QUIT;  break;

        case 127: // 127 is DEL
        case 'x': response = IPT_CLEAR; break;

        default:
            if ('1' <= ipt && ipt <= '9') {
                response = ipt - '0';
            } else response = NON_IPT;
    }

    if (recordFile && response != NON_IPT) {
        char buf[] = { ipt };
        fwrite(buf, 1, 1, recordFile);
    }

    return response;

}

/**
 * Performs the action indicated by char. See #defines at top of
 * this file for information about which char maps to which action.
 */
void performAction(SudokuGame* gameState, char action) {
    switch (action) {
        // handle exit and non-input cases
        case NON_IPT: break;
        case IPT_QUIT:
            doFinishGame(gameState, 0);

        // handle up/down/left/right
        case IPT_UP:    moveUp(gameState);    break;
        case IPT_DOWN:  moveDown(gameState);  break;
        case IPT_LEFT:  moveLeft(gameState);  break;
        case IPT_RIGHT: moveRight(gameState); break;

        case IPT_CLEAR:
            clearDigit(gameState);
            break;

        default:
            if (1 <= action && action <= 9) {
                // handle set digits
                putDigit(gameState, '0' + action);
                if (isComplete(*gameState))
                    doFinishGame(gameState, 1);
            }
            break;
    }
}

void redraw(SudokuGame gameState) {
    printf("\x1b[?25l" MOVE_00);
    drawBoard(gameState);

    MOVE_TO(getExpectedCursorX(gameState), getExpectedCursorY(gameState));
    printf("\x1b[?25h");
}

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &termiosOriginalIn);
}


void enableRawMode() {
    tcgetattr(STDIN_FILENO, &termiosOriginalIn);
    atexit(disableRawMode);

    struct termios raw = termiosOriginalIn;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    // disable STDOUT line buffering
    setvbuf(stdout, NULL, _IONBF, 0);
}
