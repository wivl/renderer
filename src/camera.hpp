#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <Eigen/Dense>
#include <Eigen/src/Core/Matrix.h>
#include <vector>

#include "shader.hpp"
#include "object.hpp"
#include "light.hpp"

using namespace Eigen;

// TODO: s:
// * calculate view and projection while generating Camera object
//   and setting position, target or up vector.
// * 

class Camera {
private:
    Eigen::Vector3f position;
    Eigen::Vector3f target;
    Eigen::Vector3f up;

    float fovy;
    float aspect;
    float near;
    float far;

    int width;
    int height;

    Shader shader;

    // matrixes
    // model matrix is in Object class
    Eigen::Matrix4f view;
    Eigen::Matrix4f projection;
    Eigen::Matrix4f viewport;

    // TODO: define set model view projection
    // ModelView combines model and view matrixes

public:
    Camera();
    Camera(Eigen::Vector3f position, Eigen::Vector3f target, Eigen::Vector3f up, int width, int height, float fovy, float aspect, float near, float far);

    void set_position(Eigen::Vector3f position);
    void set_target(Eigen::Vector3f target);
    void set_up(Eigen::Vector3f up);

    Eigen::Vector3f get_position();
    Eigen::Vector3f get_target();
    Eigen::Vector3f get_up();

    void set_fov(float fovy);
    void set_far(float far);
    void set_near(float near);

    float get_fov();
    float get_far();
    float get_near();

    Eigen::Matrix4f get_modelview(Eigen::Matrix4f model);

    void set_shader(Shader &shader);

    // objects are stored in the list
    void render(std::vector<Object> obj_list, ppm::Image &image, std::vector<float> &zbuffer, Light light);



};

#endif // !__CAMERA_H__
