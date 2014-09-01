#ifndef MODELDATA_H
#define MODELDATA_H

#include <vector>

struct vector3d {
    double x;
    double y;
    double z;
};

struct vertex {
    unsigned int index;
    vector3d pos;
    std::vector<vertex *> linked;
};

struct line {
    unsigned int index;
    vertex * verta;
    vertex * vertb;
    std::vector<line *> linked;
};

struct face {
    unsigned int index;
    std::vector<line *> edges;
    vector3d normal;
    struct bounds {
        vector3d min;
        vector3d max;
    } bounds;
    std::vector<face *> linked;
};

class ModelData
{
public:
    ModelData();
};

#endif // MODELDATA_H
