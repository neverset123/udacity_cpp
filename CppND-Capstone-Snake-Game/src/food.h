#ifndef FOOD_H
#define FOOD_H

#include <random>

class Food {
public:
    Food(int grid_width, int grid_height)
        : engine(dev()), random_w(0, static_cast<int>(grid_width - 1)), random_h(0, static_cast<int>(grid_height - 1)) {}
    // Function to set a new random position for the food.
    void Place();
    // Function to check if the food is at a certain position.
    bool IsAt(int x, int y) const;
    // Getters for the food's position.
    int GetX() const;
    int GetY() const;

private:
    std::random_device dev;
    std::mt19937 engine;
    std::uniform_int_distribution<std::mt19937::result_type> random_w;
    std::uniform_int_distribution<std::mt19937::result_type> random_h;

    int x;
    int y;
};

#endif