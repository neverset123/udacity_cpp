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
  Game(std::size_t grid_width, std::size_t grid_height);
  ~Game();
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  std::unique_ptr<Snake> snake;
  Food* food;
  int score{0};
  bool food_eaten;
  std::mutex mtx_;
  bool running = true;

  void PlaceFood();
  void Update();
  void Log();
};

#endif