/*
 * ui.c - Screen drawing, menus and keyboard input
 *
 * Owner: Aarish
 *
 * Functions in this file:
 *   hideCursor(), gotoxy(), drawBorder(), drawSnake(), drawFood(),
 *   drawScore(), showMainMenu(), showGameOverScreen(), getInput()
 *
 * NOTE: Global variables (snakeX, score, direction, etc.) are defined in
 * main.c. Here we only use them. Do NOT define them again.
 *
 * Screen layout (rows):
 *   0 .. HEIGHT-1   the game area with the border
 *   HEIGHT          score line            (drawScore)
 *   HEIGHT+1        controls help line    (drawScore)
 *   HEIGHT+2        pause message         (pauseGame in food_score.c)
 */

#include "game.h"

/* Console colour codes used with SetConsoleTextAttribute() */
#define COLOR_GREEN   10
#define COLOR_CYAN    11
#define COLOR_RED     12
#define COLOR_YELLOW  14
#define COLOR_WHITE   7

/* ---------------------------------------------------------------
 * Helper functions (only used inside this file)
 * --------------------------------------------------------------- */

/* Changes the text colour of everything printed after this call */
static void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)color);
}

/* Returns the name of the current difficulty as text */
static const char *getDifficultyName(void)
{
    if (difficulty == EASY)
    {
        return "Easy";
    }
    if (difficulty == HARD)
    {
        return "Hard";
    }
    return "Medium";
}

/* Prints a character at a screen position */
static void putCharAt(int x, int y, char ch)
{
    gotoxy(x, y);
    putchar(ch);
}

/* ---------------------------------------------------------------
 * hideCursor()
 * Hides the blinking console cursor. Call once at program start.
 * --------------------------------------------------------------- */
void hideCursor(void)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;

    info.dwSize = 1;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(console, &info);
}

/* ---------------------------------------------------------------
 * gotoxy()
 * Moves the cursor to column x and row y.
 * Everything that draws on the screen uses this.
 * --------------------------------------------------------------- */
