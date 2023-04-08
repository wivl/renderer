#include "shader.hpp"
#include <png++/rgba_pixel.hpp>

Shader::Shader() {}

Shader::Shader(Matrix4f uni) {
    this->uniform_m = uni;
}

void Shader::set_uniform(Matrix4f uniform_m) {
    this->uniform_m = uniform_m;
}

// TODO: set by 4 matrixes
void Shader::set_uniform(Matrix4f model, Matrix4f view, Matrix4f projection, Matrix4f viewport) {

}

Matrix4f Shader::get_uniform() {
    return uniform_m;
}


Vector4f Shader::vert(Vector3f vertex, int nthvert) {
    Vector4f vert4(vertex.x(), vertex.y(), vertex.z(), 1.0f);

    // std::cout << "[LOG]Shader::vert: uniform_m: ";
    // for (int i = 0; i < 4; i++) {
        // for (int j = 0; j < 4; j++) {
            // std::cout << uniform_m(i, j) << " ";
        // }
        // std::cout << std::endl;
    // }
    // std::cout << "[LOG]Shader::vert: vert4: ";
    // for (int i = 0; i < 4; i++) {
        // std::cout << vert4(i) << " ";
    // }
    // std::cout << std::endl;

    vert4 = uniform_m * vert4;
    std::cout << "[LOG]Shader::vert: vert4: ";
    for (int i = 0; i < 4; i++) {
        std::cout << vert4(i) << " ";
    }
    std::cout << std::endl;
    // TODO: itensity
    return vert4;
}

bool Shader::fragment(Vector3f bar, png::rgba_pixel &color) {
    // TODO: itensity
    color = png::rgba_pixel(255, 255, 255, 255);
    return false;
}

