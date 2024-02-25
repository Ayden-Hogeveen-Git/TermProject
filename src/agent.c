#include <stdio.h>
#include <stdlib.h>
#include "structures.h"


// Function prototypes
int minValue(Board* board, int playersTurn, ValidMoves validMoves);
int maxValue(Board* board, int playersTurn, ValidMoves validMoves);

//int minValue(Board* board, int playersTurn, ValidMoves validMoves, int alpha, int beta)
//int maxValue(Board* board, int playersTurn, ValidMoves validMoves, int alpha, int beta)



void printBoard(Board board) {
    for (int x=0; x<8; x++) {
        for (int y=0; y<8; y++) {
            printf("%c ", board.state[x][y]);
        }
        printf("\n");
    }
}

void makeFirstMove(Board* board, Point point) {
    // Convert the x coordinates from A-H to 0-7
    int x = point.x - 'A';

    // Convert the y coordinates from 1-8 to 0-7
    int y = point.y - 1;

    // Make the move
    board->state[x][y] = 'O';
}

void makeMove(Board* board, Move move) {
    // Convert the x coordinates from A-H to 0-7
    int oldX = move.start.x - 'A';
    int newXIndex = move.end.x - 'A';

    // Convert the y coordinates from 1-8 to 0-7
    int oldY = move.start.y - 1;
    int newYIndex = move.end.y - 1;

    // Make the move
    board->state[newXIndex][newYIndex] = board->state[oldX][oldY];
    board->state[oldX][oldY] = 'O';
    board->state[(oldX + newXIndex) / 2][(oldY + newYIndex) / 2] = 'O';

    // board->state[newX][newY] = board->state[x][y];
    // board->state[x][y] = 'O';
    // board->state[(x + newX) / 2][(y + newY) / 2] = 'O';
}

int isValidFirstMove(Board* board, int playersTurn, Point point) {
    // Convert the x coordinates from A-H to 0-7
    int x = point.x - 'A';

    // Convert the y coordinates from 1-8 to 0-7
    int y = point.y - 1;

    // Check if the start coordinates are within the game starting area
    if (x < 4 || x > 5 || y < 4 || y > 5) {
        return 0;
    }

    // Check if the player is moving their own piece
    if (playersTurn == 0 && board->state[x][y] != 'B') {
        return 0;
    } else if (playersTurn == 1 && board->state[x][y] != 'W') {
        return 0;
    }

    return 1;
}

int isValidMove(Board* board, int playersTurn, Move move) {
    // Convert the x coordinates from A-H to 0-7
    int x = move.start.x - 'A';
    int newX = move.end.x - 'A';

    // Convert the y coordinates from 1-8 to 0-7
    int y = move.start.y - 1;
    int newY = move.end.y - 1;

    // Check if the start coordinates are within the game board
    if (x < 0 || x > 7 || y < 0 || y > 7) {
        return 0;
    }

    // Check if the end coordinates are within the game board
    if (newX < 0 || newX > 7 || newY < 0 || newY > 7) {
        return 0;
    }

    // Check if the piece is moving to an empty space
    if (board->state[newX][newY] != 'O') {
        return 0;
    }

    // Check if the piece is moving to a valid space
    if (abs(newX - x) != 2 && abs(newY - y) != 2) {
        return 0;
    }

    // Check if the player is moving their own piece
    if (playersTurn == 0 && board->state[x][y] != 'B') {
        return 0;
    } else if (playersTurn == 1 && board->state[x][y] != 'W') {
        return 0;
    }

    return 1;
}

void addValidMove(ValidMoves* validMoves, Move move) {
    // If the valid moves array is full, double the capacity
    if (validMoves->size == validMoves->capacity) {
        validMoves->capacity *= 2;
        validMoves->moves = realloc(validMoves->moves, validMoves->capacity * sizeof(Move));
    }

    // Add the valid move to the array
    validMoves->moves[validMoves->size] = move;
    validMoves->size++;
}

