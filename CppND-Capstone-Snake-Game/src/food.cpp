#include "food.h"
#include <iostream>

void Food::Place() 
{
    x = random_w(engine);
    y = random_h(engine);
}

bool Food::IsAt(int x, int y) const 
{
    return this->x == x && this->y == y;
}

int Food::GetX() const { return x; }
int Food::GetY() const { return y; }
