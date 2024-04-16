#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height, std::size_t num_of_life, std::string mode)
    : food(new Food(grid_width, grid_height)), mode(mode)
    {
      snakeVec.push_back(std::move(std::make_unique<Snake>(grid_width, grid_height, num_of_life))); 
      snakeVec.push_back(std::move(std::make_unique<Snake>(grid_width, grid_height, num_of_life)));
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
    if(mode=="s")
    {
      controller.HandleInput(running, *snakeVec.at(0).get());
    }
    else if (mode=="d")
    {
      controller.HandleInput(running, *snakeVec.at(0).get(), *snakeVec.at(1).get());
    }
    
    Update();
    std::vector<Snake*> snakes_raw;
    if(mode == "s")
    {
      snakes_raw = {snakeVec.at(0).get()};
    }
    else if(mode == "d")
    {
      snakes_raw = {snakeVec.at(0).get(), snakeVec.at(1).get()};
    }
    renderer.Render(snakes_raw, *food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(scores, frame_count, mode);
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
    if (!snakeVec.at(0)->SnakeCell(*food) && !snakeVec.at(1)->SnakeCell(*food)) {
      return;
    }
  }
}

void Game::Update() {
  size_t num_snake{1};
  if (!snakeVec.at(0)->alive || !snakeVec.at(1)->alive) return;
  snakeVec.at(0)->Update();
  if(mode == "d")
  {
    snakeVec.at(1)->Update();
    num_snake = 2;
  }

  for(size_t i{0}; i<num_snake; i++)
  {
    int new_x = static_cast<int>(snakeVec.at(i)->head_x);
    int new_y = static_cast<int>(snakeVec.at(i)->head_y);
    // Check if there's food over here
    if (food->GetX() == new_x && food->GetY() == new_y) {
      std::lock_guard<std::mutex> guard(mtx_);
      if(food->GetType() == FoodType::NORMAL)
      {
        scores.at(i)++;
        PlaceFood();
        // Grow snake and increase speed.
        snakeVec.at(i)->GrowBody();
        snakeVec.at(i)->speed += 0.02;
      }
      else
      {
        snakeVec.at(i)->Reverse();
        PlaceFood();
      }
    }
  }
}

void Game::Log() 
{
  int score_record = 10;
  while(running)
  {
    mtx_.lock();
    int score = GetScore(0);
    mtx_.unlock();
    if(score==score_record)
    {
      std::cout << "congratulations, you have gotten " << score << " scores!\n";
      score_record += score_record;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

int Game::GetScore(size_t i) const { return scores.at(i); }
int Game::GetSize(size_t i) const { return snakeVec.at(i)->size; }