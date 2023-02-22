#include "shader.h"
#include "draw.h"
#include "geometry.h"
#include "matrix.h"
#include "tinytga.h"

// world to camera
Matrix S_viewport(int x, int y, int w, int h, int depth) {
    Matrix m = m_identity(4);
    m.data[0*m.cols+3] = x+w/2.f;
    m.data[1*m.cols+3] = y+h/2.f;
    m.data[2*m.cols+3] = depth/2.f;

    m.data[0*m.cols+0] = w/2.f;
    m.data[1*m.cols+1] = h/2.f;
    m.data[2*m.cols+2] = depth/2.f;
    return m;
}

// projection
Matrix S_projection(float coeff) {
    Matrix projection = m_identity(4);
    projection.data[3*projection.cols+2] = coeff;
    return projection;
}

// model to world
Matrix S_modelview(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f z = vec3f_minus(eye, center);
    vec3f_normalize(&z, 1);
    Vec3f x = vec3f_cross(up, z);
    vec3f_normalize(&x, 1);
    Vec3f y = vec3f_cross(z, x);
    vec3f_normalize(&y, 1);
    Matrix modelview = m_identity(4);

    modelview.data[0*modelview.cols+0] = x.x;
    modelview.data[1*modelview.cols+0] = y.x;
    modelview.data[2*modelview.cols+0] = z.x;
    modelview.data[0*modelview.cols+3] = -center.x;

    modelview.data[0*modelview.cols+1] = x.y;
    modelview.data[1*modelview.cols+1] = y.y;
    modelview.data[2*modelview.cols+1] = z.y;
    modelview.data[1*modelview.cols+3] = -center.y;

    modelview.data[0*modelview.cols+2] = x.z;
    modelview.data[1*modelview.cols+2] = y.z;
    modelview.data[2*modelview.cols+2] = z.z;
    modelview.data[2*modelview.cols+3] = -center.z;

    return modelview;
}

Matrix S_get_final_matrix(Matrix *v, Matrix *p, Matrix *m) {
    Matrix final = m_multiply(m_multiply(*v, *p), *m);
    return final;
}

Vec4f S_vertex_shader(Vec4f v, Matrix m);

bool S_fragment_shader(Vec3f bar, tt_color *color);

void S_draw_triangle(tt_image *image, tobj_model *model, Vec3i *pts, Vec2i *uvs, int *zbuffer);


