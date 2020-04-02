#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <queue>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

template<class T> class MessageQueue
{
public:
    T receive();
    void send(T &&v);
    bool isEmpty();
private:
    std::mutex _mutex;
    std::condition_variable _cond;
    std::deque<T> _messages; // list of all vehicles waiting to enter this intersection
};

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  ~Game();
  Game(const Game &chatbot);
  Game& operator=(const Game &chatbot);
  Game(Game &&chatbot);
  Game& operator=(Game &&chatbot);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake;
  SDL_Point food;
  SDL_Point life;
  SDL_Point double_food;
  SDL_Point temp_food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  
  std::shared_ptr<MessageQueue<SDL_Point>> queue;
  int score{0};

  void PlaceFood();
  void Update();
  void reset(SDL_Point &point);
  bool MatchesFoodOrLife(int x, int y);
  std::thread start_thread;
  std::atomic<bool> on;
};

#endif
