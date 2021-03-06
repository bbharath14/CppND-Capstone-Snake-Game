#include "renderer.h"
#include <iostream>
#include <string>

//constructor
Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}



//destroy window and quit in destructor
Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}
//copy constructor
Renderer::Renderer(const Renderer &source) noexcept: screen_width(source.screen_width),
screen_height(source.screen_height),
grid_width(source.grid_width),
grid_height(source.grid_height){
    sdl_window = source.sdl_window;
    sdl_renderer = source.sdl_renderer;
    std::cout << "Renderer Copy Constructor Called\n" ;
}

//copy assignment operator
Renderer& Renderer::operator=(const Renderer &source) noexcept{
    std::cout << "Renderer Copy Assignement Operator Called\n" ;
    if(this == &source){
        return *this;
    }
    sdl_window = source.sdl_window;
    sdl_renderer = source.sdl_renderer;
    return *this;
}

//move constructor
Renderer::Renderer(Renderer &&source): screen_width(source.screen_width),
screen_height(source.screen_height),
grid_width(source.grid_width),
grid_height(source.grid_height){
    sdl_window = source.sdl_window;
    sdl_renderer = source.sdl_renderer;
    std::cout<< "Renderer Move Constructor Called\n";
}

//move assignment operatior
Renderer& Renderer::operator=(Renderer &&source){
    std::cout<< "Renderer Move Assignemnt Operator Called\n";
    if(this == &source){
        return *this;
    }
     sdl_window = source.sdl_window;
    sdl_renderer = source.sdl_renderer;
    return *this;
}

//fill the screen with food and the snake
void Renderer::Render(Snake const snake, SDL_Point const &food, SDL_Point const &life, SDL_Point const &double_food) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
    if(food.x!=-1 && food.y!=-1){
      SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
      block.x = food.x * block.w;
      block.y = food.y * block.h;
      SDL_RenderFillRect(sdl_renderer, &block);
    }

  // Render life
    if(life.x!=-1 && life.y!=-1){
        SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0xFF, 0xFF);
        block.x = life.x * block.w;
        block.y = life.y * block.h;
        SDL_RenderFillRect(sdl_renderer, &block);
    }
    
  // Render double life
    if(double_food.x!=-1 && double_food.y!=-1){
        SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0xFF, 0x00, 0xFF);
        block.x = double_food.x * block.w;
        block.y = double_food.y * block.h;
        SDL_RenderFillRect(sdl_renderer, &block);
    }

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

//update the title of the window based on the parameters
void Renderer::UpdateWindowTitle(int score, int fps, int lives) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps) +
    " Lives: " + std::to_string(lives)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
