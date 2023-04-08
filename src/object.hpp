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
    std::vector<std::vector<Vector3f>> faces;
    std::vector<std::vector<Vector3f>> normals;
    std::vector<std::vector<Vector2f>> uvs;
    Vector3f position;
    // TODO: rotate: up vector (or angle: float)
public:
    Object();
    Object(std::vector<float> vertices, std::vector<int> indexes, Vector3f position);
    Object(const char *filename, int filetype);

    int nface();
    std::vector<Vector3f> get_face(int index);
    std::vector<Vector3f> get_normal(int index);
    std::vector<Vector2f> get_uv(int index);

    Vector3f get_position();
    void set_position(Vector3f position);

    // get model matrix
    Matrix4f get_model();
};

#endif // !__OBJECT_H__
