#ifndef _SHADER_H_
#define _SHADER_H_

#include "matrix.h"
#include "geometry.h"
#include "tinytga.h"
#include "tinyobj.h"

typedef struct {
    Vec3f light_dir;    // light source position
    Vec3f eye;          // camera position
    Vec3f center;       // TODO: not understand yet
    Vec3f up;           // camera up direction

    Matrix uniform_M;           // 4x4 Projection*ModelView
    Matrix uniform_MIT;         // 4x4 (Projection*ModelView).invert_transpose()
    Matrix uniform_Mshadow;     // 4x4 matrix, transform framebuffer screen coordinates to shadowbuffer screen coordinates

    tt_image *aoimage;  // ambient occlusion texture

    Vec3f varying_intensity;    // intensity of 3 vertices
    Matrix varying_uv;          // a 2x3 matrix. 2 rows for u and v, 3 columns (one per vertex)
    Matrix varying_tri;         // 3x3, for depth shader
    Matrix varying_tri4;        // 4x3, for ao shader
    // Matrix final_matrix;        // combination
} Shader;

#define DEPTH 2000

// TODO: understand the transformation
Matrix ModelView;   // model to world
Matrix Viewport;    // world to camera
Matrix Projection;  // Projection

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
void S_set_viewport(int x, int y, int w, int h);

// projection
void S_set_projection(float coeff);

// model to world
void S_set_modelview(Vec3f eye, Vec3f center, Vec3f up);

void S_init_shader(Shader *shader);


// No texture
Vec4f S_vertex_shader(Shader *shader, tobj_model *model, int iface, int nthvert);
bool S_fragment_shader(Shader *shader, Vec3f bar, tt_color *color);
void S_draw_triangle(Shader *shader, tt_image *image, tobj_model *model,
        Vec4f *pts, int *zbuffer);


// texture
Vec4f S_vertex_shader_texture(Shader *shader, tobj_model *model, int iface, int nthvert);
bool S_fragment_shader_texture(Shader *shader, tobj_model *model, Vec3f bar, tt_color *color);
void S_draw_triangle_texture(Shader *shader, tt_image *image, tobj_model *model,
        Vec4f *pts, int *zbuffer);



// texture + normalmapping
Vec4f S_vertex_shader_normalmapping(Shader *shader, tobj_model *model, int iface, int nthvert);
bool S_fragment_shader_normalmapping(Shader *shader, tobj_model *model, Vec3f bar, tt_color *color);
void S_draw_triangle_normalmapping(Shader *shader, tt_image *image, tobj_model *model,
        Vec4f *pts, int *zbuffer);



// texture + normalmapping + specularmapping
Vec4f S_vertex_shader_specular(Shader *shader, tobj_model *model, int iface, int nthvert);
bool S_fragment_shader_specular(Shader *shader, tobj_model *model, Vec3f bar, tt_color *color);
void S_draw_triangle_specular(Shader *shader, tt_image *image, tobj_model *model,
        Vec4f *pts, int *zbuffer);



// depth shader for shadow
Vec4f S_vertex_shader_depth(Shader *shader, tobj_model *model, int iface, int nthvert);
bool S_fragment_shader_depth(Shader *shader, tobj_model *model, Vec3f bar, tt_color *color);
void S_draw_triangle_depth(Shader *shader, tt_image *image, tobj_model *model,
        Vec4f *pts, float *zbuffer);



// shadow shader
Vec4f S_vertex_shader_shadow(Shader *shader, tobj_model *model, int iface, int nthvert);
bool S_fragment_shader_shadow(Shader *shader, tt_image *image, tobj_model *model, float *shadowbuffer, Vec3f bar, tt_color *color);
void S_draw_triangle_shadow(Shader *shader, tt_image *image, tobj_model *model,
        Vec4f *pts, float *zbuffer, float *shadowbuffer);



// TODO: AO SHADER
Vec4f S_vertex_shader_AO(Shader *shader, tobj_model *model, int iface, int nthvert);
bool S_fragment_shader_AO(Shader *shader, Vec3f fragcoord, Vec3f bar, tt_color *color);
void S_draw_triangle_AO(Shader *shader, tt_image *image, tobj_model *model,
        Vec4f *pts, float *zbuffer);


#endif
