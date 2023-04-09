#include "object.hpp"
#include <Eigen/src/Core/Matrix.h>
#include <tiny_obj_loader.h>

#include <iostream>
#include <assert.h>

#define TINYOBJLOADER_IMPLEMENTATION

tinyobj::ObjReaderConfig reader_config;
tinyobj::ObjReader reader;

Object::Object() {
    this->position << 0.0f, 0.0f, 0.0f;
}

Object::Object(std::vector<float> vertices, std::vector<int> indexes, Vector3f position) {
    this->position = position;
    // FIX: read vector<Vector3f> from vector<float>
}



Object::Object(const char *filename, int filetype) {
    this->position << 0.0f, 0.0f, 0.0f;
    // TODO: use library
    reader_config.mtl_search_path = "";

    if (!reader.ParseFromFile(filename, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "[ERR]TinyObjReader: " << reader.Error();
        }
        exit(1);
    }
    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }
    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // for every shape
    for (size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;
        // for every face
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            // fv vertices in this face
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // since faces are all triangles, fv is always equal to 3
            std::vector<Vector3f> face(3);
            std::vector<Vector3f> normal(3);
            std::vector<Vector2f> uv(3);
            // for every vertex
            for (size_t v = 0; v < fv; v++) {
                assert(fv == 3);
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                face[v] << attrib.vertices[3*size_t(idx.vertex_index)+0],
                           attrib.vertices[3*size_t(idx.vertex_index)+1],
                           attrib.vertices[3*size_t(idx.vertex_index)+2];


                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    normal[v] << attrib.normals[3*size_t(idx.normal_index)+0],
                                 attrib.normals[3*size_t(idx.normal_index)+1],
                                 attrib.normals[3*size_t(idx.normal_index)+2];
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    uv[v] << attrib.texcoords[2*size_t(idx.texcoord_index)+0],
                             attrib.texcoords[2*size_t(idx.texcoord_index)+1];
                }

                // Optional: vertex colors
                // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
            } // for face end
            this->uvs.push_back(uv);
            this->normals.push_back(normal);
            this->faces.push_back(face);
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }
}


int Object::nface() {
    return faces.size();
}

std::vector<Vector3f> Object::get_face(int index) {
    return faces[index];
}


std::vector<Vector3f> Object::get_normal(int index) {
    return normals[index];
}

std::vector<Vector2f> Object::get_uv(int index) {
    return uvs[index];
}

Vector3f Object::get_position() {
    return position;
}
void Object::set_position(Vector3f position) {
    this->position = position;
}

// get Model Matrix
Matrix4f Object::get_model() {
    // TODO: rotate angle theta set to 0
    float rx = 0, ry = 0, rz = 0;
    // TODO: scale set to 1
    float sx = 1, sy = 1, sz = 1;

    // Translate
    Matrix4f Mtranslate = Matrix4f::Identity();
    for (int i = 0; i < 3; i++) {
        // x/w, y/w, z/w
        Mtranslate(i, 3) = position(i); /// position(3);
    }

    // Scale
    Matrix4f Mscale;
    Mscale << sx, 0, 0, 0,
              0, sy, 0, 0,
              0, 0, sz, 0,
              0, 0, 0, 1;

    Matrix4f MrotateX;
    MrotateX << 1,          0,          0,          0,
                0,          cos(rx),    -sin(rx),   0,
                0,          sin(rx),    cos(rx),    0,
                0,          0,          0,          1;

    Matrix4f MrotateY;
    MrotateY << cos(ry),    0,          sin(ry),    0,
                0,          1,          0,          0,
                -sin(ry),   0,          cos(ry),    0,
                0,          0,          0,          1; 

    Matrix4f MrotateZ;
    MrotateZ << cos(rz),    -sin(rz),   0,          0,
                sin(rz),    cos(rz),    0,          0,
                0,          0,          1,          0,
                0,          0,          0,          1; 

    // Rotate
    Matrix4f Mrotate = (MrotateZ * MrotateY * MrotateX);
    Matrix4f temp = (Mrotate*Mscale*Mtranslate);
    // FIX: column 4 not a number
    std::cout << "[LOG]Object::get_model: model: " << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << temp(i, j) << " ";
        }
        std::cout << std::endl;
    }




    return (Mrotate*Mscale*Mtranslate);
}


// TODO: ~Object()
