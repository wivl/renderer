#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <Eigen/Dense>
#include <Eigen/src/Core/Matrix.h>

#include <vector>

#include "light.hpp"

using namespace Eigen;

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
    Vector4f vert(Vector3f vertex, int nthvert);
    bool fragment(Vector3f bar, png::rgba_pixel &color);


};

#endif // !__SHADER_HPP__
