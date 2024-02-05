/*
 ============================================================================
 Name        : FlightFighter
 Author      : C.Feng
 Description : A simple shooting game developed in C
 ============================================================================
 */

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// Position struct representing x, y coordinates
typedef struct {
    int x, y;
} Position;

// Player struct with position and score
typedef struct {
    Position position;
    int score;
    int lives;
} Player;

// Enemy struct with a position
typedef struct {
    Position position;
} Enemy;

// Bullet struct with a position
typedef struct {
    Position position;
} Bullet;

// Function to move the cursor to a specific position on the console
void gotoxy(int x, int y)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { x, y };
    SetConsoleCursorPosition(handle, position);
}

// Function to hide the blinking cursor
void hideCursor()
{
    CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

// Function to set up the game state (Initialization)
void initialize(Player* player, Enemy* enemy, Bullet* bullet, int* high, int* width)
{
    *high = 20;
    *width = 40;
    player->position.x = 10;
    player->position.y = 15;
    player->score = 0;
    player->lives = 3;
    bullet->position.x = -1;
    bullet->position.y = -1;
    enemy->position.x = rand() % *high;
    enemy->position.y = rand() % *width;

    hideCursor();
}

// Function to draw the game entities on the screen
void draw(Player player, Enemy enemy, Bullet bullet, int high, int width)
{
    gotoxy(0, 0);

    // Draw the top border of the game area
    for(int j = 0; j < width + 2; j++) {
        printf("#");
    }
    printf("\n");

    for(int i = 0; i < high; i++) {
        // Draw the left border of the game area
        printf("#");

        for(int j = 0; j < width; j++) {
            // Check and draw player, enemy, or bullet if their position matches
            if(i == player.position.x && j == player.position.y) {
                printf("*"); // The aircraft
            } else if(i == enemy.position.x && j == enemy.position.y) {
                printf("V"); // The enemy
            } else if(i == bullet.position.x && j == bullet.position.y) {
                printf("|"); // The bullet
            } else {
                // Print space if no player, enemy, or bullet is in the current position
                printf(" ");
            }
        }

        // Draw the right border of the game area
        printf("#\n");
    }

    // Draw the bottom border of the game area
    for(int j = 0; j < width + 2; j++) {
        printf("#");
    }
    printf("\n");

    // Display the player's current score
    printf("Score: %d Lives: %d\n", player.score, player.lives);
}

// Function to update the game state without user input
void updateWithoutInput(Player* player, Enemy* enemy, Bullet* bullet, int high, int width)
{
    static int speed = 0;

    if(bullet->position.x > -1) {
        bullet->position.x--;
    }

    if(bullet->position.x == enemy->position.x && bullet->position.y == enemy->position.y) {
        player->score++;        // One point for player
        enemy->position.x = -1; // New enemy
        enemy->position.y = rand() % width;
        bullet->position.x = -1;
    }

    if(enemy->position.x > high) {
        enemy->position.x = -1; // New enemy
        enemy->position.y = rand() % width;
    }

    if(enemy->position.x >= high) {
        player->lives--; // Player loses a life
        enemy->position.x = -1;
        enemy->position.y = rand() % width;
    }

    if(enemy->position.x == player->position.x && enemy->position.y == player->position.y) {
        player->lives--; // Player loses a life
        enemy->position.x = -1;
        enemy->position.y = rand() % width;
    }

    if(player->lives <= 0) {
        printf("Game Over!\n");
        exit(0);
    }

    if(speed < 10) {
        speed++;
    }

    if(speed == 10) {
        enemy->position.x++;
        speed = 0;
    }
}

// Function to update the game state with user input
void updateWithInput(Player* player, Bullet* bullet, int high, int width)
{
    char input;

    if(kbhit()) {
        input = getch();
        switch(input) {
        case 'a':
            if(player->position.y > 0) {
                player->position.y--;
            }
            break;
        case 'd':
            if(player->position.y < width - 1) {
                player->position.y++;
            }
            break;
        case 'w':
            if(player->position.x > 0) {
                player->position.x--;
            }
            break;
        case 's':
            if(player->position.x < high - 1) {
                player->position.x++;
            }
            break;
        case ' ': // Shooting
            bullet->position.x = player->position.x - 1;
            bullet->position.y = player->position.y;
            break;

        default:
            break;
        }
    }
}

int main()
{
    Player player;
    Enemy enemy;
    Bullet bullet;
    int high, width;

    system("cls");
    initialize(&player, &enemy, &bullet, &high, &width);

    while(1) {
        draw(player, enemy, bullet, high, width);
        updateWithoutInput(&player, &enemy, &bullet, high, width);
        updateWithInput(&player, &bullet, high, width);
    }

    return 0;
}
