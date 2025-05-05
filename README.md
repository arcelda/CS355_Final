# CS355_Final

In order to run the game, please use:

gcc snakeGame.c functions.c -o snakeGame.out -lncurses

To run the game:

./snakeGame.out

// Notes
Be advised that the snake begins in the center of the terminal. The snake
also starts off pointing and moving in a random direction (Up, down, left, right)

// Losing Conditions
1. Collide with the wall
2. Collide with any part of the snake itself
3. Trying to go in the reverse direction of where the snake is currently moving.
For example, if the snake is going left, clicking right will automatically end
the game. The only legitimate moves you can make if your snake is going left,
are to continue going left, going up, or going down.