#pragma once

struct position
{
    double x; 
    double y;
};

struct color
{
    //Graphics related data. Sidenote: LAB color would be cool but I can't be bothered
    int r;
    int g;
    int b;
    int a;
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
    
    position getPosition();
    void setPosition(double x, double y);
    
    double getRotation();
    void setRotation(double theta);
    
    color getFillColor();
    void setFillColor(int r, int g, int b, int a);
    
private:
    double life;
    double total_life;
    
    //All of this stuff handeled by the entity instead
    position entityLocation;
    double size;
    double rotation;
    double radius = 10;
    color color {255,255,255,255};
    
};
}

