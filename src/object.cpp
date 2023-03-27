#include "object.hpp"
#include <Eigen/src/Core/Matrix.h>

Object::Object() {}

Object::Object(const char *filename, int filetype) {
    // TODO: use library
}

int Object::nvert() {
    return vertices.size();
}

int Object::nface() {
    return faces.size();
}

Eigen::Vector3f Object::get_face(int index) {
    // TODO: 
}

Eigen::Vector3f Object::get_vert(int index) {
    // TODO: 
}
// TODO: ~Object()
