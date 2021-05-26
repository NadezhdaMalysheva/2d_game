#ifndef GAME_LVL
#define GAME_LVL

#include "../Objects/Object.h"
#include "../Image.h"
#include "fstream"
#include <vector>
#include <iostream>
#include "../elementary/Elementary.h"


struct Level
{
    Level(const std::string &map_path, int channals, short level);//, Point &point);

    bool CanMove(Point coord);
    bool Move(Point coord, Point size);
    bool Void (Point coord);
    bool NotEnd ();
    Point GetStartCoord() { return location * batch_size + batch_size/4; }
    Point BlocFromCoord(Point coord) {return {coord.x/batch_size, coord.y/batch_size};}
    bool UpdateObj(Image &screen, int x0, int y0, int x1, int y1, int i);

    void Destroy();

    Image background;
    std::vector<std::string> Map;
    bool end = false;
    bool win = false;
    bool die = false;
    short levels = 0;
    std::vector <Object> moveObj; 


private:

    Point location = {0, 0};
    int batch_size;
    int max_x;
    int max_y;
    Image Texture[3];
    Point End;

};



#endif //GAME_LVL