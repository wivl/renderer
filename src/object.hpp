#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <Eigen/Dense>
#include <Eigen/src/Core/Matrix.h>
#include <vector>

enum FILE_TYPE {
    FT_OBJ = 0,
    FT_GLTF = 1,
};

class Object {
private:
    std::vector<Eigen::Vector3f> vertices;
    std::vector<Eigen::Vector3i> faces;
public:
    Object();
    Object(const char *filename, int filetype);
    int nface();
    int nvert();
    Eigen::Vector3f get_vert(int index);
    Eigen::Vector3f get_face(int index);
};

#endif // !__OBJECT_H__
