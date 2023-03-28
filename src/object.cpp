#include "object.hpp"
#include <Eigen/src/Core/Matrix.h>

Object::Object() {
    this->position << 0.0f, 0.0f, 0.0f;
}

Object::Object(const char *filename, int filetype) {
    this->position << 0.0f, 0.0f, 0.0f;
    // TODO: use library
}

int Object::nvert() {
    return vertices.size();
}

int Object::nface() {
    return faces.size();
}

Eigen::Vector3f Object::get_face(int index) {
    return faces[index];
}

Eigen::Vector3f Object::get_vert(int index) {
    return vertices[index];
}

Eigen::Vector3f Object::get_position() {
    return position;
}
void Object::set_position(Eigen::Vector3f position) {
    this->position = position;
}

// get Model Matrix
Eigen::Matrix4f Object::get_model() {
    // TODO: rotate angle theta set to 0
    float rx = 0, ry = 0, rz = 0;
    // TODO: scale set to 1
    float sx = 1, sy = 1, sz = 1;

    // Translate
    Eigen::Matrix4f Mtranslate = Eigen::Matrix4f::Identity();
    for (int i = 0; i < 3; i++) {
        // x/w, y/w, z/w
        Mtranslate(i, 3) = position(i) / position(3);
    }

    // Scale
    Eigen::Matrix4f Mscale;
    Mscale << sx, 0, 0, 0,
              0, sy, 0, 0,
              0, 0, sz, 0,
              0, 0, 0, 1;

    Eigen::Matrix4f MrotateX;
    MrotateX << 1,          0,          0,          0,
                0,          cos(rx),    -sin(rx),   0,
                0,          sin(rx),    cos(rx),    0,
                0,          0,          0,          1;

    Eigen::Matrix4f MrotateY;
    MrotateY << cos(ry),    0,          sin(ry),    0,
                0,          1,          0,          0,
                -sin(ry),   0,          cos(ry),    0,
                0,          0,          0,          1; 

    Eigen::Matrix4f MrotateZ;
    MrotateZ << cos(rz),    -sin(rz),   0,          0,
                sin(rz),    cos(rz),    0,          0,
                0,          0,          1,          0,
                0,          0,          0,          1; 

    // Rotate
    Eigen::Matrix4f Mrotate = MrotateZ * MrotateY * MrotateX;

    return Mrotate*Mscale*Mtranslate;
}


// TODO: ~Object()
