#include "light.hpp"
#include <Eigen/src/Core/Matrix.h>

Light::Light(float dx, float dy, float dz) {
    dir << dx, dy, dz;
}

Vector3f Light::get_dir() {
    return dir;
}

void Light::set_dir(Vector3f dir) {
    this->dir = dir;
}
