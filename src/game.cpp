#include "game.h"
#include <iostream>
#include "SDL.h"
#include <queue>
#include <future>

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)) {
  queue = std::make_shared<MessageQueue<SDL_Point>>();
  //PlaceFood();
  reset(life);
  reset(double_food);
  reset(food);
  start_thread = std::thread(&Game::PlaceFood, this);
}

Game::~Game(){
    start_thread.join();
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

template <typename T>
T MessageQueue<T>::receive()
{
        std::unique_lock<std::mutex> uLock(_mutex);
        _cond.wait(uLock, [this] { return !_messages.empty(); }); // pass unique lock to condition variable

        // remove last vector element from queue
         T v = std::move(_messages.back());
        _messages.pop_back();

        return v; // will not be copied due to return value optimization (RVO) in C++
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
        // perform vector modification under the lock
        std::lock_guard<std::mutex> uLock(_mutex);

        // add vector to queue
        _messages.push_back(std::move(msg));
        _cond.notify_one(); // notify client after pushing new Vehicle into vector
}

template <typename T>
bool MessageQueue<T>::isEmpty(){
    return _messages.empty();
}

SDL_Point Game::PlaceFood() {
  int x, y;
  while (true) {
      while (queue->isEmpty()) {
          do{
        x = random_w(engine);
        y = random_h(engine);
          }while(x>31 || y>31 || snake.SnakeCell(x, y));
        // Check that the location is not occupied by a snake item before placing
        // food.
        if (!snake.SnakeCell(x, y)) {
          food.x = x;
          food.y = y;
        }
        std::async(std::launch::async, &MessageQueue<SDL_Point>::send, queue, std::move(food));
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
      double_food = queue->receive();
      reset(food);
      reset(life);
      // Grow snake and increase speed.
      snake.GrowBody();
      snake.speed += 0.02;
    }else if(score%7==0){
      life = queue->receive();
      reset(food);
      reset(double_food);
    }else{
      food = queue->receive();
      reset(double_food);
      reset(life);
      // Grow snake and increase speed.
      snake.GrowBody();
      snake.speed += 0.02;
    }
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

void Game::reset(SDL_Point &point){
    point.x = -100;
    point.y = -100;
}
