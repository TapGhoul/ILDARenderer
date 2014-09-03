#ifndef MODELDATA_H
#define MODELDATA_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <3d/quaternion.h>

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
    std::vector<face *> linked;
};

class ModelData
{
public:
    ModelData();
    void processData(std::string filepath);
    std::vector<vertex> vertices;
    std::vector<vertex_normal> normals;
    std::vector<line> lines;
    std::vector<face> faces;
    vector3d center;
    Quaternion orientation;
    void normalizeQuaternion();

private:
};

#endif // MODELDATA_H
