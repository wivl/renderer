#ifndef _SHADER_H_
#define _SHADER_H_

#include "matrix.h"
#include "geometry.h"
#include "tinytga.h"
#include "tinyobj.h"

typedef struct {
    Vec3f light_dir;
    Vec3f eye;
    Vec3f center;
    Vec3f up;

    Matrix uniform_M;   //  4x4 Projection*ModelView
    Matrix uniform_MIT; // 4x4 (Projection*ModelView).invert_transpose()

    Vec3f varying_intensity; // intensity of 3 vertexes
    Matrix varying_uv;  // a 2x3 matrix. 2 rows for u and v, 3 columns (one per vertex)
    Matrix modelview;   // model to world
    Matrix viewport;    // world to camera
    Matrix projection;  // projection
    Matrix final_matrix;// combination
} Shader;

/* 
 * vertex shader: 告诉显卡点应该被画在什么位置，对每一个顶点
 * （这些顶点可能有很多属性（纹理，法线等等），也可能只有最基
 * 本的位置属性），假设有三个顶点，那么就应该被调用3次。
 *
 * fragment shader: 对每一个需要渲染的像素运行fragment shader，
 * 设置像素的颜色，并最终渲染在屏幕上，每个pixel都需要调用一次
 *
 * */


Shader* S_make();


// world to camera
void S_set_viewport(Shader *shader, int x, int y, int w, int h);

// projection
void S_set_projection(Shader *shader, float coeff);

// model to world
void S_set_modelview(Shader *shader, Vec3f eye, Vec3f center, Vec3f up);

void S_get_final_matrix(Shader *shader);

Vec4f S_vertex_shader(Shader *shader, tobj_model *model, int iface, int nthvert);

Vec4f S_vertex_shader_texture(Shader *shader, tobj_model *model, int iface, int nthvert);

bool S_fragment_shader(Shader *shader, Vec3f bar, tt_color *color);

bool S_fragment_shader_texture(Shader *shader, tobj_model *model, Vec3f bar, tt_color *color);

void S_draw_triangle(Shader *shader, tt_image *image, tobj_model *model,
        Vec4f *pts, int *zbuffer);

void S_draw_triangle_texture(Shader *shader, tt_image *image, tobj_model *model,
        Vec4f *pts, int *zbuffer);

// TODO: diffuse map shaders
Vec4f S_vertex_shader_normalmapping(Shader *shader, tobj_model *model, int iface, int nthvert);

bool S_fragment_shader_normalmapping(Shader *shader, tobj_model *model, Vec3f bar, tt_color *color);

void S_draw_triangle_normalmapping(Shader *shader, tt_image *image, tobj_model *model,
        Vec4f *pts, int *zbuffer);



// TODO: specular mapping shaders



#endif