ValidMoves findValidMoves(Board* board, int playersTurn) {
    // Initialize the valid moves array
    ValidMoves validMoves;
    validMoves.capacity = 10;

    // Allocate memory for valid moves array
    validMoves.moves = malloc(validMoves.capacity * sizeof(Move));
    
    // Check if memory allocation failed or not
    if (validMoves.moves == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    // Initialize valid moves array
    validMoves.size = 0;

    // Find valid moves
    int index = 0;
    for (int x=0; x<8; x++) {
        for (int y=0; y<8; y++) {
            if (board->state[x][y] == 'B') {
                // Check if the piece can move to the left
                Move moveLeft = {{x, y}, {x-2, y}};
                if (isValidMove(board, playersTurn, moveLeft) == 1) {
                    addValidMove(&validMoves, moveLeft);
                    index++;
                }

                // Check if the piece can move to the right
                Move moveRight = {{x, y}, {x+2, y}};
                if (isValidMove(board, playersTurn, moveRight) == 1) {
                    addValidMove(&validMoves, moveRight);
                    index++;
                }

                // Check if the piece can move up
                Move moveUp = {{x, y}, {x, y+2}};
                if (isValidMove(board, playersTurn, moveUp) == 1) {
                    addValidMove(&validMoves, moveUp);
                    index++;
                }

                // Check if the piece can move down
                Move moveDown = {{x, y}, {x, y-2}};
                if (isValidMove(board, playersTurn, moveDown) == 1) {
                    addValidMove(&validMoves, moveDown);
                    index++;
                }
            }
        }
    }

    // Return valid moves array
    return validMoves;
}

Board* minMaxResult(Board* board, Move move) {
    // Make the move
    makeMove(board, move);

    // Return the new state
    return board;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

/*Alpha-Beta for minValue*/
/*
int minValue(Board* board, int playersTurn, ValidMoves validMoves, int alpha, int beta) {

    // If terminal state, return utility value of 0
    if (findValidMoves(board, playersTurn).size == 0) {
        return 0;
    }

    // Loop through valid moves
    for (int i = 0; i < validMoves.size; i++) {
        // Get the next state
        Board* nextState = minMaxResult(board, validMoves.moves[i]);

        // Get the max value
        int v = maxValue(nextState, findValidMoves(nextState), alpha, beta);

        // Update beta
        beta = min(beta, v);

        // Perform pruning
        if (beta <= alpha) {
            return beta;
        }
    }
    return beta;
}
*/
/*Alpha-Beta for maxValue*/
/*
int maxValue(Board* board, int playersTurn, ValidMoves validMoves, int alpha, int beta) {

    // If terminal state, return utility value of 0
    if (findValidMoves(board, playersTurn).size == 0) {
        return 0;
    }

    // Loop through valid moves
    for (int i = 0; i < validMoves.size; i++) {
        // Get the next state
        Board* nextState = minMaxResult(board, validMoves.moves[i]);

        // Get the min value
        int v = minValue(nextState, findValidMoves(nextState), alpha, beta);

        // Update alpha
        alpha = max(alpha, v);

        // Perform pruning
        if (alpha >= beta) {
            return alpha;
        }
    }
    return alpha;
}
*/

int minValue(Board* board, int playersTurn, ValidMoves validMoves) {
    /*
    function MIN-VALUE(state) returns a utility value
     if TERMINAL-TEST(state) then return UTILITY(state)
     v ← ∞
     for a,s in SUCCESSORS(state) do
     v ← MIN(v,MAX-VALUE(s))
     return v
    */

    // If terminal state, return utility value of 0
    if (findValidMoves(board, playersTurn).size == 0) {
        return 0;
    }

    // Initialize v to positive infinity
    int v = 1000;

    // Loop through valid moves
    for (int i=0; i<validMoves.size; i++) {
        // Get the next state
        Board* nextState = minMaxResult(board, validMoves.moves[i]);

        // Get the max value
        v = min(v, maxValue(nextState, playersTurn, findValidMoves(nextState, playersTurn)));
    }

    // Return v
    return v;
}

int maxValue(Board* board, int playersTurn, ValidMoves validMoves) {
    /*
    function MAX-VALUE(state) returns a utility value
     if TERMINAL-TEST(state) then return UTILITY(state)
     v ← - ∞
     for a,s in SUCCESSORS(state) do
     v ← MAX(v,MIN-VALUE(s))
     return v
     */

    // If terminal state, return utility value
    if (findValidMoves(board, playersTurn).size == 0) {
        return 0;
    }

    // Initialize v to negative infinity
    int v = -1000;

    // Loop through valid moves
    for (int i=0; i<validMoves.size; i++) {
        // Get the next state
        Board* nextState = minMaxResult(board, validMoves.moves[i]);

        // Get the min value
        v = max(v, minValue(nextState, playersTurn, findValidMoves(nextState, playersTurn)));
    }

    // Return v
    return v;
}

Move minimax(Board* board, int playersTurn) {
    /*
    function MINIMAX-DECISION(state) returns an action
     inputs: state, current state in game
     v←MAX-VALUE(state)
     return the action in SUCCESSORS(state) with value v
    */

    // Find valid moves
    ValidMoves validMoves = findValidMoves(board, playersTurn);

    // Initialize best move index
    int bestMoveIndex = -1;

    // Initialize max and min values
    // Max (this agent) wants to maximize the total number of valid moves
    // Min (opponent) wants to minimize the total number of valid moves
    int max = -1000; // Max is initialized to negative infinity
    int min = 1000; // Min is initialized to positive infinity

    // Loop through valid moves
    for (int i=0; i<validMoves.size; i++) {
        // Get the next state
        Board* nextState = minMaxResult(board, validMoves.moves[i]);

        // Get the min value
        int minValueResult = minValue(nextState, playersTurn, findValidMoves(nextState, playersTurn));

        // If min value is less than min, update min
        if (minValueResult < min) {
            min = minValueResult;
        }

        // If min value is greater than max, update max and best move index
        if (minValueResult > max) {
            max = minValueResult;
            bestMoveIndex = i;
        }
    }

    // Return best move
    return validMoves.moves[bestMoveIndex];
}

int main() {
    // Game Variables
    int running = 1; 
    Point firstMoveBlack, firstMoveWhite;  // Coordinates of the first two moves
    int playersTurn = 0;  // 0 for black, 1 for white => black always moves first

    // Initialize the game board
    Board board = {{
        {'B', 'W', 'B', 'W', 'B', 'W', 'B', 'W'},
        {'W', 'B', 'W', 'B', 'W', 'B', 'W', 'B'},
        {'B', 'W', 'B', 'W', 'B', 'W', 'B', 'W'},
        {'W', 'B', 'W', 'B', 'W', 'B', 'W', 'B'},
        {'B', 'W', 'B', 'W', 'B', 'W', 'B', 'W'},
        {'W', 'B', 'W', 'B', 'W', 'B', 'W', 'B'},
        {'B', 'W', 'B', 'W', 'B', 'W', 'B', 'W'},
        {'W', 'B', 'W', 'B', 'W', 'B', 'W', 'B'}
    }};

    // Print out the initial board
    printf("Initial board:\n");
    printBoard(board);

    // Get the first move for black
    while (1) {
        printf("BLACK: Enter the 'X' and 'Y' coordinates of the piece you want to remove: ");
        scanf("%c %d", &firstMoveBlack.x, &firstMoveBlack.y);
        if (isValidFirstMove(&board, playersTurn, firstMoveBlack) == 1) {
            break;
        }
    }

    // Make the first move for black and print the boardV
    makeFirstMove(&board, firstMoveBlack);
    printBoard(board);

    // Get the first move for white
    while (1) {
        printf("WHITE: Enter the 'X' and 'Y' coordinates of the piece you want to remove: ");
        scanf("%c %d", &firstMoveWhite.x, &firstMoveWhite.y);
        if (isValidFirstMove(&board, playersTurn, firstMoveWhite) == 1) {
            break;
        }
    }

    // Make the first move for white and print the board
    makeFirstMove(&board, firstMoveWhite);
    printBoard(board);

    // Main Game Loop
    while (running == 1) {
        // Initialize move
        Move move;

        // Get the next move
        if (playersTurn == 0) {
            printf("BLACK's move\n");
            move = minimax(&board, playersTurn);
        } else {
            printf("WHITE's move\n");
            printf("Enter the 'X' and 'Y' coordinates of the piece you want to move: ");
            scanf("%c %d", &move.start.x, &move.start.y);
            printf("Enter the 'X' and 'Y' coordinates of the new position: ");
            scanf("%c %d", &move.end.x, &move.end.y);
        }

        // Check if move is valid
        if (isValidMove(&board, playersTurn, move) == 0) {
            printf("Invalid move\n");
        } else {
            makeMove(&board, move);
            printBoard(board);
            playersTurn = (playersTurn + 1) % 2;
        }

        // Check if the game is over
        if (findValidMoves(&board, playersTurn).size == 0) {
            running = 0;
        }
    }
}
