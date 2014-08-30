#ifndef LASERFUNCTIONSILDA_H
#define LASERFUNCTIONSILDA_H

#include "ildaserializer.h"
#include <math.h>

class LaserFunctionsILDA
{
public:
    LaserFunctionsILDA();
    static std::vector<coordinate_data> spinCubeYZ(int time);
};

#endif // LASERFUNCTIONSILDA_H
