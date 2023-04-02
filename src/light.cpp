#include "light.hpp"
#include <Eigen/src/Core/Matrix.h>

Light::Light(float dx, float dy, float dz) {
    dir << dx, dy, dz;
}

Light::Light(Eigen::Vector3f dir) {
    this->dir = dir;
}

Eigen::Vector3f Light::get_dir() {
    return dir;
}

Eigen::Vector3f Light::set_dir(Eigen::Vector3f dir) {
    this->dir = dir;
}
