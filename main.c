/*
 * main.c - Program entry point and global variables
 *
 * Owner: Mohib
 *
 * This file does two things:
 *   1. DEFINES the global variables that are shared by all files
 *      (game.h only declares them with "extern").
 *   2. Contains main(), which shows the menu and starts the game.
 *
 * Compile all files together:
 *   gcc main.c snake.c ui.c food_score.c -o snake
 */

#include "game.h"

/* ---------------------------------------------------------------
 * GLOBAL VARIABLES (the only place where they are defined)
 * --------------------------------------------------------------- */

/* Snake: index 0 is the head, index (length - 1) is the tail */
int snakeX[MAX_LENGTH];
int snakeY[MAX_LENGTH];
int length = START_LENGTH;
int direction = RIGHT;

/* Food position */
int foodX = 0;
int foodY = 0;

/* Score */
int score = 0;
int highScore = 0;

/* Game settings and state */
int difficulty = MEDIUM;
int speed = SPEED_MEDIUM;
int gameOver = 0;
int paused = 0;

/* Last tail position removed by moveSnake(), used by drawSnake() */
int oldTailX = 0;
int oldTailY = 0;

/* ---------------------------------------------------------------
 * main()
 * Starts the program and keeps showing the main menu until the
 * player chooses Exit.
 * --------------------------------------------------------------- */
int main(void)
{
    int choice;
    int playAgain;
    int running = 1;

    /* one-time setup */
    SetConsoleTitle("Snake Game");
    hideCursor();
    loadHighScore();        /* read the best score from highscore.txt */

    while (running)
    {
        choice = showMainMenu();

        switch (choice)
        {
            case 1:
                /* Start Game: keep playing while the player says yes */
                do
                {
                    gameLoop();                     /* plays one full game  */
                    playAgain = showGameOverScreen();
                } while (playAgain);
                break;

            case 2:
                /* Difficulty */
                selectDifficulty();
                break;

            case 4:
                /* Exit */
                running = 0;
                break;

            default:
                /* option 3 (High Score) is handled inside showMainMenu() */
                break;
        }
    }

    system("cls");
    printf("\n\n        Thanks for playing!\n\n");
    return 0;
}
