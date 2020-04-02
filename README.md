## Description
This is the classic snake game where the player manoeuvres a line which grows in length,
with the line itself being a primary obstacle. The player controls the snake on a bordered
plane. As the player attempts to eat items by running into them with the head of the snake,
the length and speed of the snake increases. The direction in which the snake moves can be
controlled by the arrow keys on the keyboard. The snake when the head touches any part of
the body of the snake or when it touches the border of the window.
There are two special foods which occur randomly. The food present in green colour
indicates a life which helps to save the snake automatically in case the snake dies. The food
present in blue colour indicates double food which increases the score as well as the snake
length by 2 units. Note that normal food will also be visible along with the special food, only
one of them can be eaten. The other will disappear as soon as one is eaten.
The score and the lives will be visible on the title of the window.

## Dependencies for Running Locally
* cmake >= 3.7
* All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
* Linux: make is installed by default on most Linux distros
* Mac: [install Xcode command line tools to get
make](https://developer.apple.com/xcode/features/)
* Windows: [Click here for installation
instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
* All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
* Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
* gcc/g++ >= 5.4
* Linux: gcc / g++ is installed by default on most Linux distros
* Mac: same deal as make - [install Xcode command line
tools](https://developer.apple.com/xcode/features/)
* Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions
1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.
