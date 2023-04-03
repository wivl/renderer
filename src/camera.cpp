#include "camera.hpp"
#include <Eigen/src/Core/Matrix.h>

#include <cmath>

#define cot(x) (1.0/std::tan(x))

Eigen::Matrix4f _cal_view(Eigen::Vector3f position, Eigen::Vector3f target, Eigen::Vector3f up) {
    // VIEW MATRIX
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
    return Mrotate * Mtranslate;
    
}

Eigen::Matrix4f _cal_proj(float fovy, float aspect, float near, float far) {
    Eigen::Matrix4f temp;
    temp << cot(fovy/2)/aspect, 0,              0,                      0,
            0,                  cot(fovy/2),    0,                      0,
            0,                  0,              (near+far)/(near-far),  -(2*near*far)/(near-far),
            0,                  0,              1,                      0;
    return temp;
}

Eigen::Matrix4f _cal_viewport(int width, int height) {
    Eigen::Matrix4f temp;
    temp << (float)width/2,     0,                  0, (float)width/2,
            0,                  (float)height/2,    0, (float)height/2,
            0,                  0,                  1, 0,
            0,                  0,                  0, 1;    
    return temp;
}

Camera::Camera() {
    position << 0.0f, 0.0f, 0.0f;
    target << 0.0f, 0.0f, 0.0f;
    up << 0.0f, 1.0f, 0.0f;
}



Camera::Camera(Eigen::Vector3f position, Eigen::Vector3f target, Eigen::Vector3f up, int width, int height, float fovy, float aspect, float near, float far) {
    this->position = position;
    this->target = target;
    this->up = up;

    this->fovy = fovy;
    this->aspect = aspect;
    this->near = near;
    this->far = far;

    this->width = width;
    this->height = height;
    // calculate view and projection

    // VIEW MATRIX
    this->view = _cal_view(position, target, up);

    // PROJECTION
    // fov,aspect,far,near
    this->projection = _cal_proj(fovy, aspect, near, far);

    // VIEWPORT
    this->viewport = _cal_viewport(width, height);

    // TODO: set shader
   
}

void Camera::set_position(Eigen::Vector3f position) {
    this->position = position;
    this->view = _cal_view(position, target, up);
}

void Camera::set_target(Eigen::Vector3f target) {
    this->target = target;
    this->view = _cal_view(position, target, up);
}

void Camera::set_up(Eigen::Vector3f up) {
    this->up = up;
    this->view = _cal_view(position, target, up);
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

void Camera::set_fov(float fovy) {
    this->fovy = fovy;
    this->projection = _cal_proj(fovy, aspect, near, far);
}
void Camera::set_far(float far) {
    this->far = far;
    this->projection = _cal_proj(fovy, aspect, near, far);
}
void Camera::set_near(float near) {
    this->near = near;
    this->projection = _cal_proj(fovy, aspect, near, far);
}

float Camera::get_fov() {
    return fovy;
}
float Camera::get_far() {
    return far;
}
float Camera::get_near() {
    return near;
}


void Camera::set_shader(Shader &shader) {
    this->shader = shader;
}

// camera represent's the world. objects are stored in the list
void Camera::render(std::vector<Object> obj_list, Light light) {
    // TODO: 
}

Eigen::Matrix4f Camera::get_modelview(Eigen::Matrix4f model) {
    return view*model;
}
