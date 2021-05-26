#include "Player.h"

bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

void Player::ProcessInput(MovementDir dir)
{
  int move_dist = move_speed * 1;
  Point MoveCoord = getPointMD(dir);
  if (GameLevel->Move(coords + MoveCoord * move_dist, Hero.GetSize()+(-5)))
  {
    if (MoveCoord.x) old_coords.x = coords.x;
    else if (MoveCoord.y) old_coords.y = coords.y;
    coords = coords + MoveCoord * move_dist;
  }

}

void Player::Draw(Image &screen)
{
  if (dectroy)
  {
    dectroy = false;
    screen = GameLevel -> background;

    Hero.SetCoord(coords, old_coords.x - coords.x);
    Hero.DrawImage(screen);
    Hero.UpIndex();
    old_coords = coords;
  }
  else if(Moved())
  {

    for(int y = old_coords.y; y < old_coords.y + Hero.height; ++y)
    {
      for(int x = old_coords.x; x < old_coords.x + Hero.width; ++x)
      {
        screen.PutPixel(x, y, GameLevel -> background.GetPixel(x, y));
      }
    }
  
    Hero.SetCoord(coords, old_coords.x - coords.x);
    Hero.DrawImage(screen);
    Hero.UpIndex();
    old_coords = coords;
  }
}