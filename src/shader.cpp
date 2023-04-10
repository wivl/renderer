#include "shader.hpp"
#include "ppmpp/ppmpp.hpp"

#include <algorithm>

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


// std::vector<VectorXf> vectors: vertices(3), normals(3), uvs(2)
Vector4f Shader::vert(Vertex vertex, int nthvert, Light light) {
    // TODO: handle vertex class
    // - only vert
    // - vert + normal
    // - vert + uv
    // - vert + normal + uv
    
    Vector4f vert4(vertex.get_vert().x(), vertex.get_vert().y(), vertex.get_vert().z(), 1.0f);
    vert4 = uniform_m * vert4;

    if (vertex.hasn()) {
        varying_intensity(nthvert) = std::fmax(0, vertex.get_normal().dot(light.get_dir()));
        std::cout << "[LOG]Shader::vert: varying intensity: " 
            << varying_intensity(0) << " "
            << varying_intensity(1) << " " 
            << varying_intensity(2) << std::endl; 
    }

    if (vertex.hasu()) {
        // TODO: handle uv
    }


    // LOG
    std::cout << "[LOG]Shader::vert: vert4: ";
    for (int i = 0; i < 4; i++) {
        std::cout << vert4(i) << " ";
    }
    std::cout << std::endl;

    return vert4;
}

bool Shader::fragment(Vector3f bar, ppm::Color &color) {
    // TODO: itensity
    color = ppm::Color(255, 255, 255);
    float intensity = varying_intensity.dot(bar);
    std::cout << "[LOG]Shader::fragment: intensity: " << intensity << std::endl;
    color.intensity(intensity);
    return false;
}

