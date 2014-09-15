#include "modeldata.h"
#include <math.h>
#include <algorithm>
#include "satcollision.h"

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
            vert.origPos = vert.pos;
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

bool sortXYZ (vector3d a, vector3d b)
{
    return sqrt(a.x * a.x + a.y * a.y) > sqrt(b.x * b.x + b.y * b.y);
}

void ModelData::rotate(vector3d rot)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        vector3d norm;
        vector3d pos = vertices[i].origPos;

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

void ModelData::recalculateFaceBounds()
{
    for (std::vector<face>::iterator it = faces.begin(); it != faces.end(); it++)
    {
        face * f = (face *) &*it;
        f->canDraw = true;
        f->canDraw1 = true;
        f->bounds.min = f->bounds.max = f->verts[0]->pos;
        f->center.x = f->center.y = f->center.z = 0;
        for (std::vector<vertex *>::iterator it1 = f->verts.begin(); it1 != f->verts.end(); it1++)
        {
            vertex * v = (vertex *) *it1;
            vector3d pos = v->pos;
            f->bounds.min.x = std::min(pos.x, f->bounds.min.x);
            f->bounds.min.y = std::min(pos.y, f->bounds.min.y);
            f->bounds.min.z = std::min(pos.z, f->bounds.min.z);

            f->bounds.max.x = std::max(pos.x, f->bounds.max.x);
            f->bounds.max.y = std::max(pos.y, f->bounds.max.y);
            f->bounds.max.z = std::max(pos.z, f->bounds.max.z);
            f->center.x += v->pos.x;
            f->center.y += v->pos.y;
            f->center.z += v->pos.z;
        }
        f->center.x /= f->verts.size();
        f->center.y /= f->verts.size();
        f->center.z /= f->verts.size();
    }
}

std::vector<face *> ModelData::filterVisible(int allowedOverlaps)
{
    recalculateFaceBounds();
    std::vector<face *> visibleFaces;
    for (std::vector<face>::iterator it = faces.begin(); it != faces.end(); it++)
    {
        face * f = (face *) &*it;
        bool forceDraw = false;
        int visibleVerts = f->verts.size();
        int hiddenCount = 0;
        SATCollision coll;
        for (std::vector<face>::iterator it1 = faces.begin(); it1 != faces.end(); it1++)
        {
            face * fComp = (face *) &*it1;
            if (f->center.z <= fComp->center.z &&
                    (f->bounds.min.x < fComp->bounds.max.x) == (f->bounds.max.x >= fComp->bounds.min.x) &&
                    (f->bounds.min.y < fComp->bounds.max.y) == (f->bounds.max.y >= fComp->bounds.min.y))
            {
                //if (std::find_first_of(f->verts.begin(), f->verts.end(),
                //                       fComp->verts.begin(), fComp->verts.end()) == f->verts.end())
                    f->canDraw = coll.SATVerts(f->verts, fComp->verts);
            }
            if (!f->canDraw)
                if (hiddenCount < allowedOverlaps)
                {
                    hiddenCount++;
                    f->canDraw = true;
                } else
                    break;
        }
        visibleFaces.push_back(f);
    }
    return visibleFaces;
}

