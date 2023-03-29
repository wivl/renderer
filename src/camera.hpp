#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <Eigen/Dense>
#include <Eigen/src/Core/Matrix.h>
#include <vector>

#include "shader.hpp"
#include "object.hpp"

// TODO: s:
// * calculate view and projection while generating Camera object
//   and setting position, target or up vector.
// * 

class Camera {
private:
    Eigen::Vector3f position;
    Eigen::Vector3f target;
    Eigen::Vector3f up;

    Shader shader;

    // matrixes
    Eigen::Matrix4f view;
    Eigen::Matrix4f projection;

    // TODO: define set model view projection
    // ModelView combines model and view matrixes

public:
    Camera();
    Camera(Eigen::Vector3f position, Eigen::Vector3f target, Eigen::Vector3f up);

    void set_position(Eigen::Vector3f position);
    void set_target(Eigen::Vector3f target);
    void set_up(Eigen::Vector3f up);


    Eigen::Vector3f get_position();
    Eigen::Vector3f get_target();
    Eigen::Vector3f get_up();

    Eigen::Matrix4f get_modelview(Eigen::Matrix4f Mmodel);

    void set_shader(Shader &shader);

    // objects are stored in the list
    void render(std::vector<Object> obj_list, std::vector<Eigen::Vector3f> lights);



};

#endif // !__CAMERA_H__
