#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <Eigen/Dense>
#include <Eigen/src/Core/Matrix.h>
#include <vector>

#include "shader.hpp"
#include "object.hpp"

class Camera {
private:
    Eigen::Vector3f position;
    Shader shader;

    // matrixes
    Eigen::Matrix4f view;
    Eigen::Matrix4f projection;

    // TODO: define set model view projection
    // ModelView combines model and view matrixes
    void _get_modelview(); // used in the Camera::render() function

public:
    Camera();
    Camera(Eigen::Vector3f position, Shader &shader);

    void set_position(Eigen::Vector3f position);
    Eigen::Vector3f get_position();

    void set_shader(Shader &shader);

    // camera represent's the world. objects are stored in the list
    void render(std::vector<Object> obj_list, std::vector<Eigen::Vector3f> lights);



};

#endif // !__CAMERA_H__
