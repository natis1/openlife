#pragma once
#include <math.h>

#include "physics.hpp"

struct position
{
    double x; 
    double y;
};

struct color
{
    //Graphics related data. Sidenote: LAB color would be cool but I can't be bothered
    double r;
    double g;
    double b;
    double a;
};

namespace objects
{
class Entity
{

public:
    Entity(double size, double life);
//    virtual void update();
    bool alive();
    void damage(double n=1.);
    void regen(double n=1.);
    double lifePercent();
    
    void move(double dx, double dy);
    
    
    
    double getRadius();
    void setRadius(double radius);
    
    position getPosition();
    void setPosition(double x, double y);
    
    double getRotation();
    void setRotation(double theta);

    void rotate(double theta);
    
    color getFillColor();
    int getApproximateFillColor();
    void setFillColor(double r, double g, double b, double a);
    void setFillColor(color c);
    
private:
    double life;
    double total_life;
    
    //All of this stuff handeled by the entity instead
    position entityLocation;
    double size;
    double rotation;
    double radius = 10;
    color entityColor {255.,255.,255.,255.};
    
};
}
