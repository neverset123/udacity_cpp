#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(new Snake(grid_width, grid_height)), food(new Food(grid_width, grid_height))
    {
      food->Place();
    }

Game::~Game()
{
  delete food;
  
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;

  std::thread t = std::thread(&Game::Log, this);
  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, *snake.get());
    Update();
    renderer.Render(*snake.get(), *food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
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
  t.join();
}

void Game::PlaceFood() {
  while (true) {
    // Check that the location is not occupied by a snake item before placing
    // food.
    food->Place();
    if (!snake->SnakeCell(*food)) {
      return;
    }
  }
}

void Game::Update() {
  if (!snake->alive) return;

  snake->Update();

  int new_x = static_cast<int>(snake->head_x);
  int new_y = static_cast<int>(snake->head_y);

  // Check if there's food over here
  if (food->GetX() == new_x && food->GetY() == new_y) {
    std::lock_guard<std::mutex> guard(mtx_);
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake->GrowBody();
    snake->speed += 0.02;
  }
}

void Game::Log() 
{
  int score_record = 10;
  while(running)
  {
    mtx_.lock();
    int score = GetScore();
    mtx_.unlock();
    if(score==score_record)
    {
      std::cout << "congratulations, you have gotten " << score << " scores!\n";
      score_record += score_record;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake->size; }