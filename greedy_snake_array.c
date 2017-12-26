//
// Copyright (C), 2017-2018, CodingTeam Organization
// FileName: greedySnake.c
// Author: Jiahao Zheng
// Github: https://github.com/Jiahonzheng
// Mail: jiahonzheng@gmail.com
// Date: 2017-12-21
// Description: This is a program to simulate 'Greedy Snake'.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define SNAKE_MAX_LENGTH 20
#define SNAKE_HEAD 'H'
#define SNAKE_BODY 'X'
#define BLANK_CELL ' '
#define WALL_CELL '*'
#define FOOD '$'
#define MOVE_UP 'W'
#define MOVE_DOWN 'S'
#define MOVE_LEFT 'A'
#define MOVE_RIGHT 'D'
#define PAUSE_GAME 'P'
#define RESUME_GAME 'C'
#define EXIT_GAME 'Q'
#define CHECK_SNAKE_HEAD_TOUCH_WALL (snakeX[snakeLength - 1] < 11 && \
                                     snakeX[snakeLength - 1] > 0 &&  \
                                     snakeY[snakeLength - 1] < 11 && \
                                     snakeY[snakeLength - 1] > 0)
#define CHECK_SNAKE_EAT_FOOD (snakeX[snakeLength - 1] == foodX && \
                              snakeY[snakeLength - 1] == foodY)

// bulid and initialize the map
char map[13][13] = {
    "************",
    "*XXXXH     *",
    "*          *",
    "*          *",
    "*          *",
    "*          *",
    "*          *",
    "*          *",
    "*          *",
    "*          *",
    "*          *",
    "************",
};

// initialize the x-coordinate of each cell of the snake
int snakeX[SNAKE_MAX_LENGTH] = { 1, 2, 3, 4, 5 };

// initialize the y-coordinate of each cell of the snake
int snakeY[SNAKE_MAX_LENGTH] = { 1, 1, 1, 1, 1 };

// initialize the length of the snake
int snakeLength = 5;

// initialize the x-coordinate of the food
int foodX = 0;
int foodY = 0;

// create the food on a randomized blank cell
void createFood(void);

// snakeMove(dx, dy)
// snake stepping:
// dx = -1(left), 1(right), 0(no move);
// dy = -1(up), 1(down), 0(no move)
void snakeMove(int, int);

// out cells of the grid
void output(void);

// check whether the snake eats the food:
// yes >>> return 1;
// no >>> return 0
int checkSnakeEatFood(void);

// check whether the snake head touches the body or wall:
// yes >>> return 1;
// no >>> return 0
int checkSnakeDie(void);

// outs when gameover
void gameover(void);

int main(void) {
    int  markInputWrong = 0;
    int  markPause      = 0;
    char input          = '0';

    // initialize the food
    createFood();

    // display the game
    output();

    srand((unsigned)time(NULL));

    while (1) {
        input = getchar();
        getchar();
        input = toupper(input);

        // check whether the game is paused
        if (markPause == 1) {
            if (input == 'Q') {
                printf("You have exited the game\n");
                exit(0);
            }

            if (input != 'C') {
                printf("The game is paused. Please press 'C' to resume.\n");
                continue;
            }
        }

        switch (input) {
            // move 'up'
            case MOVE_UP:
                snakeMove(0, -1);
                break;

            // move 'down'
            case MOVE_DOWN:
                snakeMove(0, 1);
                break;

            // move 'left'
            case MOVE_LEFT:
                snakeMove(-1, 0);
                break;

            // move 'right'
            case MOVE_RIGHT:
                snakeMove(1, 0);
                break;

            // Pause
            case PAUSE_GAME:
                printf("Waiting for resuming. Press 'C' to resume.\n");
                markPause = 1;
                break;

            // Resume
            case RESUME_GAME:

                if (markPause == 0) {
                    markInputWrong = 1;
                    break;
                } else {
                    printf("The game has been resumed.\n");
                    markPause = 0;
                    continue;
                }

            case EXIT_GAME:
                printf("You have exited the game\n");
                exit(0);

            // mark the wrong input cases
            default:
                markInputWrong = 1;
                break;
        }

        // check whether it is a wrong input
        if (markInputWrong == 1) {
            printf("Wrong input. The snake does not move.\n");
            markInputWrong = 0;
        } else {
            output();

            if (checkSnakeDie()) {
                gameover();
                break;
            }
        }
    }

    return 0;
}

// create the food on a randomized blank cell
void createFood(void) {
    int markReplicated = 0;

    // to make sure that the food is exactly created on a blank cell
    while (!markReplicated) {
        foodX = 1 + rand() % 10;
        foodY = 1 + rand() % 10;

        for (int i = 0; i < snakeLength - 1; i++) {
            if (snakeX[i] != foodX) {
                if (snakeY[i] != foodY) {
                    markReplicated = 1;
                }
            }
        }
    }

    map[foodY][foodX] = FOOD;
}

// snake stepping:
// dx = -1(left), 1(right), 0(no move);
// dy = -1(up), 1(down), 0(no move)
void snakeMove(int dx, int dy) {
    int newTailX = snakeX[0];
    int newTailY = snakeY[0];

    map[snakeY[0]][snakeX[0]]                             = BLANK_CELL;
    map[snakeY[snakeLength - 1]][snakeX[snakeLength - 1]] = SNAKE_BODY;

    for (int i = 0; i < snakeLength - 1; i++) {
        snakeX[i] = snakeX[i + 1];
        snakeY[i] = snakeY[i + 1];
    }

    snakeX[snakeLength - 1]                              += dx;
    snakeY[snakeLength - 1]                              += dy;
    map[snakeY[snakeLength - 1]][snakeX[snakeLength - 1]] = SNAKE_HEAD;

    // check whether the snake eats the food
    if (checkSnakeEatFood()) {
        for (int i = snakeLength - 1; i >= 0; i--) {
            snakeX[i] = snakeX[i - 1];
            snakeY[i] = snakeY[i - 1];
        }
        snakeX[0]               = newTailX;
        snakeY[0]               = newTailY;
        map[newTailY][newTailX] = SNAKE_BODY;
    }
}

// out cells of the grid
void output(void) {
    for (int i = 0; i < 12; i++) {
        printf("%s\n", map[i]);
    }
}

// check whether the snake eats the food:
// yes >>> return 1;
// no >>> return 0
int checkSnakeEatFood(void) {
    int markAte = 0;

    // if the snake has eaten the food, then its strength is increased
    if (CHECK_SNAKE_EAT_FOOD) {
        createFood();
        markAte = 1;
        snakeLength++;
    }

    return markAte;
}

// check whether the snake head touches the body or wall:
// yes >>> return 1;
// no >>> return 0
int checkSnakeDie(void) {
    int markDied = 0;

    // check whether the snake head if touches the body
    for (int i = 0; i < snakeLength - 2; i++) {
        if (snakeX[i] == snakeX[snakeLength - 1]) {
            if (snakeY[i] == snakeY[snakeLength - 1]) {
                markDied = 1;
            }
        }
    }

    // check whether the snake head if touches the wall
    return markDied || !CHECK_SNAKE_HEAD_TOUCH_WALL;
}

// outs when gameover
void gameover(void) {
    printf("GAME OVER!\n");
}