void gotoxy(int x, int y)
{
    COORD position;

    position.X = (SHORT)x;
    position.Y = (SHORT)y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

/* ---------------------------------------------------------------
 * drawBorder()
 * Draws the walls around the game area.
 * --------------------------------------------------------------- */
void drawBorder(void)
{
    int x, y;

    setColor(COLOR_CYAN);

    /* top and bottom walls */
    for (x = 0; x < WIDTH; x++)
    {
        putCharAt(x, 0, CHAR_WALL);
        putCharAt(x, HEIGHT - 1, CHAR_WALL);
    }

    /* left and right walls */
    for (y = 0; y < HEIGHT; y++)
    {
        putCharAt(0, y, CHAR_WALL);
        putCharAt(WIDTH - 1, y, CHAR_WALL);
    }

    setColor(COLOR_WHITE);
}

/* ---------------------------------------------------------------
 * drawSnake()
 * Erases the old tail cell, then draws the head and the body.
 * oldTailX / oldTailY are set by moveSnake() in snake.c.
 * --------------------------------------------------------------- */
void drawSnake(void)
{
    int i;

    /* 1. Erase the old tail (only if it is inside the play area,
     *    so we never erase a wall by mistake) */
    if (oldTailX >= 1 && oldTailX <= WIDTH - 2 &&
        oldTailY >= 1 && oldTailY <= HEIGHT - 2)
    {
        putCharAt(oldTailX, oldTailY, CHAR_EMPTY);
    }

    /* 2. Draw the body */
    setColor(COLOR_GREEN);
    for (i = 1; i < length; i++)
    {
        putCharAt(snakeX[i], snakeY[i], CHAR_SNAKE_BODY);
    }

    /* 3. Draw the head last so it is always on top */
    setColor(COLOR_YELLOW);
    putCharAt(snakeX[0], snakeY[0], CHAR_SNAKE_HEAD);

    setColor(COLOR_WHITE);
}

/* ---------------------------------------------------------------
 * drawFood()
 * Draws the apple at foodX, foodY.
 * --------------------------------------------------------------- */
void drawFood(void)
{
    setColor(COLOR_RED);
    putCharAt(foodX, foodY, CHAR_FOOD);
    setColor(COLOR_WHITE);
}

/* ---------------------------------------------------------------
 * drawScore()
 * Shows the score, high score and difficulty under the game area,
 * plus a short controls reminder.
 * --------------------------------------------------------------- */
void drawScore(void)
{
    setColor(COLOR_WHITE);

    /* %-5d / %-6s pad the text so old digits are always overwritten */
    gotoxy(0, HEIGHT);
    printf("Score: %-5d High Score: %-5d Level: %-6s",
           score, highScore, getDifficultyName());

    gotoxy(0, HEIGHT + 1);
    printf("Arrows: Move | P: Pause | ESC: Quit");
}

/* ---------------------------------------------------------------
 * showMainMenu()
 * Shows the main menu and returns the player's choice:
 *   1 = Start Game
 *   2 = Difficulty
 *   3 = High Score  (handled inside this function, see below)
 *   4 = Exit
 * Option 3 shows the high score screen and then returns to the menu,
 * so this function only returns 1, 2 or 4 in practice.
 * --------------------------------------------------------------- */
int showMainMenu(void)
{
    char choice;

    while (1)
    {
        system("cls");

        setColor(COLOR_GREEN);
        printf("\n\n");
        printf("        ==============================\n");
        printf("                 SNAKE  GAME\n");
        printf("        ==============================\n\n");

        setColor(COLOR_WHITE);
        printf("          1. Start Game\n");
        printf("          2. Difficulty  (current: %s)\n", getDifficultyName());
        printf("          3. High Score\n");
        printf("          4. Exit\n\n");
        printf("        Enter your choice (1-4): ");

        choice = (char)_getch();

        if (choice == '1')
        {
            system("cls");
            return 1;
        }
        else if (choice == '2')
        {
            system("cls");
            return 2;
        }
        else if (choice == '3')
        {
            /* show the high score screen, then go back to the menu */
            system("cls");
            setColor(COLOR_YELLOW);
            printf("\n\n");
            printf("        ===== HIGH SCORE =====\n\n");
            setColor(COLOR_WHITE);
            printf("          Best score: %d\n\n", highScore);
            printf("        Press any key to go back...");
            _getch();
        }
        else if (choice == '4')
        {
            system("cls");
            return 4;
        }
        /* any other key: the loop simply shows the menu again */
    }
}

/* ---------------------------------------------------------------
 * showGameOverScreen()
 * Shows "GAME OVER" with the final score.
 * Returns 1 if the player wants to play again, 0 to exit.
 * Call updateHighScore() BEFORE this so the high score is correct.
 * --------------------------------------------------------------- */
int showGameOverScreen(void)
{
    char choice;

    system("cls");

    setColor(COLOR_RED);
    printf("\n\n");
    printf("        ==============================\n");
    printf("                  GAME OVER\n");
    printf("        ==============================\n\n");

    setColor(COLOR_WHITE);
    printf("          Your score : %d\n", score);
    printf("          High score : %d\n\n", highScore);

    if (score > 0 && score == highScore)
    {
        setColor(COLOR_YELLOW);
        printf("          *** NEW HIGH SCORE! ***\n\n");
        setColor(COLOR_WHITE);
    }

    printf("        Play again? (Y = Yes, N = No): ");

    while (1)
    {
        choice = (char)_getch();

        if (choice == 'y' || choice == 'Y')
        {
            system("cls");
            return 1;
        }
        if (choice == 'n' || choice == 'N')
        {
            system("cls");
            return 0;
        }
        /* any other key is ignored, keep waiting */
    }
}

/* ---------------------------------------------------------------
 * getInput()
 * Reads the keyboard WITHOUT stopping the game (uses _kbhit()).
 *  - Arrow keys change 'direction'
 *  - The snake cannot turn back directly into itself
 *  - P calls pauseGame()
 *  - ESC ends the game
 * --------------------------------------------------------------- */
void getInput(void)
{
    int key;
    int lastDirection = direction;  /* direction at the start of this frame */

    /* Read every key waiting in the buffer. Checking against
     * lastDirection (not 'direction') stops two quick key presses
     * from making the snake reverse into itself in a single move. */
    while (_kbhit())
    {
        key = _getch();

        if (key == KEY_ARROW_PREFIX || key == 0)
        {
            /* arrow keys send two codes: the prefix, then the real key */
            key = _getch();

            switch (key)
            {
                case KEY_UP:
                    if (lastDirection != DOWN)
                    {
                        direction = UP;
                    }
                    break;

                case KEY_DOWN:
                    if (lastDirection != UP)
                    {
                        direction = DOWN;
                    }
                    break;

                case KEY_LEFT:
                    if (lastDirection != RIGHT)
                    {
                        direction = LEFT;
                    }
                    break;

                case KEY_RIGHT:
                    if (lastDirection != LEFT)
                    {
                        direction = RIGHT;
                    }
                    break;

                default:
                    break;
            }
        }
        else if (key == KEY_PAUSE_LOWER || key == KEY_PAUSE_UPPER)
        {
            pauseGame();
        }
        else if (key == KEY_ESC)
        {
            gameOver = 1;
        }
    }
}
