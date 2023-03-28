#include "camera.hpp"
#include <Eigen/src/Core/Matrix.h>


Camera::Camera() {
    position << 0.0f, 0.0f, 0.0f;
}

Camera::Camera(Eigen::Vector3f position, Shader &shader) {
    this->position = position;
    this->shader = shader;
}

void Camera::set_position(Eigen::Vector3f position) {
    this->position = position;
}

Eigen::Vector3f Camera::get_position() {
    return position;
}

void Camera::set_shader(Shader &shader) {
    this->shader = shader;
}

// camera represent's the world. objects are stored in the list
void Camera::render(std::vector<Object> obj_list, std::vector<Eigen::Vector3f> lights) {
    // TODO: 
}

void _get_modelview(Eigen::Matrix4f Mmodel) {
    
}
