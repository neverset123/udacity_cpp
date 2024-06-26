#include "snake.h"
#include <cmath>
#include <iostream>
#include <algorithm>

std::unordered_map<Snake::Direction, Snake::Direction> Snake::opposite_direction = {
                                                        {Snake::Direction::kUp, Snake::Direction::kDown},
                                                        {Snake::Direction::kDown, Snake::Direction::kUp},
                                                        {Snake::Direction::kLeft, Snake::Direction::kRight},
                                                        {Snake::Direction::kRight, Snake::Direction::kLeft},
                                                    };
void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::Reverse() {
  if(body.size()==0)
  {
    switch (direction) {
      case Snake::Direction::kUp:
        direction = Snake::Direction::kDown;
        break;

      case Snake::Direction::kDown:
        direction = Snake::Direction::kUp;
        break;

      case Snake::Direction::kLeft:
        direction = Snake::Direction::kRight;
        break;

      case Snake::Direction::kRight:
        direction = Snake::Direction::kLeft;
        break;
    }
  }
  else
  {
    SDL_Point current_cell{
        static_cast<int>(head_x),
        static_cast<int>(head_y)};
    body.push_back(current_cell);
    head_x = body.begin()->x;
    head_y = body.begin()->y;
    body.erase(body.begin());
    std::reverse(body.begin(), body.end());
    if(body.end()->x==head_x && body.end()->y<head_y)
    {
      direction = Snake::Direction::kDown;
    }
    else if(body.end()->x==head_x && body.end()->y>head_y)
    {
      direction = Snake::Direction::kUp;
    }
      else if(body.end()->x>head_x && body.end()->y==head_y)
    {
      direction = Snake::Direction::kLeft;
    }
    else if(body.end()->x<head_x && body.end()->y==head_y)
    {
      direction = Snake::Direction::kRight;
    }
  }
}

void Snake::UpdateHead() {
  switch (direction) {
    case Snake::Direction::kUp:
      head_y -= speed;
      break;

    case Snake::Direction::kDown:
      head_y += speed;
      break;

    case Snake::Direction::kLeft:
      head_x -= speed;
      break;

    case Snake::Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      num_of_life--;
      if(num_of_life==0)
      {
        alive = false;
      }
      else
      {
        Reset();
      }
    }
  }
}

void Snake::Reset()
{
  direction = Snake::Direction::kUp;
  speed = 0.1f;
  size = 1;
  head_x = grid_width / 2;
  head_y = grid_height / 2;
  body.clear();
  growing = false;
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(Food& food) {
  if (food.GetX() == static_cast<int>(head_x) && food.GetY() == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (food.GetX() == item.x && food.GetY() == item.y) {
      return true;
    }
  }
  return false;
}