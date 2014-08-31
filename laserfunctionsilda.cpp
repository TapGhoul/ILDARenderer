#include "laserfunctionsilda.h"

using namespace std;

LaserFunctionsILDA::LaserFunctionsILDA()
{
}

coordinate_data setPoint(__int16_t x, __int16_t y, __int8_t colour = 1, bool blanking = false) {
    coordinate_data output;
    output.x = x;
    output.y = y;
    output.colour = colour;
    output.blanking = blanking;
    return output;
}

std::vector<coordinate_data> LaserFunctionsILDA::spinCubeYZ(int time)
{
    vector<coordinate_data> coords(21);
    double angle = (time*M_PI)/128.0f-M_PI;
    int sX = sin(angle)*8000;
    int sY = cos(angle)*8000;
    int scale = 2000;

    coords[0] = setPoint(sX, sY);
    coords[1] = setPoint(sY, -sX);
    coords[2] = setPoint(scale + sY, scale - sX);
    coords[3] = setPoint(scale - sX, scale - sY);
    coords[4] = setPoint(-sX, -sY);
    coords[5] = setPoint(-sY, sX);
    coords[6] = setPoint(scale - sY, scale + sX);
    coords[7] = setPoint(scale + sX, scale + sY);
    coords[8] = setPoint(sX, sY);
    coords[9] = setPoint(-sY, sX);
    coords[10] = setPoint(scale/2, scale/2, 2);
    coords[11] = setPoint(scale-sY, scale+sX, 2);
    coords[12] = setPoint(scale-sX, scale-sY);
    coords[13] = setPoint(scale/2, scale/2, 3);
    coords[14] = setPoint(-sX, -sY, 3);
    coords[15] = setPoint(sY, -sX);
    coords[16] = setPoint(scale/2, scale/2, 4);
    coords[17] = setPoint(scale+sY, scale-sX, 4);
    coords[18] = setPoint(scale+sX, scale+sY);
    coords[19] = setPoint(scale/2, scale/2, 5);
    coords[20] = setPoint(sX, sY, 5);

    return coords;
}
