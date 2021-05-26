#ifndef MAIN_IMAGE_H
#define MAIN_IMAGE_H

#include <string>
#include <memory>
#include <iostream>
#include "./elementary/Elementary.h"

constexpr int tileSize = 16;
constexpr Pixel backgroundColor{0, 0, 0, 0};

struct Image
{
  explicit Image(const std::string &a_path);
  Image(int a_width, int a_height, int a_channels);
  Image(const Image & img);
  Image();

  int Save(const std::string &a_path);

  int Width()    const { return width; }
  int Height()   const { return height; }
  int Channels() const { return channels; }
  size_t Size()  const { return size; }
  Pixel* Data()        { return  data; }

  Pixel GetPixel(int x, int y) const { return data[width * y + x];}
  void PutPixel(int x, int y, const Pixel &pix) { data[width* y + x] = pix; }
  void GetPixels(int x, int y, int h, int w, Image &ptr);
  void PutPixels(int x, int y, int h, int w, Image &img_ptr, int x1=0, int y1=0);
  bool Allocated() {return self_allocated;}
  Image & operator=(const Image & img);


  ~Image();

private:
  int width = -1;
  int height = -1;
  int channels = 3;
  size_t size = 0;
  Pixel *data = nullptr;
  bool self_allocated = false;
  std::string path = " ";
};

#endif //MAIN_IMAGE_H
