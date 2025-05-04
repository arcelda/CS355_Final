#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define DELAY 100000 // Snake speed

// Direction macros
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

// Snake structure
typedef struct {
    int x, y;
} Coordinate;

// External global variables (used in snakeGame.c)
extern int snake_length;
extern int trophy_timer;
extern int trophy_value;
extern char shape;
extern int time_elapsed;

// Function prototypes
void init_game();
void draw_border();
void move_snake(Coordinate *snake, int length, int direction);
int check_collision(Coordinate head, int max_x, int max_y);
void draw_trophy(Coordinate *trophy, int max_x, int max_y, int captured, int expired_time);
char trophy_shape(int value);
int trophy_collision(Coordinate head, Coordinate trophy, int max_x, int max_y);
int self_collision(Coordinate *snake);
int check_for_win(int snake_length, int max_x, int max_y);
void draw_snake(Coordinate *snake, int length, int direction);
int update_direction(int current_direction, int ch);
int check_loss(Coordinate *snake, int max_x, int max_y);
int handle_win(int snake_length, int max_x, int max_y);

#endif
