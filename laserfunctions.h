#ifndef LASERFUNCTIONS_H
#define LASERFUNCTIONS_H

#include "laserscene.h"

class LaserFunctions
{
public:
    LaserFunctions();

    static void cycleColours(LaserScene *scene, int time);
    static void spinSquareEdges(LaserScene *scene, int time);
};

#endif // LASERFUNCTIONS_H