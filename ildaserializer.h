#ifndef ILDASERIALIZER_H
#define ILDASERIALIZER_H

#include <iostream>
#include <vector>


typedef struct colour_data {
    __uint8_t r;
    __uint8_t g;
    __uint8_t b;
} colour_data;

typedef struct coordinate_data {
  __int16_t x;
  __int16_t y;
  __uint8_t colour;
  bool blanking;
} coordinate_data;

class ILDASerializer
{
public:
    explicit ILDASerializer();
    static std::vector<char> coordinates(std::vector<std::vector<coordinate_data> > coords);
    static std::vector<char> colourTable();

};

#endif // ILDASERIALIZER_H
