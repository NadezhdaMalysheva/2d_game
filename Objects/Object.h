#ifndef OBJ_H
#define OBJ_H

#include "../elementary/Elementary.h"
#include "../Image.h"
#include <iostream>
#include <vector>


namespace
{
    float sigmoid(float x)
    {
        x = exp(-x);
        x = x / ((1 + x)*(1 + x));
        return x;
    } 
}



struct Object
{

    std::vector<std::string> frame;
   // Image *screen; 
    short index = 0;
    short even = 0;
    short speed;
    bool direction = false;
    bool light;
    int height;
    int width;
    int light_radius = 150;
    Point coord;
    
    explicit Object(std::vector <std::string> img_path, Point point = {0, 0}, bool lights=false, short speeds=5)//, Image *main_image = nullptr)
    {
        coord = point;
        frame = img_path;
        Image img(frame[0]);
        height = img.Width();
        width = img.Width();
        if (speeds==0) speed = 5;
        else speed = speeds;
        light = lights;
    }
    

    void DrawImage(Image &screen)
    {
        Image image(frame[index]);
        for(int y = std::max(coord.y, 0); y < std::min(coord.y + height, screen.Height()); ++y)
        {
            for(int x = std::max(coord.x, 0); x < std::min(coord.x + width, screen.Width()); ++x)
            {
                int x_sign = x - coord.x;
                if (direction) x_sign = coord.x + height - x;
                Pixel obj = image.GetPixel(x_sign, coord.y + height - y);
                Pixel scr = screen.GetPixel(x, y);
                //Pixel f = scr + obj;
                screen.PutPixel(x, y, scr + obj);
            }
        }
    }


    void SetCoord(Point point, int sign = 0) 
    {
        if (sign > 0) direction = true;
        if (sign < 0) direction = false;
        coord = point; 
        //UpIndex();
        
    }

    Point GetSize(){return{width, height};}

    void UpIndex()
    {
        if (!even) index = (index + 1)%frame.size();
        even = (even + 1)%speed;
    }

    void Lights(Image &screen)
    {
        if (light)
        {
            int x_beg = coord.x + height/2 - light_radius;
            int y_beg = coord.y + height/2 - light_radius;

            int x_end = coord.x + height/2 + light_radius;
            int y_end = coord.y + height/2 + light_radius;

            for(int j = std::max(y_beg , 0); j < std::min(y_end, screen.Height()); j++)
            for(int i = std::max(x_beg, 0); i < std::min(x_end, screen.Width()); i++)
            {
                screen.PutPixel(i, j, screen.GetPixel(i, j) + Light({i, j}));
            }
        }
    }
    
    
    Pixel Light(Point coords)
    {
        Point shift = {-width/2, -height/2};    
        float radius = coord.radius(coords + shift);
        float coeff = 0.0;
        if (radius <= light_radius)
        {
            coeff = (2-(0.05*(index%4))) * sigmoid(radius/27);
        }
        /*std::cout << coord.x << " " << coord.y << std::endl; 
        std::cout << coords.x << " " << coords.y << std::endl; 
        std::cout << coeff << " " << radius << std::endl; 
        std::cout << int(255*coeff) << std::endl; */

        return {200, 200, 0, 255 * coeff};   
    }
    //void SetScreen(Image *main_image) {screen = main_image;}

};

#endif //OBJ_H
