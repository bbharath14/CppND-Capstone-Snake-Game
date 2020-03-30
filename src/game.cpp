#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)) {
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food, life, double_food);

    frame_end = SDL_GetTicks();
    
    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count, snake.lives);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
      do{
    x = random_w(engine);
    y = random_h(engine);
      }while(x>31 || y>31 || snake.SnakeCell(x, y));
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      life.x = -1;
      life.y = -1;
      double_food.x = -1;
      double_food.y = -1;
      return;
    }
  }
}

void Game::PlaceLife(){
  int x, y;
  while (true) {
      do{
    x = random_w(engine);
    y = random_h(engine);
      }while(x>31 || y>31 || snake.SnakeCell(x, y));
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      life.x = x;
      life.y = y;
      food.x = -1;
      food.y = -1;
      double_food.x = -1;
      double_food.y = -1;
      return;
    }
  }
}

void Game::PlaceDoubleFood(){
  int x, y;
  while (true) {
      do{
    x = random_w(engine);
    y = random_h(engine);
      }while(x>31 || y>31 || snake.SnakeCell(x, y));
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      double_food.x = x;
      double_food.y = y;
      life.x = -1;
      life.y = -1;
      food.x = -1;
      food.y = -1;
      return;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;
  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);
  bool needs_update = false;
  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    needs_update = true;
  }else if(double_food.x == new_x && double_food.y == new_y){
    score += 2;
    needs_update = true;
  }else if(life.x == new_x && life.y == new_y){
      score++;
    snake.lives += 1;
    needs_update = true;
  }
  if(needs_update){
    if(score%5==0){
      PlaceDoubleFood();
      // Grow snake and increase speed.
      snake.GrowBody();
      snake.speed += 0.02;
    }else if(score%7==0){
      PlaceLife();
    }else{
      PlaceFood();
      // Grow snake and increase speed.
      snake.GrowBody();
      snake.speed += 0.02;
    }
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }
