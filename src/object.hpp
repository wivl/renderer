#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <Eigen/Dense>
#include <Eigen/src/Core/Matrix.h>
#include <vector>

using namespace Eigen;

enum FILE_TYPE {
    FT_OBJ = 0,
    FT_GLTF = 1,
};

class Object {
private:
    // indexes start from 0
    std::vector<Vector3f> vertices;
    std::vector<Vector3i> faces;
    Vector3f position;
    // TODO: rotate: up vector
public:
    Object();
    Object(const char *filename, int filetype);
    int nface();
    int nvert();
    Vector3f get_vert(int index);
    Vector3i get_face(int index);
    Vector3f get_position();
    void set_position(Vector3f position);
    Matrix4f get_model();
};

#endif // !__OBJECT_H__
