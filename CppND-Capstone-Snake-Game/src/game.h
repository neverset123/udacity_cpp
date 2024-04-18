#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <deque>
#include <thread>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "food.h"

class Point {
public:
    int x, y;
    double g, h, f;
    Point* parent;
    Snake::Direction direction;

    Point(int x, int y) : x(x), y(y), g(0), h(0), f(0), parent(nullptr), direction(Snake::Direction::kUp) {}

    double distance(Point* other) {
        return abs(other->x - x) + abs(other->y - y);
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, std::size_t screen_width, std::size_t screen_height, std::size_t num_of_life, std::string mode);
  ~Game();
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore(size_t i) const;
  int GetSize(size_t i) const;
  std::vector<std::unique_ptr<Point>> aStarSearch();

 private:
  std::vector<std::unique_ptr<Snake>> snakeVec;
  Food* food;
  std::vector<int> scores = {0,0};
  bool food_eaten;
  std::mutex mtx_;
  bool running = true;
  std::string mode;
  std::size_t grid_width;
  std::size_t grid_height;
  std::size_t screen_width;
  std::size_t screen_height;
  std::vector<std::unique_ptr<Point>> path;

  void PlaceFood(FoodType type);
  void Update();
};

#endif