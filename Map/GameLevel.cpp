#include "GameLevel.h"

Level::Level(const std::string &map_path, int channels, short level)//, Point &point)
{
    end = false;
    die = false;
    levels = level;

    std::ifstream file;
    file.open(map_path);
    if (!file.is_open())
    {
        std::cout <<"File reading error"<< std::endl;
    }
    else
    {
        int w, h;
        file >> h >> w;
        std::vector<std::string> arr(h);
        max_x = h - 1;
        max_y = w - 1;

        for (auto &a : arr) 
        {
            file >> a;
        }

        Image wall = Image("./resources/background/1/1.png");
        Image wall_destructible = Image("./resources/background/1/2.png");
        Image floor = Image("./resources/background/1/floor.png");
        Image end = Image("./resources/background/1/next.png");
        Image lava = Image("./resources/background/1/lava.png");
        std::vector<std::string> frame =             {"./resources/background/fire/0fire.png",
                                                      "./resources/background/fire/1fire.png",
                                                      "./resources/background/fire/2fire.png",
                                                      "./resources/background/fire/3fire.png",
                                                      "./resources/background/fire/4fire.png",
                                                      "./resources/background/fire/5fire.png",
                                                      "./resources/background/fire/6fire.png",
                                                      "./resources/background/fire/7fire.png",
                                                      "./resources/background/fire/8fire.png",
                                                      "./resources/background/fire/9fire.png",
                                                      "./resources/background/fire/10fire.png",
                                                      "./resources/background/fire/11fire.png",
                                                      "./resources/background/fire/12fire.png",
                                                      "./resources/background/fire/13fire.png",
                                                      "./resources/background/fire/14fire.png"};

        Texture[0] = wall;
        Texture[1] = wall_destructible;
        Texture[2] = floor;

        batch_size  = wall.Height();


        Image bac = Image(h*batch_size,  w*batch_size, channels);

        
                
        for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j) 
        {
            switch (arr[i][j]) 
            {
                case '#':
                    bac.PutPixels( j * batch_size, i * batch_size, batch_size, batch_size, wall);
                    break;
                case '.':
                    bac.PutPixels( j * batch_size, i * batch_size, batch_size, batch_size, floor);
                    break;
                case '%':
                    bac.PutPixels( j * batch_size, i * batch_size, batch_size, batch_size, wall_destructible);
                    break;
                case '@':
                    location = {i, j};
                    arr[i][j] = '.';
                    bac.PutPixels( j * batch_size, i * batch_size, batch_size, batch_size, floor);
                    break;
                case '$':
                    End = {i, j};
                    bac.PutPixels( j * batch_size, i * batch_size, batch_size, batch_size, end);
                    break;
                case '_':
                    bac.PutPixels( j * batch_size, i * batch_size, batch_size, batch_size, lava);
                    break;
                case 'f':
                    arr[i][j] = '#';
                    moveObj.push_back(Object(frame, {i * batch_size, j * batch_size}, true));
                    bac.PutPixels( j * batch_size, i * batch_size, batch_size, batch_size, wall);
                    break;
                default:
                    break;
            }
        }

        background = bac;
        Map = arr;

        file.close();
    }

}

bool Level::Void (Point coord)
{
    return ((coord.x >= max_x) || (coord.y >= max_y) || (coord.x <= 0) || (coord.y <= 0));
}

bool Level::CanMove(Point coord)
{
    Point p = BlocFromCoord(coord);

    if (!Void(p)) return Map[p.x][p.y] == '.';
    else return false;
}

bool Level::Move(Point coord, Point size)
{
    Point p = BlocFromCoord(coord);
    Point p0 = BlocFromCoord(coord + size);
        bool boolEx = (Map[p.x][p.y] == '#' || Map[p.x][p.y] == '%') || (Map[p0.x][p.y] == '#' || Map[p0.x][p.y] == '%') ||
         (Map[p.x][p0.y] == '#' || Map[p.x][p0.y] == '%') || (Map[p0.x][p0.y] == '#' || Map[p0.x][p0.y] == '%');
        if (!boolEx)
        {
            location = p;
            
            return NotEnd ();
        }

    return false; 
}

void Level::Destroy()
{
    int diadFalse = (location.x + location.y)%2;

    for (int i = location.y-1; i <= location.y+1; ++i)
    for (int j = location.x-1; j <= location.x+1; ++j) 
    {

        if (!Void({j, i}) && ((i+j)%2 != diadFalse))
        {
            if (Map[j][i] == '%') 
            {
                background.PutPixels( i * batch_size, j * batch_size, batch_size, batch_size, Texture[2]);
                Map[j][i] = '.';
            }
        }

    }
}

bool Level::NotEnd () 
{   
    
    if (location == End)
    {
        end = true;
        win = true;
        return false;
    }
    if (Void(location))
    {
        end = true;
        die = true;
        return false;
    }
    return true;
}



bool Level::UpdateObj(Image &screen, int x0, int y0, int x1, int y1, int i)
{
    //for (int i=0; i < moveObj.size(); i++)
    //{
        Object *obj = &moveObj[i];

        int x_beg = obj -> coord.x + obj -> height/2 - obj -> light_radius;
        int y_beg = obj -> coord.y + obj -> height/2 - obj -> light_radius;

        int x_end = obj -> coord.x + obj -> height/2 + obj -> light_radius;
        int y_end = obj -> coord.y + obj -> height/2 + obj -> light_radius;

        //std::cout << i << " " << up <<std::endl;
        
        bool in_screen = (std::max(y_beg , y0) - std::min(y_end, y1) < 0) && (std::max(x_beg , x0) - std::min(x_end, x1) < 0);
        if (in_screen)
        {
            Object objloc = *obj;  //конструктор копирования
            objloc.SetCoord({obj -> coord.x - x0, obj -> coord.y - y0});
            objloc.Lights(screen);
            objloc.DrawImage(screen);
            obj -> UpIndex();
        }
        

    //}
    return in_screen;
}

