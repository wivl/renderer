#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <Eigen/Dense>
#include <Eigen/src/Core/Matrix.h>

#include <vector>

#include "light.hpp"

// Actually a "shader program" or "shader pipeline" since it's about software renderer
// camera class contains a shader
// include vertex shader and fragment shader
class Shader {
    // we have model projection
private:
    // TODO: define members
    Eigen::Matrix4f uniform_m;
    Eigen::Vector3f varying_intensity;


public:
    Shader();
    Shader(Eigen::Matrix4f uni);

    // getters and setters
    void set_uniform(Eigen::Matrix4f uniform_m);
    void set_uniform(Eigen::Matrix4f model, Eigen::Matrix4f view, Eigen::Matrix4f matrix, Eigen::Matrix4f viewport);
    Eigen::Matrix4f get_uniform();




    // shaders
    Eigen::Vector4f vert(Eigen::Vector3f vertex, int nthvert);
    bool fragment(Eigen::Vector3f bar, Eigen::Vector4f &color);


};

#endif // !__SHADER_HPP__
