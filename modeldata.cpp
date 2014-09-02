#include "modeldata.h"


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
    }
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
