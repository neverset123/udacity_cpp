#include "food.h"
#include <iostream>

void Food::Place() 
{
    x = random_w(engine);
    y = random_h(engine);
    bool random_bool = std::rand()%2;
    if(random_bool)
    {
        food_type = FoodType::NORMAL;
    }
    else{
        food_type = FoodType::POISONOUS;
    }
}

bool Food::IsAt(int x, int y) const 
{
    return this->x == x && this->y == y;
}

int Food::GetX() const { return x; }
int Food::GetY() const { return y; }
FoodType Food::GetType() const { return food_type; }
