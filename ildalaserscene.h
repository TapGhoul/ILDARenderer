#ifndef ILDALASERSCENE_H
#define ILDALASERSCENE_H

#include "ildaserializer.h"

class ILDALaserScene
{
public:
    ILDALaserScene();

private:
    __int16_t xPos;
    __int16_t yPos;
    colour_data colour;
    bool blanking;
    void setColour(colour_data newColour);
};

#endif // ILDALASERSCENE_H
