#include "matrix.h"
#include "geometry.h"
#include <stdlib.h>
#include <assert.h>

Matrix m_make(int rows, int cols) {
    Matrix m = {
        .rows = rows,
        .cols = cols,
        .data = (float*)malloc(sizeof(float)*rows*cols)
    };
    return m;
}

Matrix m_make_from_data(int rows, int cols, float *data) {
    Matrix m = {
        .rows = rows,
        .cols = cols,
        .data = data
    };
    return m;

}

Matrix m_identity(int dimensions) {
    Matrix E = m_make(dimensions, dimensions);
    for (int i = 0; i < dimensions; i++) {
        for (int j = 0; j < dimensions; j++) {
            E.data[i*dimensions+j] = (i == j ? 1. : 0.);
        }
    }
    return E;
}

Matrix m_multiply(Matrix a, Matrix b) {
    assert(a.cols == b.rows);
    Matrix result = m_make(a.rows, b.cols);
    for (int i = 0; i < a.rows; i++) {      // result: rows
        for (int j = 0; j < b.cols; j++) {  // result: cols
            result.data[i*b.cols+j] = 0.f;
            for (int k = 0; k < a.cols; k++) {
                result.data[i*b.cols+j] += a.data[i*a.cols+k] * b.data[k*b.cols+j];
            }
        }
    }
    return result;
}

Vec4f m_multiply_vec4f(Matrix m, Vec4f v) {
    assert(m.rows == 4);
    Vec4f ans;
    for (int i = 0; i < 4; i++) {
        float temp = 0;
        for (int j = 0; j < 4; j++) {
            temp += m_get(&m, i, j)*vec4f_get(&v, j);
        }
        vec4f_set(&ans, i, temp);
    }
    return ans;
}

Vec4i m_multiply_vec4i(Matrix m, Vec4i v) {
    assert(m.rows == 4);
    Vec4i ans;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            vec4i_set(&ans, i, m_get(&m, i, j)*vec4i_get(&v, j));
        }
    }
    return ans;
}

Vec2f m_multiply_vec3f_2(Matrix m, Vec3f v) {
    assert(m.rows == 2);
    Vec2f ans;
    for (int i = 0; i < 2; i++) {
        float temp = 0;
        for (int j = 0; j < 3; j++) {
            temp += m_get(&m, i, j)*vec3f_get(&v, j);
        }
        vec2f_set(&ans, i, temp);
    }
    return ans;
}

Vec3f m_multiply_vec3f_3(Matrix m, Vec3f v) {
    assert(m.rows == 3);
    Vec3f ans;
    for (int i = 0; i < 3; i++) {
        float temp = 0;
        for (int j = 0; j < 3; j++) {
            temp += m_get(&m, i, j)*vec3f_get(&v, j);
        }
        vec3f_set(&ans, i, temp);
    }
    return ans;
}

Matrix m_transpose(Matrix *m) {
    Matrix result = m_make(m->cols, m->rows);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            result.data[i*result.cols+j] = m->data[j*m->cols+i];
        }
    }
    return result;
}

Matrix m_inverse(Matrix *m) {   // A^(-1)
    assert(m->rows == m->cols);
    // augmenting the square matrix with the identity matrix of the same dimensions a => [ai]
    Matrix result = m_make(m->rows, m->cols*2);
    for(int i=0; i<m->rows; i++)
        for(int j=0; j<m->cols; j++)
            result.data[i*result.cols+j] = m->data[i*m->cols+j];
    for(int i=0; i<m->rows; i++)
        result.data[i*result.cols+i+m->cols] = 1;
    // first pass
    for (int i=0; i<m->rows-1; i++) {
        // normalize the first row
        for(int j=result.cols-1; j>=0; j--)
            result.data[i*result.cols+j] /= result.data[i*result.cols+i];
        for (int k=i+1; k<m->rows; k++) {
            float coeff = result.data[k*result.cols+i];
            for (int j=0; j<result.cols; j++) {
                result.data[k*result.cols+j] -= result.data[i*result.cols+j]*coeff;
            }
        }
    }
    // normalize the last row
    for(int j=result.cols-1; j>=m->rows-1; j--)
        result.data[(m->rows-1)*result.cols+j] /= result.data[(result.rows-1)*result.cols+result.rows-1];
    // second pass
    for (int i=m->rows-1; i>0; i--) {
        for (int k=i-1; k>=0; k--) {
            float coeff = result.data[k*result.cols+i];
            for (int j=0; j<result.cols; j++) {
                result.data[k*result.cols+j] -= result.data[i*result.cols+j]*coeff;
            }
        }
    }
    // cut the identity matrix back
    Matrix truncate = m_make(m->rows, m->cols);
    for(int i=0; i<m->rows; i++)
        for(int j=0; j<m->cols; j++)
            truncate.data[i*truncate.cols+j] = result.data[i*result.cols+j+m->cols];
    return truncate;
}
Matrix m_invert_transpose(Matrix m) {
    Matrix tmp = m_inverse(&m);
    return m_transpose(&tmp);
}


Vec3f m_matrix_to_vec3f(Matrix m) {
    return vec3f_make(
            m.data[0*m.cols+0]/m.data[3*m.cols+0],
            m.data[1*m.cols+0]/m.data[3*m.cols+0], 
            m.data[2*m.cols+0]/m.data[3*m.cols+0]
        );
}

Matrix m_vec3f_to_matrix(Vec3f v) {
    Matrix m = m_make(4, 1);
    m.data[0*m.cols+0] = v.x;
    m.data[1*m.cols+0] = v.y;
    m.data[2*m.cols+0] = v.z;
    m.data[3*m.cols+0] = 1.f;
    return m;
}


float m_get(Matrix *m, int row, int col) {
    return m->data[row*m->cols+col];
}

void m_set(Matrix *m, int row, int col, float value) {
    if (m == NULL || m->data == NULL) {
        return ;
    }
    m->data[row*m->cols+col] = value;
}


void m_set_col2(Matrix *m, int idx, Vec2f v) {
    assert(m->cols > idx);
    for (size_t i=m->rows; i--; m_set(m, i, idx, vec2f_get(&v, i)));
}

void m_set_col3(Matrix *m, int idx, Vec3f v) {
    assert(m->cols > idx);
    for (size_t i=m->rows; i--; m_set(m, i, idx, vec3f_get(&v, i)));
}


