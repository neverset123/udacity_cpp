#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <deque>
#include <thread>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "food.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, std::size_t num_of_life, std::string mode);
  ~Game();
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore(size_t i) const;
  int GetSize(size_t i) const;

 private:
  std::vector<std::unique_ptr<Snake>> snakeVec;
  Food* food;
  std::vector<int> scores = {0,0};
  bool food_eaten;
  std::mutex mtx_;
  bool running = true;
  std::string mode;

  void PlaceFood();
  void Update();
  void Log();
};

#endif