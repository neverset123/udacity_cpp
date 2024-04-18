#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height, std::size_t screen_width, std::size_t screen_height, std::size_t num_of_life, std::string mode)
    : food(new Food(grid_width, grid_height)), mode(mode), grid_width(grid_width), grid_height(grid_height), screen_width(screen_width), screen_height(screen_height)
    {
      snakeVec.push_back(std::move(std::make_unique<Snake>(grid_width, grid_height, num_of_life))); 
      snakeVec.push_back(std::move(std::make_unique<Snake>(grid_width, grid_height, num_of_life)));
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

  std::thread t = std::thread(&Game::PlaceFood, this, FoodType::NORMAL);
  std::thread t1 = std::thread(&Game::PlaceFood, this, FoodType::POISONOUS);
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
    else if (mode=="ai")
    {
      controller.HandleInput(running, *snakeVec.at(0).get());
      path = aStarSearch();
      path.pop_back();
      if(!path.empty())
      {
        if (snakeVec.at(1)->direction != Snake::opposite_direction[path.back()->direction] || snakeVec.at(1)->size == 1) snakeVec.at(1)->direction = path.back()->direction;
        path.pop_back();
      }
    }

    Update();
    std::vector<Snake*> snakes_raw;
    if(mode == "s")
    {
      snakes_raw = {snakeVec.at(0).get()};
    }
    else if(mode == "d" || mode == "ai")
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
  t1.join();
}

void Game::PlaceFood(FoodType type) {
  while(running)
  {
    mtx_.lock();
    if(food_eaten)
    {
        // Check that the location is not occupied by a snake item before placing
        // food.
        food->Place(type);
        if(mode == "s")
        {
          if (!snakeVec.at(0)->SnakeCell(*food)) {
            food_eaten = false;
          }
        }
        else if(mode == "d" || mode == "ai")
        {
          if (!snakeVec.at(0)->SnakeCell(*food) && !snakeVec.at(1)->SnakeCell(*food)) {
            food_eaten = false;
          }
        }
    }
    mtx_.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void Game::Update() {
  size_t num_snake{1};
  if (!snakeVec.at(0)->alive || !snakeVec.at(1)->alive) return;
  snakeVec.at(0)->Update();
  if(mode == "d" || mode == "ai")
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
        // PlaceFood();
        food_eaten = true;
        // Grow snake and increase speed.
        snakeVec.at(i)->GrowBody();
        snakeVec.at(i)->speed += 0.02;
      }
      else
      {
        snakeVec.at(i)->Reverse();
        // PlaceFood();
        food_eaten = true;
      }
    }
    while(food_eaten)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }
}

std::vector<std::unique_ptr<Point>> Game::aStarSearch() {
    std::vector<std::vector<int>> grid(grid_height, std::vector<int>(grid_width, 0));
    int head_x, head_y;
    for(auto& snake : snakeVec)
    {
      for (SDL_Point const &point : snake->body) {
        grid[point.y][point.x] = 1;
      }
      head_x = static_cast<int>(snake->head_x); // x, y coordinates are changed to standard coordiantes
      head_y = static_cast<int>(snake->head_y);
      grid[head_y][head_x] = 1;
    }
    Point* start = new Point(head_y, head_x);
    Point* end = new Point(food->GetY(), food->GetX());
    // std::cout<<"start: " << start->x << ", " << start->y << "\n";
    // std::cout<<"end: " << end->x << ", " << end->y << "\n";
    start->h = start->distance(end);
    start->f = start->h;

    std::vector<std::unique_ptr<Point>> path;
    std::vector<Point*> openSet;
    std::vector<Point*> closedSet;

    openSet.push_back(start);
    while (!openSet.empty()) {
        Point* current = *openSet.begin();
        openSet.erase(openSet.begin());

        if (*current == *end) {
            // Reconstruct path
            Point* p = current;
            while (p != nullptr) {
                path.push_back(std::move(std::make_unique<Point>(*p)));
                p = p->parent;
            }
            break;
        }
        closedSet.push_back(current);

        // Generate neighbors
        std::vector<Point*> neighbors;
        if (current->x > 0) {
            Point* point = new Point(current->x - 1, current->y);
            point->direction = Snake::Direction::kUp;
            neighbors.push_back(point);
        }
        if (current->x < grid.size() - 1 && grid[current->x + 1][current->y] == 0) {
            Point* point = new Point(current->x + 1, current->y);
            point->direction = Snake::Direction::kDown;
            neighbors.push_back(point);
        }
        if (current->y > 0 && grid[current->x][current->y - 1] == 0) {
            Point* point = new Point(current->x, current->y - 1);
            point->direction = Snake::Direction::kLeft;
            neighbors.push_back(point);
        }
        if (current->y < grid[0].size() - 1 && grid[current->x][current->y + 1] == 0) {
            Point* point = new Point(current->x, current->y + 1);
            point->direction = Snake::Direction::kRight;
            neighbors.push_back(point);
        }

        for (Point* neighbor : neighbors) {
            if (std::find_if(closedSet.begin(), closedSet.end(), [neighbor](Point* point){return *point==*neighbor;}) != closedSet.end()) {
                continue;
            }

            double gScore = current->g + current->distance(neighbor);
            if (gScore < neighbor->g || std::find_if(openSet.begin(), openSet.end(), [neighbor](Point* point){return *point==*neighbor;}) == openSet.end()) {
                neighbor->g = gScore;
                neighbor->h = neighbor->distance(end);
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent = current;

                if (std::find_if(openSet.begin(), openSet.end(), [neighbor](Point* point){return *point==*neighbor;}) == openSet.end())
                {
                    openSet.push_back(neighbor);
                }
            }
        }
    }

    // Clean up memory
    for (Point* p : closedSet) {
        delete p;
    }

    // delete start;
    // delete end;

    return path;
}

int Game::GetScore(size_t i) const { return scores.at(i); }
int Game::GetSize(size_t i) const { return snakeVec.at(i)->size; }