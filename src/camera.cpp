#include "camera.hpp"
#include <Eigen/src/Core/Matrix.h>


Camera::Camera() {
    position << 0.0f, 0.0f, 0.0f;
    target << 0.0f, 0.0f, 0.0f;
    up << 0.0f, 1.0f, 0.0f;
}

Camera::Camera(Eigen::Vector3f position, Eigen::Vector3f target, Eigen::Vector3f up) {
    this->position = position;
    this->target = target;
    this->up = up;
    // calculate view and projection

    // VIEW
    Eigen::Vector3f zaxis = position - target; zaxis.normalize();
    Eigen::Vector3f xaxis = up.cross(zaxis); xaxis.normalize();
    Eigen::Vector3f yaxis = zaxis.cross(xaxis); yaxis.normalize();

    // M_t
    Eigen::Matrix4f Mtranslate = Eigen::Matrix4f::Identity();
    for (int i = 0; i < 3; i++) {
        Mtranslate(i, 3) = -position(i);
    }

    // M_r
    Eigen::Matrix4f Mrotate = Eigen::Matrix4f::Identity();
    for (int row = 0; row < 3; row++) {
        Mrotate(row, 0) = xaxis(row);
        Mrotate(row, 1) = yaxis(row);
        Mrotate(row, 2) = zaxis(row);
    }

    this->view = Mrotate * Mtranslate;


    // TODO: PROJECTION
    
}

void Camera::set_position(Eigen::Vector3f position) {
    this->position = position;
}

void Camera::set_target(Eigen::Vector3f target) {
    this->target = target;
}

void Camera::set_up(Eigen::Vector3f up) {
    this->up = up;
}


Eigen::Vector3f Camera::get_position() {
    return position;
}

Eigen::Vector3f Camera::get_target() {
    return target;
}

Eigen::Vector3f Camera::get_up() {
    return up;
}

void Camera::set_shader(Shader &shader) {
    this->shader = shader;
}

// camera represent's the world. objects are stored in the list
void Camera::render(std::vector<Object> obj_list, std::vector<Eigen::Vector3f> lights) {
    // TODO: 
}

Eigen::Matrix4f Camera::get_modelview(Eigen::Matrix4f Mmodel) {
    return view*Mmodel;
}
