#pragma once
#include <cstdlib>
#include <tuple>
#include <iostream>

// Conversions
template <typename T>
T degrees(T t)
{
    return t * 57.2958;
}

template <typename T>
T radians(T t)
{
    return t * 0.0174533;
}

template <typename T>
double distance(T& t1, T& t2)
{
    auto a = t1.getPosition();
    auto b = t2.getPosition();
    double dx = abs(a.x - b.x);
    double dy = abs(a.y - b.y);
    return hypot(dx, dy);
}

template <typename T>
double distance(sf::Vector2f a, T& t)
{
    auto b = t.getPosition();
    double dx = abs(a.x - b.x);
    double dy = abs(a.y - b.y);
    return hypot(dx, dy);
}

// Compute angle between two objects in degrees
template <typename T>
double angle(T& t1, T& t2)
{
    auto a = t1.getPosition();
    auto b = t2.getPosition();
    double angle = atan2(b.y, b.x) - atan2(a.x, a.y);
    return degrees(angle);
}

// Move on a vector with magnitude amount along current rotation
template <typename T>
void moveVec(T& t, double amount)
{
    double angle = radians(t.getRotation());
    double dx    = amount * cos(angle);
    double dy    = amount * sin(angle);
    t.move(dx, dy);
}


