#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "functions.h"

// Global variables
char shape;
int trophy_value = 0;
int captured = 0;
int time_elapsed = 0;
int trophy_timer;
int expired_time = 0;
int snake_length = 3;

// Initialize ncurses
void init_game() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(FALSE);
}

// Draw the border
void draw_border() {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    box(stdscr, 0, 0);
}

// Move the snake
void move_snake(Coordinate *snake, int length, int direction) {
    for (int i = length - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    switch (direction) {
        case UP:    snake[0].y--; break;
        case DOWN:  snake[0].y++; break;
        case LEFT:  snake[0].x--; break;
        case RIGHT: snake[0].x++; break;
    }
}

// Check for collisions with border
int check_collision(Coordinate head, int max_x, int max_y) {
    return (head.x <= 0 || head.x >= max_x - 1 || head.y <= 0 || head.y >= max_y - 1);
}

// Generate or update the trophy
void draw_trophy(Coordinate *trophy, int max_x, int max_y, int captured, int expired_time) {
    if (captured == 1 || expired_time == 1) {
        if (captured == 1) {
            snake_length += trophy_value;
        }
        trophy->x = (rand() % (max_x - 2)) + 1;
        trophy->y = (rand() % (max_y - 2)) + 1;
        trophy_value = rand() % 9 + 1;
        shape = trophy_shape(trophy_value);
    }
    time_elapsed = 0;
    trophy_timer = rand() % 9 + 1;
}

// Return trophy character based on value
char trophy_shape(int value) {
    switch (value) {
        case 1: return '1';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';
        default: return '*';
    }
}

// Check for trophy collision
int trophy_collision(Coordinate head, Coordinate trophy, int max_x, int max_y) {
    return (head.x == trophy.x && head.y == trophy.y);
}

// Check if the snake hit itself
int self_collision(Coordinate *snake) {
    for (int i = 1; i < snake_length; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            return 1;
        }
    }
    return 0;
}

// Win condition
int check_for_win(int snake_length, int max_x, int max_y) {
    return (snake_length >= (max_x + max_y));
}

// Draw the snake
void draw_snake(Coordinate *snake, int length, int direction) {
    start_color();

    init_pair(1, COLOR_GREEN, COLOR_BLACK);

    for (int i = 0; i < length; i++) {
        if (i == 0) {
            attron(COLOR_PAIR(1));
            if (direction == UP)
                mvprintw(snake[0].y, snake[0].x, "V");
            else if (direction == DOWN)
                mvprintw(snake[0].y, snake[0].x, "^");
            else if (direction == LEFT)
                mvprintw(snake[0].y, snake[0].x, ">");
            else if (direction == RIGHT)
                mvprintw(snake[0].y, snake[0].x, "<");
            attroff(COLOR_PAIR(1));
        } else {
            attron(COLOR_PAIR(1));
            mvprintw(snake[i].y, snake[i].x, "O");
            attroff(COLOR_PAIR(1));
        }
    }
}

// Update direction
int update_direction(int current_direction, int ch) {
    if ((ch == KEY_UP && current_direction == DOWN) ||
        (ch == KEY_DOWN && current_direction == UP) ||
        (ch == KEY_LEFT && current_direction == RIGHT) ||
        (ch == KEY_RIGHT && current_direction == LEFT)) {
        // Invalid reverse move
        return -1;
    }

    if (ch == KEY_UP && current_direction != DOWN)
        return UP;
    if (ch == KEY_DOWN && current_direction != UP)
        return DOWN;
    if (ch == KEY_LEFT && current_direction != RIGHT)
        return LEFT;
    if (ch == KEY_RIGHT && current_direction != LEFT)
        return RIGHT;

    return current_direction; // No valid input change
}

// Loss conditions
int check_loss(Coordinate *snake, int max_x, int max_y) {
    if (check_collision(snake[0], max_x, max_y)) {
        endwin();
        printf("===============================\n");
        printf("Whoops! You went out of Bounds!\n");
        printf("===============================\n");
        return 1;
    }

    if (self_collision(snake)) {
        endwin();
        printf("=================================================\n");
        printf("Uh oh! Looks like you took a bit out of yourself!\n");
        printf("=================================================\n");
        return 1;
    }

    return 0;
}

// Win Condition
int handle_win(int snake_length, int max_x, int max_y) {
    if (check_for_win(snake_length, max_x, max_y)) {
        endwin();
        printf("=========================\n");
        printf("Congratulations, you won!\n");
        printf("=========================\n");
        return 1;
    }
    return 0;
}

// Initializing snake
void initialize_snake(int direction, Coordinate *snake, int max_x, int max_y) {
switch (direction) {
        case 0: // Starting the snake up
            for (int i = 0; i < snake_length; i++) {
                snake[i].x = max_x / 2;
                snake[i].y = max_y / 2 + i;
            }
            break;
        case 1: // Starting the snake down
            for (int i = 0; i < snake_length; i++) {
                snake[i].x = max_x / 2;
                snake[i].y = max_y / 2 - i;
            }
            break;
        case 2: // Starting the snake left
            for (int i = 0; i < snake_length; i++) {
                snake[i].x = max_x / 2 + i;
                snake[i].y = max_y / 2;
            }
            break;
        case 3: // Starting the snake right
            for (int i = 0; i < snake_length; i++) {
                snake[i].x = max_x / 2 - i;
                snake[i].y = max_y / 2;
            }
            break;   
    }
}

// Initialize trophy
void initialize_trophy(Coordinate trophy, int max_x, int max_y){
    trophy.x = (rand() % (max_x - 2)) + 1;
    trophy.y = (rand() % (max_y - 2)) + 1;
    trophy_value = rand() % 9 + 1;    //Setting the random generator to get the value of the trophy
    shape = trophy_shape(trophy_value);
}

// Welcome Screen
void show_start_screen() {
    clear();
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    char *title = "WELCOME TO TERMINAL SNAKE!";
    char *instructions[] = {
        "Use the arrow keys to move the snake.",
        "Collect trophies (numbers) to grow. Win by growing to half the perimeter of the map!",
        "Avoid the walls and yourself.",
        "Trophies disappear if not collected in time.",
        "Don't try to reverse direction!",
        "",
        "Press any key to start and once the game starts press 'Q' to quit"
    };

    mvprintw(max_y / 2 - 4, (max_x - strlen(title)) / 2, "%s", title);
    for (int i = 0; i < sizeof(instructions)/sizeof(instructions[0]); i++) {
        mvprintw(max_y / 2 - 2 + i, (max_x - strlen(instructions[i])) / 2, "%s", instructions[i]);
    }

    refresh();
    nodelay(stdscr, FALSE); // Wait for key press
    getch();
    nodelay(stdscr, TRUE);  // Resume non-blocking input
}

