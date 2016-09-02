#pragma once
#include "entity.hpp"
#include "../tools/tools.hpp"

class Food;

using FoodVec = std::vector<std::shared_ptr<Food>>;

class Food : public Entity
{
public:
    Food();

    void eat();
};
