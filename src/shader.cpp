#include "shader.hpp"
#include "object.hpp"
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
    
    Vector4f vert4(vertex.get_vert().x(), vertex.get_vert().y(), vertex.get_vert().z(), 1.0f);
    vert4 = uniform_m * vert4;

    if (vertex.hasn()) {
        varying_intensity(nthvert) = std::fmax(0, vertex.get_normal().dot(light.get_dir()));
    }

    if (vertex.hasu()) {
        // TODO: texture
        varying_uv.col(nthvert) = vertex.get_uv();
    } else {
        varying_uv.col(nthvert) << 0.0f, 0.0f;
    }


    // LOG
    // std::cout << "[LOG]Shader::vert: vert4: ";
    // for (int i = 0; i < 4; i++) {
        // std::cout << vert4(i) << " ";
    // }
    // std::cout << std::endl;


    return vert4;
}

bool Shader::fragment(Object &obj, Vector3f bar, ppm::Color &color) {
    // TODO: itensity
    color = ppm::Color(255, 255, 255);
    float intensity = varying_intensity.dot(bar);
    std::cout << "[LOG]Shader::fragment: intensity: " << intensity << std::endl;

    // FIX: if uv doesn't exits
    // if (!varying_uv.isZero()) {
    Vector2f uv = varying_uv * bar;
    std::cout << uv << std::endl;
    color = obj.get_texture(uv);
    std::cout << color.get_raw() << std::endl;
    // }
    color.intensity(intensity);
    return false;
}

