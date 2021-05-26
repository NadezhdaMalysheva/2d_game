#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"



Image::Image(const std::string &a_path)
{
  if((data = (Pixel*)stbi_load(a_path.c_str(), &width, &height, &channels, 0)) != nullptr)
  {
    //std::cout << "Open img" << std::endl;
    size = width * height * channels; 
    path = a_path;
    //std::cout << width << ' ' << height << std::endl;
  }
  else 
  {
    std::cout << "Can't(((("<< std::endl;
    std::cout << a_path<< std::endl;
  }
}

Image::Image(int a_width, int a_height, int a_channels)
{
  data = new Pixel[a_width * a_height]{};
  if(data != nullptr)
  {
    width = a_width;
    height = a_height;
    size = a_width * a_height * a_channels;
    channels = a_channels;
    self_allocated = true;
    path = "None)))";
  }
}

Image::Image(const Image& img)
{
  width = img.width;
  height = img.height;
  channels = img.channels;
  size = width * height * channels; 
  data = new Pixel[width * height]{};
  if(data != nullptr)
  {
    for (int i=0; i<width * height; i++)
    {
      data[i] = img.data[i];
    }
  }
  self_allocated = true;
}

Image::Image() 
{
  //std::cout << "Пуст"<<std::endl;
  data = nullptr;
  path = "Null";

}

Image & Image::operator=(const Image & img)
{
  if(self_allocated)
    delete [] data;
  else
  {
    if (data != nullptr) stbi_image_free(data);
  }
  width = img.width;
  height = img.height;
  channels = img.channels;
  size = width * height * channels; 
  data = new Pixel[width * height]{};
  if(data != nullptr)
  {
    for (int i=0; i<width * height; i++)
    {
      data[i] = img.data[i];
    }
  }
  self_allocated = true;
  return *this;
}

void Image::PutPixels(int x, int y, int h, int w, Image &ptr, int x1, int y1)
  {
    for (int i=x; i<h+x; i++) 
    for (int j=y; j<w+y; j++)
    {
      PutPixel(j, i, ptr.GetPixel(y1+j-y, x1+i-x));
    }

  }

void Image::GetPixels(int x, int y, int h, int w, Image &ptr)
  {
    for (int i=x; i<h+x; i++) 
    for (int j=y; j<w+y; j++)
    {
      PutPixel(j, i, ptr.GetPixel(j, i));
    }

  }

int Image::Save(const std::string &a_path)
{
  auto extPos = a_path.find_last_of('.');
  if(a_path.substr(extPos, std::string::npos) == ".png" || a_path.substr(extPos, std::string::npos) == ".PNG")
  {
    stbi_write_png(a_path.c_str(), width, height, channels, data, width * channels);
  }
  else if(a_path.substr(extPos, std::string::npos) == ".jpg" || a_path.substr(extPos, std::string::npos) == ".JPG" ||
          a_path.substr(extPos, std::string::npos) == ".jpeg" || a_path.substr(extPos, std::string::npos) == ".JPEG")
  {
    stbi_write_jpg(a_path.c_str(), width, height, channels, data, 100);
  }
  else
  {
    //std::cerr << "Unknown file extension: " << a_path.substr(extPos, std::string::npos) << "in file name" << a_path << "\n";
    return 1;
  }

  return 0;
}

Image::~Image()
{
  if(self_allocated)
    delete [] data;
  else
  {
    if (data != nullptr) stbi_image_free(data);
  }
}