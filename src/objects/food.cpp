#include "food.hpp"

Food::Food() :
    Entity(4.0, 1)
{

}

void Food::eat()
{
    damage(); // Reduce life by one
}
