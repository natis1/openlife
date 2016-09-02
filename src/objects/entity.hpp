#include <SFML/Graphics.hpp>

class Entity : public sf::CircleShape
{

public:
    Entity(double size=10, int life=50);
    virtual void update();
    bool alive();
    void damage(int n=1);
    void regen(int n=1);
    double lifePercent();

private:
    int life;
    int total_life;
};

