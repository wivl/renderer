#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <Eigen/Dense>
#include <Eigen/src/Core/Matrix.h>
#include <_types/_uint8_t.h>
#include <cstdint>
#include <ppmpp/ppmpp.hpp>

#include <vector>

#include "light.hpp"

using namespace Eigen;

class Vertex {
    private:
        Vector3f vert;
        Vector3f normal;
        Vector2f uv;
        bool v;
        bool n;
        bool u;
    public:
        Vertex() { v = n = u = false; }

        Vertex(Vector3f vert, Vector3f normal, Vector2f uv) {
            this->vert = vert;
            this->normal = normal;
            this->uv = uv;
            v = n = u = true;
        }

        Vertex(Vector3f vert, Vector3f normal) {
            this->vert = vert;
            this->normal = normal;
            v = n = true;
            u = false;
        }

        Vertex(Vector3f vert, Vector2f uv) {
            this->vert = vert;
            this->uv = uv;
            v = u = true;
            n = false;
        }

        Vertex(Vector3f vert) {
            this->vert = vert;
            v = true;
            n = u = false;
        }

        Vector3f get_vert() { return vert; }
        Vector3f get_normal() { return normal; }
        Vector2f get_uv() { return uv; }
        void set_vert(Vector3f vert) { 
            this->vert = vert;
            v = true;
        }
        void set_normal(Vector3f normal) { 
            this->normal = normal;
            n = true;
        }
        void set_uv(Vector2f uv) { 
            this->uv = uv;
            u = true;
        }
        bool hasv() { return v; }
        bool hasn() { return n; }
        bool hasu() { return u; }
};

// Actually a "shader program" or "shader pipeline" since it's about software renderer
// camera class contains a shader
// include vertex shader and fragment shader
class Shader {
    // we have model projection
private:
    // TODO: define members
    Matrix4f uniform_m;
    Vector3f varying_intensity;


public:
    Shader();
    Shader(Matrix4f uni);

    // getters and setters
    void set_uniform(Matrix4f uniform_m);
    void set_uniform(Matrix4f model, Matrix4f view, Matrix4f matrix, Matrix4f viewport);
    Matrix4f get_uniform();

    // shaders
    // std::vector<VectorXf> vectors: vertices(3), normals(3), uvs(2)
    Vector4f vert(Vertex vertex, int nthvert, Light light);
    bool fragment(Vector3f bar, ppm::Color &color);


};

#endif // !__SHADER_HPP__
