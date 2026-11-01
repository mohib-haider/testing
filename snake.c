/*
 * snake.c - Snake logic and the main game loop
 *
 * Owner: Mohib
 *
 * Functions in this file:
 *   initSnake(), moveSnake(), growSnake(), checkWallCollision(),
 *   checkSelfCollision(), resetGame(), gameLoop()
 *
 * NOTE: Global variables (snakeX, snakeY, length, score, etc.) are
 * defined in main.c. Here we only use them. Do NOT define them again.
 *
 * Snake storage: index 0 is the HEAD, index (length - 1) is the TAIL.
 */

#include "game.h"

/* ---------------------------------------------------------------
 * initSnake()
 * Puts the snake in the middle of the screen, moving right.
 * The head is at index 0 and the body extends to the left.
 * --------------------------------------------------------------- */
void initSnake(void)
{
    int i;

    length = START_LENGTH;
    direction = RIGHT;

    for (i = 0; i < length; i++)
    {
        snakeX[i] = (WIDTH / 2) - i;    /* body goes to the left of the head */
        snakeY[i] = HEIGHT / 2;
    }

    /* Starting value for the tail position, used by drawSnake() */
    oldTailX = snakeX[length - 1];
    oldTailY = snakeY[length - 1];
}

/* ---------------------------------------------------------------
 * moveSnake()
 * Moves the snake one step in the current direction.
 * Every segment takes the place of the segment in front of it,
 * and the head moves to a new cell.
 * The old tail position is saved so drawSnake() can erase it.
 * --------------------------------------------------------------- */
void moveSnake(void)
{
    int i;

    /* save the tail position BEFORE it is overwritten */
    oldTailX = snakeX[length - 1];
    oldTailY = snakeY[length - 1];

    /* shift the body: start from the tail and go towards the head */
    for (i = length - 1; i > 0; i--)
    {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }

    /* move the head */
    switch (direction)
    {
        case UP:
            snakeY[0]--;
            break;

        case DOWN:
            snakeY[0]++;
            break;

        case LEFT:
            snakeX[0]--;
            break;

        case RIGHT:
            snakeX[0]++;
            break;

        default:
            break;
    }
}

/* ---------------------------------------------------------------
 * growSnake()
 * Makes the snake one segment longer after eating an apple.
 * The new segment is placed where the tail was before the last move
 * (saved in oldTailX / oldTailY by moveSnake()).
 * Call this AFTER moveSnake().
 * --------------------------------------------------------------- */
void growSnake(void)
{
    if (length < MAX_LENGTH)
    {
        snakeX[length] = oldTailX;
        snakeY[length] = oldTailY;
        length++;
    }
}

/* ---------------------------------------------------------------
 * checkWallCollision()
 * Returns 1 if the head is on a wall, otherwise 0.
 * The walls are at x = 0, x = WIDTH-1, y = 0 and y = HEIGHT-1.
 * --------------------------------------------------------------- */
int checkWallCollision(void)
{
    if (snakeX[0] <= 0 || snakeX[0] >= WIDTH - 1 ||
        snakeY[0] <= 0 || snakeY[0] >= HEIGHT - 1)
    {
        return 1;
    }
    return 0;
}

/* ---------------------------------------------------------------
 * checkSelfCollision()
 * Returns 1 if the head is on any part of the snake's own body,
 * otherwise 0. The head (index 0) is not compared with itself.
 * --------------------------------------------------------------- */
int checkSelfCollision(void)
{
    int i;

    for (i = 1; i < length; i++)
    {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
        {
            return 1;
        }
    }
    return 0;
}

/* ---------------------------------------------------------------
 * resetGame()
 * Gets everything ready for a new game: new snake, score 0,
 * a fresh apple. The difficulty (speed) is NOT changed.
 * --------------------------------------------------------------- */
void resetGame(void)
{
    initSnake();            /* must come first: generateFood() needs the snake */
    resetScore();
    gameOver = 0;
    paused = 0;
    generateFood();
}

/* ---------------------------------------------------------------
 * gameLoop()
 * Plays ONE complete game, from the first frame until game over.
 * Each frame: read input, move, check collisions, check food,
 * draw, wait.
 * --------------------------------------------------------------- */
void gameLoop(void)
{
    resetGame();

    /* draw the starting screen */
    system("cls");
    drawBorder();
    drawFood();
    drawSnake();
    drawScore();

    while (!gameOver)
    {
        /* 1. read the keyboard (may change direction, pause, or quit) */
        getInput();
        if (gameOver)
        {
            break;          /* the player pressed ESC */
        }

        /* 2. move the snake */
        moveSnake();

        /* 3. check for collisions */
        if (checkWallCollision() || checkSelfCollision())
        {
            gameOver = 1;
            break;
        }

        /* 4. check if the snake ate the apple */
        if (checkFoodEaten())
        {
            growSnake();
            updateScore();
            generateFood();     /* may end the game if the board is full */
            if (!gameOver)
            {
                drawFood();
            }
        }

        /* 5. draw the new frame */
        drawSnake();
        drawScore();

        /* 6. wait; a smaller 'speed' value makes the snake faster */
        Sleep(speed);
    }

    /* the game is over: save the high score if it was beaten */
    updateHighScore();
}
