#ifndef SATCOLLISION_H
#define SATCOLLISION_H

#include <vector>
#include "types.h"
//#include "modeldata.h"

class SATCollision
{
public:
    SATCollision();
    bool SAT(std::vector<vector3d> polyA, std::vector<vector3d> polyB);
    bool SATVerts(std::vector<vertex *> polyA, std::vector<vertex *> polyB);

private:
    double dotProduct(vector3d vec1, vector3d vec2);
    vector3d normalize(vector3d vec);
    vector3d perpendicular(vector3d vec);
    vector3d vecDir(vector3d pointA, vector3d pointB);
    double *project(std::vector<vector3d> verts, vector3d axis);
    double *projectFace(std::vector<vertex *> verts, vector3d axis);
    bool contains(double point, double *bounds);
    bool overlap(double *polyA, double *polyB);
};

#endif // SATCOLLISION_H
