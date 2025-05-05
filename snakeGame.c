/*
    This is a submission from Chris DeLeon and Alex Reyes
*/

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // For usleep
#include "functions.h"

int main() {
    srand(time(NULL));   // Initialization of random number

    int max_x, max_y, direction = 0;
    direction = rand() % 4; // Randomizing the initial direction of the snake
    Coordinate snake[100]; // Arbitrary max size
    Coordinate trophy;
    trophy_timer = rand() % 9;
    start_color();

    // Initialize game environment
    init_game();

    // Load start screen
    show_start_screen();  // Show rules before gameplay

    // Get terminal dimensions
    getmaxyx(stdscr, max_y, max_x);

    // Initialize snake in the center
    initialize_snake(direction, snake, max_x, max_y);

    // Initialize trophy in a random location
    initialize_trophy(trophy, max_x, max_y);
    
    //Checking to make sure the initial trophy does not spawn on snake head
    while (trophy.x == snake[0].x && trophy.y == snake[0].y) {
        trophy.x = (rand() % (max_x - 2)) + 1;
        trophy.y = (rand() % (max_y - 2)) + 1;
    }

    // Game loop
    while (1) {
        clear();
        draw_border();

        // Capture user input
        nodelay(stdscr, TRUE);
        int ch = getch();
        if (ch == 'q' || ch == 'Q') {
            endwin();
            printf("==========================================\n");
            printf("Aww, you quit?! Hope to see you next time!\n");
            printf("==========================================\n");
            return 0;
        }
        // Changing direction and if trying to reverse, ends game
        int new_direction = update_direction(direction, ch);
        if (new_direction == -1) {
            endwin();
            printf("=====================================\n");
            printf("Looks like you tried to go backwards!\n");
            printf("=====================================\n");
            return 0;
        }
        direction = new_direction;

        // Move snake
        move_snake(snake, snake_length, direction);

        // Check for collisions to border or self
        if (check_loss(snake, max_x, max_y)) return 0;

        // Check for getting trophy
        if (trophy_collision(snake[0], trophy, max_x, max_y)){
            draw_trophy(&trophy, max_x, max_y, 1, 0);
        }

        // Checking for win condition
        if (handle_win(snake_length, max_x, max_y)) return 0;

        // Draw the snake
        draw_snake(snake, snake_length, direction);

        // Draw the trophy
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
        attron(COLOR_PAIR(2));
        mvprintw(trophy.y, trophy.x, "%c", shape);
        attroff(COLOR_PAIR(2));

        refresh();
        if (direction == UP || direction == DOWN) {
            usleep(DELAY*1.5-(snake_length*1000));
        }else {
            usleep(DELAY-(snake_length*1000));
        }
        time_elapsed = time_elapsed + DELAY;
        if(((double)time_elapsed/1000000) >= trophy_timer){
            draw_trophy(&trophy, max_x, max_y, 0, 1);
        }
    }

    // End ncurses mode
    endwin();
    return 0;
}
