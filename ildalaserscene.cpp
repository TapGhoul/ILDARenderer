#include "ildalaserscene.h"

ILDALaserScene::ILDALaserScene() :
    xPos(0), yPos(0), blanking(false)
{
    colour.r = 255;
    colour.g = 255;
    colour.b = 255;
}

void ILDALaserScene::setColour(colour_data newColour) {
    colour.r = newColour.r;
    colour.g = newColour.g;
    colour.b = newColour.b;
}
