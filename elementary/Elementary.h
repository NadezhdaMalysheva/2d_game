#ifndef EL_H
#define EL_H

#include <iostream>
#include <cmath>

struct Point
{
  int x;
  int y;

  Point operator+(Point pt)
    {
        Point p = {0,0};
        p.x = x + pt.x;
        p.y = y + pt.y;
        return p;
    }

  Point operator+(int pt)
    {
        Point p = {0,0};
        p.x = x + pt;
        p.y = y + pt;
        return p;
    }

  Point operator*(int i)
    {
      return {x * i, y * i};
    }

  Point operator*(Point p)
    {
      return {x * p.x, y * p.y};
    }

  bool operator==(Point pt)
    {
        return (pt.x == x) && (pt.y == y);
    }

  float radius(Point p)
  {
    return sqrt((p.x - x)*(p.x - x) + (p.y - y)*(p.y - y));
  }

  //Point operator=(const Point &p)
};

enum class MovementDir
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

namespace
{
  Point getPointMD(MovementDir dir)
  {
    switch(dir)
    {
      case MovementDir::UP:
        return {0, 1};
      case MovementDir::DOWN:
        return {0, -1};
      case MovementDir::LEFT:
        return {-1, 0};
      case MovementDir::RIGHT:
        return {1, 0};
      default:
        std::cout <<"aaaaa"<<std::endl;
        return {0, 0};
    }
  }
}

struct Pixel
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  Pixel operator*(uint8_t i)
  {
    return {r * i, g * i, b * i, a};
  }

  Pixel operator+(Pixel p)
  {
    float transparency = float(p.a)/255;
    Pixel ret = { r*(1-transparency) + p.r*transparency, 
                  g*(1-transparency) + p.g*transparency,
                  b*(1-transparency) + p.b*transparency,
                  a};
    return ret;
  }

  Pixel & operator=(const Pixel & p)
  {
    r = p.r;
    g = p.g;
    b = p.b;
    a = p.a;
    return *this;
  }

};



#endif //EL_H
