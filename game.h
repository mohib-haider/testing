/*
 * game.h - Shared header for the Snake Game (C, Windows Console)
 *
 * Team: Mohib, Aarish, Hasan
 *
 * RULES:
 *  1. Every .c file must start with:  #include "game.h"
 *  2. Do NOT change anything in this file without telling the whole team.
 *  3. Do NOT rename functions or variables after Day 1.
 *  4. The global variables are DEFINED once in main.c (Mohib)
 *     and only DECLARED here with "extern".
 */

#ifndef GAME_H
#define GAME_H

/* ---------------------------------------------------------------
 * LIBRARIES
 * --------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>      /* _kbhit(), _getch() */
#include <windows.h>    /* Sleep(), console functions */
#include <time.h>       /* time() for srand() */

/* ---------------------------------------------------------------
 * CONSTANTS
 * --------------------------------------------------------------- */

/* Game area size (the border is drawn on the outer edge).
 * Playable cells: x = 1 .. WIDTH-2,  y = 1 .. HEIGHT-2 */
#define WIDTH           40
#define HEIGHT          20

/* Maximum snake length = number of playable cells (safe upper limit) */
#define MAX_LENGTH      (WIDTH * HEIGHT)

/* Snake starting length */
#define START_LENGTH    3

/* Points gained for each apple */
#define POINTS_PER_APPLE 10

/* Directions */
#define UP      1
#define DOWN    2
#define LEFT    3
#define RIGHT   4

/* Arrow key codes (_getch() returns 224 first, then one of these) */
#define KEY_ARROW_PREFIX 224
#define KEY_UP           72
#define KEY_DOWN         80
#define KEY_LEFT         75
#define KEY_RIGHT        77

/* Other keys */
#define KEY_PAUSE_LOWER  'p'
#define KEY_PAUSE_UPPER  'P'
#define KEY_ESC          27
#define KEY_ENTER        13

/* Difficulty levels */
#define EASY    1
#define MEDIUM  2
#define HARD    3

/* Game speed = delay in milliseconds passed to Sleep().
 * Smaller delay = faster snake. */
#define SPEED_EASY      150
#define SPEED_MEDIUM    100
#define SPEED_HARD      60

/* File where the high score is stored */
#define HIGHSCORE_FILE  "highscore.txt"

/* Characters used for drawing (change if you like) */
#define CHAR_WALL       '#'
#define CHAR_SNAKE_HEAD 'O'
#define CHAR_SNAKE_BODY 'o'
#define CHAR_FOOD       '@'
#define CHAR_EMPTY      ' '

/* ---------------------------------------------------------------
 * GLOBAL VARIABLES  (defined in main.c, shared by everyone)
 * --------------------------------------------------------------- */

/* Snake body: index 0 is the HEAD, index (length-1) is the TAIL */
extern int snakeX[MAX_LENGTH];
extern int snakeY[MAX_LENGTH];
extern int length;          /* current snake length                     */
extern int direction;       /* UP, DOWN, LEFT or RIGHT                  */

/* Food position */
extern int foodX;
extern int foodY;

/* Score */
extern int score;           /* current score                            */
extern int highScore;       /* best score (loaded from highscore.txt)   */

/* Game settings and state */
extern int difficulty;      /* EASY, MEDIUM or HARD                     */
extern int speed;           /* delay in ms used by Sleep()              */
extern int gameOver;        /* 1 = game finished, 0 = still playing     */
extern int paused;          /* 1 = game paused,   0 = running           */

/* The last tail position removed by moveSnake().
 * Aarish uses this to erase the old tail from the screen. */
extern int oldTailX;
extern int oldTailY;

/* ---------------------------------------------------------------
 * FUNCTIONS OWNED BY MOHIB   (main.c, snake.c)
 * --------------------------------------------------------------- */

void initSnake(void);           /* set start position, length, direction     */
void moveSnake(void);           /* move one step; save old tail position     */
void growSnake(void);           /* increase length by 1 after eating         */
int  checkWallCollision(void);  /* returns 1 if head hit a wall, else 0      */
int  checkSelfCollision(void);  /* returns 1 if head hit own body, else 0    */
void resetGame(void);           /* reset everything for a new game           */
void gameLoop(void);            /* main loop: input, move, check, draw, wait */
/* int main(void) is also in main.c */

/* ---------------------------------------------------------------
 * FUNCTIONS OWNED BY AARISH  (ui.c)
 * --------------------------------------------------------------- */

void hideCursor(void);              /* hide the blinking console cursor      */
void gotoxy(int x, int y);          /* move cursor to column x, row y        */
void drawBorder(void);              /* draw the walls around the game area   */
void drawSnake(void);               /* draw head and body; erase oldTailX/Y  */
void drawFood(void);                /* draw the apple at foodX, foodY        */
void drawScore(void);               /* show score and high score             */
int  showMainMenu(void);            /* returns: 1=Start 2=Difficulty
                                       3=High Score 4=Exit                   */
int  showGameOverScreen(void);      /* returns 1 = play again, 0 = exit      */
void getInput(void);                /* read arrow keys and update 'direction'
                                       (must block reversing direction).
                                       If P is pressed, call pauseGame().    */

/* ---------------------------------------------------------------
 * FUNCTIONS OWNED BY HASAN  (food_score.c)
 * --------------------------------------------------------------- */

void generateFood(void);        /* random foodX, foodY, never on the snake   */
int  checkFoodEaten(void);      /* returns 1 if head is on the food, else 0  */
void updateScore(void);         /* score += POINTS_PER_APPLE                 */
void resetScore(void);          /* score = 0                                 */
void loadHighScore(void);       /* read highScore from HIGHSCORE_FILE        */
void saveHighScore(void);       /* write highScore to HIGHSCORE_FILE         */
void updateHighScore(void);     /* if score > highScore, update and save     */
void selectDifficulty(void);    /* ask player; set 'difficulty' and 'speed'  */
void pauseGame(void);           /* freeze until P is pressed again           */

#endif /* GAME_H */
