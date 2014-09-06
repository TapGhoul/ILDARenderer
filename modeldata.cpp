#include "modeldata.h"
#include <math.h>
#include <Eigen/Geometry>

//using namespace Eigen;

ModelData::ModelData()
{
}

void ModelData::processData(std::string filepath)
{
    std::ifstream in(filepath);
    if (!in.good()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }
    std::string data;
    while (in >> data) {
        if (data == "v") {
            vertex vert;
            in >> vert.pos.x >> vert.pos.y >> vert.pos.z;
            vertices.insert(vertices.end(), vert);
        } else if (data == "vn") {
            vertex_normal norm;
            in >> norm.dir.x >> norm.dir.y >> norm.dir.z;
            normals.insert(normals.end(), norm);
        } else if (data == "f") {
            face f;
            char tmpchr;
            vertex * vert;
            vertex_normal * norm;
            int vertI, normI;
            while (in.peek() != '\n') {
                in >> vertI >> tmpchr >> tmpchr >> normI;
                f.verts.insert(f.verts.end(), &vertices[vertI - 1]);
                f.normal = (vertex_normal*)&normals[normI - 1];
            }
            faces.insert(faces.end(), f);
        }
    }
    in.close();
    for (std::vector<vertex>::iterator it = vertices.begin(); it != vertices.end(); it++) {
        vertex vert = *it;
        int index = it - vertices.begin() + 1;
        std::cout << "Vertex " << index << ": " << vert.pos.x << "," << vert.pos.y << "," << vert.pos.z << std::endl;
        center.x += vert.pos.x;
        center.y += vert.pos.y;
        center.z += vert.pos.z;
    }
    center.x /= vertices.size();
    center.y /= vertices.size();
    center.z /= vertices.size();
    for (std::vector<vertex_normal>::iterator it = normals.begin(); it != normals.end(); it++) {
        vertex_normal norm = *it;
        int index = it - normals.begin() + 1;
        std::cout << "Normal " << index << ": " << norm.dir.x << "," << norm.dir.y << "," << norm.dir.z << std::endl;
    }
    for (std::vector<face>::iterator it = faces.begin(); it != faces.end(); it++) {
        face faceElem = *it;
        int index = it - faces.begin() + 1;
        std::cout << "Face " << index << ": " << std::endl;
        for (std::vector<vertex*>::iterator it1 = faceElem.verts.begin(); it1 != faceElem.verts.end(); it1++) {
            vertex * vert = *it1;
            int index1 = it1 - faceElem.verts.begin() + 1;
            std::cout << "\tVertex " << index1 << ": " << vert->pos.x << "," << vert->pos.y << "," << vert->pos.z << std::endl;
        }
        std::cout << "\tNormal: " << faceElem.normal->dir.x << "," << faceElem.normal->dir.y << "," << faceElem.normal->dir.z << std::endl;
    }
}

bool sortZ (face a, face b)
{
    double zIndA = 0;
    double zIndB = 0;
    for (int i=0; i < a.verts.size(); i++)
    {
        zIndA += a.verts[i]->pos.z;
    }
    for (int i=0; i < b.verts.size(); i++)
    {
        zIndB += b.verts[i]->pos.z;
    }
    zIndA /= a.verts.size();
    zIndB /= b.verts.size();
    return zIndA > zIndB;
}


// WARNING: This operation is DESTRUCTIVE!
void ModelData::rotate(vector3d rot)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        vector3d norm;
        vector3d pos = vertices[i].pos;

        norm.x = pos.x - center.x;
        norm.y = pos.y - center.y;
        norm.z = pos.z - center.z;
        //std::sort

        //pos.x = norm.x * sin(rotDeg) + norm.y * -cos(rotDeg);
        //pos.y = norm.y * sin(rotDeg) + norm.z * -cos(rotDeg);
        //pos.z = norm.z * sin(rotDeg) + norm.x * -cos(rotDeg);


        // Euler rotation, order XYZ
        pos.x = norm.x;
        pos.y = norm.y * cos(rot.x) - norm.z * sin(rot.x);
        pos.z = norm.z * cos(rot.x) + norm.y * sin(rot.x);

        norm = pos;

        pos.x = norm.x * cos(rot.y) + norm.z * sin(rot.y);
        pos.z = norm.z * cos(rot.y) - norm.x * sin(rot.y);

        norm = pos;

        pos.x = norm.x * cos(rot.z) - norm.y * sin(rot.z);
        pos.y = norm.y * cos(rot.z) + norm.x * sin(rot.z);

        pos.x += center.x;
        pos.y += center.y;
        pos.z += center.z;

        vertices[i].pos = pos;
    }
    std::sort(faces.begin(), faces.end(), sortZ);
}

std::vector<face *> ModelData::filterVisible()
{
    std::vector<face *> visibleFaces;
    for (std::vector<face>::iterator it = faces.begin(); it != faces.end(); it++)
    {
        face * f = (face *) &*it;
        bool canDraw = true;
        for (std::vector<face *>::iterator it1 = visibleFaces.begin(); it1 != visibleFaces.end(); it1++)
        {
            face * f1 = (face *) *it1;
            double boundsSizeX;
            double boundsSizeY;
            bool compared = false;
            for (std::vector<vertex *>::iterator it2 = f->verts.begin(); it2 != f->verts.end(); it2++)
            {
                vertex * v = (vertex *) *it2;
                if ((v->pos.x < f1->bounds.min.x) == (v->pos.x > f1->bounds.max.x) &&
                    (v->pos.y < f1->bounds.min.y) == (v->pos.y > f1->bounds.max.y))
                {
                    if (!compared)
                    {
                        // Remove "- 0.01" leniency when starting to process with multiple layers for angled faces
                        boundsSizeX = std::abs(f1->bounds.max.x - f1->bounds.min.x) - 0.01;
                        boundsSizeY = std::abs(f1->bounds.max.y - f1->bounds.min.y) - 0.01;
                        compared = true;
                    }
                    if (std::max(std::abs(v->pos.x - f1->bounds.min.x), std::abs(v->pos.x - f1->bounds.max.x)) < boundsSizeX &&
                        std::max(std::abs(v->pos.y - f1->bounds.min.y), std::abs(v->pos.y - f1->bounds.max.y)) < boundsSizeY)
                    {
                    canDraw = false;
                    break;
                    }
                }
            }
        }
        if (canDraw)
        {
            f->bounds.min = f->bounds.max = f->verts[0]->pos;
            for (std::vector<vertex *>::iterator it1 = f->verts.begin(); it1 != f->verts.end(); it1++)
            {
                vertex * v = (vertex *) *it1;
                vector3d pos = v->pos;
                f->bounds.min.x = std::min(pos.x, f->bounds.min.x);
                f->bounds.min.y = std::min(pos.y, f->bounds.min.y);

                f->bounds.max.x = std::max(pos.x, f->bounds.max.x);
                f->bounds.max.y = std::max(pos.y, f->bounds.max.y);
            }
            visibleFaces.push_back(f);
        }
    }
    return visibleFaces;
}