std::vector<face *> ModelData::filterVisibleOld()
{
    recalculateFaceBounds();
    std::vector<face *> visibleFaces;
    for (std::vector<face>::iterator it = faces.begin(); it != faces.end(); it++)
    {
        face * f = (face *) &*it;
        if (!f->canDraw)
            continue;
        //forceDraw = false;
        int intFaces = 0;
        for (std::vector<face>::iterator it1 = faces.begin(); it1 != faces.end(); it1++)
        {
            face * f1 = (face *) &*it1;
            bool skipPair = false;
            if ((f->bounds.min.x < f1->bounds.max.x) == (f->bounds.max.x >= f1->bounds.min.x) &&
                    (f->bounds.min.y < f1->bounds.max.y) == (f->bounds.max.y >= f1->bounds.min.y))//f->center.z <= f1->center.z)
            {
                for (std::vector<vertex *>::iterator it2 = f->verts.begin(); it2 != f->verts.end(); it2++)
                {
                    vertex * v = (vertex *) *it2;
                    for (std::vector<vertex *>::iterator it3 = f1->verts.begin(); it3 != f1->verts.end(); it3++)
                    {
                        vertex * v1 = (vertex *) *it3;
                        vertex * v_n = (vertex *) *(it2+1 == f->verts.end() ? f->verts.begin() : it2+1);
                        vertex * v1_n = (vertex *) *(it3+1 == f1->verts.end() ? f1->verts.begin() : it3+1);
                        if (v == v1 || v == v1_n || v1 == v_n || v_n == v1_n) {
                            //forceDraw = true;
                            //break;
                            //f->canDraw = true;
                            //f1->canDraw = false;
                            skipPair = true;
                            f->canDraw = true;
                            //canDraw = false;
                            //intVerts++;
                            //f->canDraw = !f->canDraw;
                            continue;
                        }
                        //bool checked = false;
                        if ((v->pos.y >= v1->pos.y) == (v->pos.y < v1_n->pos.y)) {
                            double xStep = (v->pos.y - v1->pos.y)/(v1_n->pos.y - v1->pos.y);
                            if (v->pos.x < v1->pos.x + xStep * (v1_n->pos.x - v1->pos.x))
                            {
                                if (f->center.z <= f1->center.z)
                                    f->canDraw = !f->canDraw;
                                else
                                    f1->canDraw = !f1->canDraw;
                                //checked = true;
                            }
                        }
#if FALSE
                        if (false && !checked && (v1->pos.y >= v->pos.y) == (v1->pos.y < v_n->pos.y)) {
                            double xStep = (v1->pos.y - v->pos.y)/(v_n->pos.y - v->pos.y);
                            if (v1->pos.x < v->pos.x + xStep * (v_n->pos.x - v->pos.x))
                            {
                                if (f->center.z <= f1->center.z)
                                    f->canDraw = !f->canDraw;
                                else
                                    f1->canDraw = !f1->canDraw;
                            }
                        }
#endif
                    }
                    if (skipPair)//!f->canDraw || !f->canDraw1 || skipPair)
                        break;
                }
            }
            if (!f->canDraw || !f->canDraw1 || skipPair)
                break;
        }
        if (f->canDraw && f->canDraw1)
        {
            /*f->bounds.min = f->bounds.max = f->verts[0]->pos;
            for (std::vector<vertex *>::iterator it1 = f->verts.begin(); it1 != f->verts.end(); it1++)
            {
                vertex * v = (vertex *) *it1;
                vector3d pos = v->pos;
                f->bounds.min.x = std::min(pos.x, f->bounds.min.x);
                f->bounds.min.y = std::min(pos.y, f->bounds.min.y);

                f->bounds.max.x = std::max(pos.x, f->bounds.max.x);
                f->bounds.max.y = std::max(pos.y, f->bounds.max.y);
            }*/
            //visibleFaces.push_back(f);
        }
        visibleFaces.push_back(f);
    }
    return visibleFaces;
}

/*coordinate_data setPoint(__int16_t x, __int16_t y, __int8_t colour = 1, bool blanking = false) {
    coordinate_data output;
    output.x = x;
    output.y = y;
    output.colour = colour;
    output.blanking = blanking;
    return output;
}*/

bool sortSegs(segment3d a, segment3d b)
{
    vector3d aMid = a.getMidpoint().abs();
    vector3d bMid = b.getMidpoint().abs();
    if (aMid.x == bMid.x)
        return aMid.y > bMid.y;
    else
        return aMid.x > bMid.x;
}

bool sortSegsNeg(segment3d a, segment3d b)
{
    vector3d aMid = a.getMidpoint().abs();
    vector3d bMid = b.getMidpoint().abs();
    if (aMid.x == bMid.x)
        return aMid.y < bMid.y;
    else
        return aMid.x < bMid.x;
}

std::vector<segment3d> ModelData::filterEdges()
{
    recalculateFaceBounds();
    std::vector<vector3d> outVerts;
    std::vector<segment3d> lines;
    std::vector<segment3d> linesOut(500);
    for (std::vector<face>::iterator it = faces.begin(); it != faces.end(); it++)
    {
        face f = (face) *it;
        for (std::vector<vertex *>::iterator it1 = f.verts.begin(); it1 != f.verts.end(); it1++)
        {
            vertex * v = (vertex *) *it1;
            vertex * v_n = (vertex *) *(it1+1 == f.verts.end() ? f.verts.begin() : it1+1);
            lines.push_back({v->pos - center, v_n->pos - center});
        }
        //outVerts.push_back({f.bounds.max.x, f.bounds.max.y, f.bounds.max.z});
    }
    //std::sort(outVerts.begin(), outVerts.end(), sortXYZ);
    std::sort(lines.begin(), lines.end(), sortSegs);
    std::unique(lines.begin(), lines.end());
    std::copy_n(lines.begin(), 250, linesOut.begin());
    std::sort(lines.begin(), lines.end(), sortSegsNeg);
    std::copy_n(lines.begin(), 250, linesOut.begin()+250);
    return linesOut;
}

std::vector<coordinate_data> ModelData::exportILDA(bool showHidden)
{
    std::vector<coordinate_data> coords;
    for (std::vector<face>::iterator it = faces.begin(); it != faces.end(); it++)
    {
        face f = (face) *it;
        if (showHidden || (f.canDraw && f.canDraw1))
            for (std::vector<vertex *>::iterator it1 = f.verts.begin(); it1 != f.verts.end(); it1++)
            {
                vertex * v = (vertex *) *it1;
                coords.push_back(ILDASerializer::setPoint(v->pos.x*16000, v->pos.y*16000, f.canDraw && f.canDraw1 ? 1 : 2, it1 == f.verts.begin()));
            }
    }
    return coords;
}
