#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include "./elementary/Elementary.h"
#include "Map/GameLevel.h"
#include "./Objects/Object.h"

struct Player
{
  explicit Player(Point pos = {.x = 10, .y = 10}, std::vector <std::string> frames = {""}) :
                 coords(pos), old_coords(coords+1), Hero(frames, pos) {};  /////////////////////////////////////////////////////////////////

  bool Moved() const;
  void ProcessInput(MovementDir dir);
  void Draw(Image &screen);

  void PutLevel(Level &level) 
  {
    GameLevel = &level; 
    coords = level.GetStartCoord();
  }

  void Destroy() {GameLevel -> Destroy(); dectroy = true;}
  Object Hero;
  Point GetCoord() {Point p = {Hero.width/2, Hero.height/2}; return (coords+p);}
  Level *GameLevel;


private:
  Point coords {.x = 10, .y = 10};
  Point old_coords {.x = 0, .y = 0};
  Pixel color {.r = 255, .g = 255, .b = 0, .a = 255};
  int move_speed = 4;
  bool dectroy = false;
};

#endif //MAIN_PLAYER_H
