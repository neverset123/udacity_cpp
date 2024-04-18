#include "food.h"
#include <iostream>

void Food::Place(FoodType type) 
{
    x = random_w(engine);
    y = random_h(engine);
    food_type = type;
}

bool Food::IsAt(int x, int y) const 
{
    return this->x == x && this->y == y;
}

int Food::GetX() const { return x; }
int Food::GetY() const { return y; }
FoodType Food::GetType() const { return food_type; }
