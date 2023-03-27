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
    Shader *shader;
    std::vector<Object> obj_list;

    // matrixes
    Eigen::Matrix4f view;
    Eigen::Matrix4f projection;

public:
    Camera();
    Camera(Eigen::Vector3f position, Shader &shader);
    Camera(Eigen::Vector3f position, Shader &shader, std::vector<Object> obj_list);

    void set_position(Eigen::Vector3f position);
    Eigen::Vector3f get_position();

    void set_shader(Shader *shader);

    void add_obj(Object obj);
    void del_obj(int index);


};

#endif // !__CAMERA_H__
