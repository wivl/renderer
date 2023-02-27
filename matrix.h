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

Matrix m_transpose(Matrix *m);

Matrix m_inverse(Matrix *m);

/* trans */

Vec3f m_matrix_to_vec3f(Matrix m);

Matrix m_vec3f_to_matrix(Vec3f v);

/* getter & setter */

float m_get(Matrix *m, int row, int col);

void m_set(Matrix *m, int row, int col, float value);

#endif
