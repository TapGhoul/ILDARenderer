#ifndef TYPES_H
#define TYPES_H
#include <vector>
#include <math.h>

struct vector3d {
    double x, y, z;
    vector3d& normalize() {double mag = sqrt(x * x + y * y + z * z); x /= mag; y /= mag; z /= mag; return *this;}
    vector3d& abs() {x = fabs(x); y = fabs(y); z = fabs(z); return *this;}
    const vector3d& operator*= (double mult) {x *= mult; y *= mult; z *= mult; return *this;}
    const vector3d& operator/= (double div) {x /= div; y /= div; z /= div; return *this;}
    vector3d operator-(const vector3d& rhs) {return {x - rhs.x, y - rhs.y, z - rhs.z};}
    bool operator==(const vector3d& rhs) {return x == rhs.x && y == rhs.y && z == rhs.z;}
    bool operator<=(const vector3d& rhs) {return x + y + z <= rhs.x + rhs.y + rhs.z;}
    bool eq2d(const vector3d& rhs) {return x == rhs.x && y == rhs.y;}
    bool lteq2d(const vector3d& rhs) {return x + y <= rhs.x + rhs.y;}
};

struct vector2d {
    double x, y;
};

struct segment3d {
    vector3d start, end;
    bool operator==(const segment3d& rhs) {return (start == rhs.start && end == rhs.end) || (start == rhs.end && end == rhs.start);}
    segment3d operator-(const vector3d& rhs) {return {start - rhs, end - rhs};}
    const vector3d getDirection() {vector3d direction = {end.x - start.x, end.y - start.y, end.z - start.z}; direction.normalize(); return direction;}
    double getLength() {return sqrt((end.x - start.x) * (end.x - start.x) + (end.y - start.y) * (end.z - start.z) + (end.z - start.z) * (end.z - start.z));}
    bool eq2d(const segment3d& rhs) {return (start.eq2d(rhs.start) && end.eq2d(rhs.end)) || (start.eq2d(rhs.end) && end.eq2d(rhs.start));}
    bool lteq2d(segment3d rhs) {return getMidpoint().lteq2d(rhs.getMidpoint());}
    vector3d getMidpoint() {return {(start.x + end.x) / 2, (start.y + end.y) / 2, (start.z + end.z) / 2};}
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
