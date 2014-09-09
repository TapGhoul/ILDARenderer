#ifndef MODELDATA_H
#define MODELDATA_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <3d/quaternion.h>
#include "types.h"

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
    void rotate(vector3d rot);
    std::vector<face *> filterVisible();
    std::vector<face *> filterVisibleOld();
    void recalculateFaceBounds();

private:
};

#endif // MODELDATA_H
