#include "shader.hpp"

Shader::Shader() {}

Shader::Shader(Eigen::Matrix4f uni) {
    this->uniform_m = uni;
}

void Shader::set_uniform(Eigen::Matrix4f uniform_m) {
    this->uniform_m = uniform_m;
}

void Shader::set_uniform(Eigen::Matrix4f model, Eigen::Matrix4f view, Eigen::Matrix4f matrix, Eigen::Matrix4f viewport) {

}

Eigen::Matrix4f Shader::get_uniform() {
    return uniform_m;
}


Eigen::Vector4f Shader::vert(Eigen::Vector3f vertex, int nthvert) {
    Eigen::Vector4f vert4(vertex.x(), vertex.y(), vertex.z(), 1.0f);

    vert4 = uniform_m * vert4;
    // TODO: itensity
    

    return vert4;
}

bool Shader::fragment(Eigen::Vector3f bar, Eigen::Vector4f &color) {
    // TODO: itensity
    color << 1.0f, 1.0f, 1.0f, 1.0f;
    return true;
}

