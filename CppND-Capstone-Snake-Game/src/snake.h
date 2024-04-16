#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"
#include "food.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height, int num_of_life)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2),
        num_of_life(num_of_life) {}

  void Update();
  void Reverse();
  void GrowBody();
  bool SnakeCell(Food& food);

  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{1};
  int num_of_life;
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);
  void Reset();

  bool growing{false};
  int grid_width;
  int grid_height;
};

#endif