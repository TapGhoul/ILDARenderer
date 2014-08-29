#ifndef ILDASERIALIZER_H
#define ILDASERIALIZER_H

#include <iostream>
#include <vector>

class ILDASerializer
{
public:
    explicit ILDASerializer();
    static std::vector<unsigned char> * coordinates();
    static std::vector<unsigned char> * colourTable();

};

#endif // ILDASERIALIZER_H
