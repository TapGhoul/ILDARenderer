#ifndef TYPES_H
#define TYPES_H
#include <vector>
#include <math.h>

struct vector3d {
    double x, y, z;
    vector3d& normalize() {double mag = sqrt(x * x + y * y + z * z); x /= mag; y /= mag; z /= mag; return *this;}
    const vector3d& operator*= (double mult) {x *= mult; y *= mult; z *= mult; return *this;}
    const vector3d& operator/= (double div) {x /= div; y /= div; z /= div; return *this;}
};

struct vector2d {
    double x, y;
};

struct vertex {
    unsigned int index;
    vector3d pos;
    vector3d origPos;
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
