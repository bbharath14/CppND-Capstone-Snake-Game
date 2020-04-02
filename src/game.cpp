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
  on = true;
  start_thread = std::thread(&Game::PlaceFood, this);
  food = queue->receive();
}

Game::~Game(){
    on = false;
    start_thread.join();
}

Game::Game(const Game &source): snake(source.snake){
    std::cout << "Game Copy Constructor Called\n" ;
}

Game& Game::operator=(const Game &source){
    std::cout << "Game Copy Assignement Operator Called\n" ;
    if(this == &source){
        return *this;
    }
    snake = source.snake;
    return *this;
}

Game::Game(Game &&source): snake(source.snake){
    std::cout<< "Game Move Constructor Called\n";
}

Game& Game::operator=(Game &&source){
    std::cout<< "Game Move Assignemnt Operator Called\n";
    if(this == &source){
        return *this;
    }
    snake = source.snake;
    return *this;
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
    return _messages.size()<5;
}

bool Game::MatchesFoodOrLife(int x, int y){
    if(food.x==x && food.y==y)
        return true;
    if(double_food.x ==x && double_food.y == y)
        return true;
    if(life.x == x && life.y == y)
        return true;
    return false;
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
      while (on && queue->isEmpty()) {
          do{
        x = random_w(engine);
        y = random_h(engine);
          }while(x>31 || y>31 || snake.SnakeCell(x, y));
        // Check that the location is not occupied by a snake item before placing
        // food.
        if (!snake.SnakeCell(x, y)) {
          temp_food.x = x;
          temp_food.y = y;
        }
        std::async(std::launch::async, &MessageQueue<SDL_Point>::send, queue, std::move(temp_food));
      }
      if(!on){
          break;
      }
  }
}

void Game::Update() {
  if (!snake.alive) return;
  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);
  bool needs_update = true;
  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
  }else if(double_food.x == new_x && double_food.y == new_y){
    score += 2;
  }else if(life.x == new_x && life.y == new_y){
      score++;
    snake.lives += 1;
  }else{
      needs_update = false;
  }
  if(needs_update){
    SDL_Point temp, temp1;
    do{
      temp = queue->receive();
    }while(MatchesFoodOrLife(temp.x, temp.y) || snake.SnakeCell(temp.x, temp.y));
    reset(life);
    reset(food);
    reset(double_food);
    food = temp;
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.01;
    if(score%11==0){
      do{
        temp1 = queue->receive();
      }while(MatchesFoodOrLife(temp1.x, temp1.y) || snake.SnakeCell(temp1.x, temp1.y));
      double_food = temp1;
    }else if(score%29==0){
        do{
          temp1 = queue->receive();
        }while(MatchesFoodOrLife(temp1.x, temp1.y) || snake.SnakeCell(temp1.x, temp1.y));
        life = temp1;
    }
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

void Game::reset(SDL_Point &point){
    point.x = -100;
    point.y = -100;
}
