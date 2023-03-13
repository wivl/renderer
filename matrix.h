#ifndef MATRIX_H_
#define MATRIX_H_

#include "geometry.h"

typedef struct {
    float *data;
    int rows, cols;
} Matrix ;

Matrix m_make(int rows, int cols);

Matrix m_make_from_data(int rows, int cols, float *data);

/* calculate */

Matrix m_identity(int dimensions);

Matrix m_multiply(Matrix a, Matrix b);

Vec4f m_multiply_vec4f(Matrix m, Vec4f v);

Vec4i m_multiply_vec4i(Matrix m, Vec4i v);

Vec2f m_multiply_vec3f_2(Matrix m, Vec3f v);

Vec3f m_multiply_vec3f_3(Matrix m, Vec3f v);

Matrix m_transpose(Matrix *m);

Matrix m_inverse(Matrix *m);

Matrix m_get_minor(Matrix m, int row, int cols);

float m_cofactor(Matrix m, int row, int cols);

Matrix m_asjugate(Matrix m);

Matrix m_invert_transpose(Matrix m);

/* trans */

Vec3f m_matrix_to_vec3f(Matrix m);

Matrix m_vec3f_to_matrix(Vec3f v);

/* getter & setter */

float m_get(Matrix *m, int row, int col);

void m_set(Matrix *m, int row, int col, float value);

void m_set_col2(Matrix *m, int idx, Vec2f v);
void m_set_col3(Matrix *m, int idx, Vec3f v);
void m_set_col4(Matrix *m, int idx, Vec4f v);

#endif
