#ifndef TYPES_H
#define TYPES_H
#include <vector>

struct vector3d {
    double x, y, z;
};

struct vector2d {
    double x, y;
};

struct vertex {
    unsigned int index;
    vector3d pos;
    std::vector<vertex *> linked;
};

struct vertex_normal {
    unsigned int index;
    vector3d dir; // Direction
};

struct line {
    unsigned int index;
    vertex * verta;
    vertex * vertb;
    std::vector<line *> linked;
};

struct face {
    unsigned int index;
    std::vector<vertex *> verts;
    std::vector<line *> edges;
    vertex_normal * normal;
    struct bounds {
        vector3d min;
        vector3d max;
    } bounds;
    vector3d center;
    bool canDraw;
    bool canDraw1;
    std::vector<face *> linked;
};

#endif // TYPES_H
