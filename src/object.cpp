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




// TODO: ~Object()
