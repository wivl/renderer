#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include <Eigen/Dense>
#include <Eigen/src/Core/Matrix.h>

using namespace Eigen;

class Light {
    // TODO: only parallel lights
private:
    Vector3f dir;
public:
    Light(Vector3f dir): dir(dir) {}
    Light(float dx, float dy, float dz);

    Vector3f get_dir();
    void set_dir(Vector3f dir);

};

#endif // !__LIGHT_HPP__
