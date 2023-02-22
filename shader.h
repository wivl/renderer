#ifndef _SHADER_H_
#define _SHADER_H_

#include "matrix.h"
#include "geometry.h"
#include "tinytga.h"
#include "tinyobj.h"

/* 
 * vertex shader: 告诉显卡点应该被画在什么位置，对每一个顶点
 * （这些顶点可能有很多属性（纹理，法线等等），也可能只有最基
 * 本的位置属性），假设有三个顶点，那么就应该被调用3次。
 *
 * fragment shader: 
 *
 * */

// world to camera
Matrix S_viewport(int x, int y, int w, int h, int depth);

// projection
Matrix S_projection(float coeff);

// model to world
Matrix S_modelview(Vec3f eye, Vec3f center, Vec3f up);

Matrix S_get_final_matrix(Matrix *v, Matrix *p, Matrix *m);

Vec4f S_vertex_shader(Vec4f v, Matrix m);

bool S_fragment_shader(Vec3f bar, tt_color *color);

void S_draw_triangle(tt_image *image, tobj_model *model, Vec3i *pts, Vec2i *uvs, int *zbuffer);



#endif
