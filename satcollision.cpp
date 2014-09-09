#include "satcollision.h"
#include <math.h>

SATCollision::SATCollision()
{
}

double SATCollision::dotProduct(vector3d vec1, vector3d vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

vector3d SATCollision::normalize(vector3d vec)
{
    double vMag = sqrt(vec.x * vec.x + vec.y * vec.y);
    vector3d nVec = {vec.x/vMag, vec.y/vMag};
    return nVec;
}

vector3d SATCollision::perpendicular(vector3d vec)
{
    vector3d pVec = {-vec.y, vec.x};
    return pVec;
}

vector3d SATCollision::vecDir(vector3d pointA, vector3d pointB)
{
    vector3d dir = {pointB.x - pointA.x, pointB.y - pointA.y};
    return dir;
}

double *SATCollision::project(std::vector<vector3d> verts, vector3d axis)
{
    vector3d axisNorm = normalize(axis);
    double min = dotProduct(verts[0],axisNorm);
    double max = min;
    for (std::vector<vector3d>::iterator it = verts.begin() + 1; it != verts.end(); it++)
    {
        double projVert = dotProduct((vector3d) *it, axis);
        min = std::min(min, projVert);
        max = std::max(max, projVert);
    }
    return new double[2]{min, max};
}

double *SATCollision::projectFace(std::vector<vertex *> verts, vector3d axis)
{
    vector3d axisNorm = normalize(axis);
    double min = dotProduct(verts[0]->pos,axisNorm);
    double max = min;
    for (std::vector<vertex *>::iterator it = verts.begin() + 1; it != verts.end(); it++)
    {
        double projVert = dotProduct((*it)->pos, axis);
        min = std::min(min, projVert);
        max = std::max(max, projVert);
    }
    return new double[2]{min, max};
}

bool SATCollision::contains(double point, double *bounds)
{
    double boundA = bounds[0];
    double boundB = bounds[1];
    if (boundA > boundB)
    {
        boundB = boundA;
        boundA = bounds[1];
    }
    return (point >= boundA && point <= boundB);
}

bool SATCollision::overlap(double *polyA, double *polyB)
{
    return contains(polyA[0], polyB) && contains(polyA[1], polyB) &&
            contains(polyB[0], polyA) && contains(polyB[1], polyA);
}

bool SATCollision::SAT(std::vector<vector3d> polyA, std::vector<vector3d> polyB)
{
    for (std::vector<vector3d>::iterator it = polyA.begin(); it != polyA.end(); it++)
    {
        vector3d vec = (vector3d) *it;
        vector3d axis = perpendicular(vec);
        double * boundsA = project(polyA, axis);
        double * boundsB = project(polyB, axis);
        if (!overlap(boundsA, boundsB))
            return false;
    }
    for (std::vector<vector3d>::iterator it = polyB.begin(); it != polyB.end(); it++)
    {
        vector3d vec = (vector3d) *it;
        vector3d axis = perpendicular(vec);
        double * boundsA = project(polyA, axis);
        double * boundsB = project(polyB, axis);
        if (!overlap(boundsA, boundsB))
            return false;
    }
    return true;
}

bool SATCollision::SATVerts(std::vector<vertex *> polyA, std::vector<vertex *> polyB)
{
    for (std::vector<vertex *>::iterator it = polyA.begin(); it != polyA.end(); it++)
    {
        vector3d vec = ((vertex *)*it)->pos;
        vector3d axis = perpendicular(vec);
        double * boundsA = projectFace(polyA, axis);
        double * boundsB = projectFace(polyB, axis);
        if (!overlap(boundsA, boundsB))
            return false;
    }
    for (std::vector<vertex *>::iterator it = polyB.begin(); it != polyB.end(); it++)
    {
        vector3d vec = ((vertex *)*it)->pos;
        vector3d axis = perpendicular(vec);
        double * boundsA = projectFace(polyA, axis);
        double * boundsB = projectFace(polyB, axis);
        if (!overlap(boundsA, boundsB))
            return false;
    }
    return true;
}
