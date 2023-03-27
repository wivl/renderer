#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <Eigen/Dense>
#include <Eigen/src/Core/Matrix.h>

#include <vector>

// Actually a "shader program" or "shader pipeline" since it's about software renderer
// camera class contains a camera pointer
// include vertex shader and fragment shader
class Shader {
    // we have model view projection
    //
private:
    // same uniform matrix in the same frame
    Eigen::Matrix4f uniform_m;


public:
    Shader();
    void set_uniform(Eigen::Matrix4f uniform_m);
    Eigen::Matrix4f get_uniform();
    Eigen::Vector4f vert();
    bool fragment();


};

#endif // !__SHADER_HPP__
