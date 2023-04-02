#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include <Eigen/Dense>
#include <Eigen/src/Core/Matrix.h>

class Light {
    // TODO: only parallel lights
private:
    Eigen::Vector3f dir;
public:
    Light(Eigen::Vector3f dir);
    Light(float dx, float dy, float dz);

    Eigen::Vector3f get_dir();
    Eigen::Vector3f set_dir(Eigen::Vector3f dir);

};

#endif // !__LIGHT_HPP__
