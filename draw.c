#include "draw.h"
#include "geometry.h"
#include "tinytga.h"
#include "tinyobj.h"
#include "matrix.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <float.h>


void _check_border(tt_image* image, uint16_t w1, uint16_t h1,
        uint16_t w2, uint16_t h2) {
	assert(w1 <= image->width && h1 <= image->height);
	assert(w2 <= image->width && h2 <= image->height);
}

void _swap_u16(uint16_t *a, uint16_t *b) {
	uint16_t temp = *b;
	*b = *a;
	*a = temp;
}
void _swap_int(int *a, int *b) {
	int temp = *b;
	*b = *a;
	*a = temp;
}

void _vec3i_swap(Vec3i *a, Vec3i *b) {
    Vec3i temp = *b;
    *b = *a;
	*a = temp;
}

void _vec2i_swap(Vec2i *a, Vec2i *b) {
    Vec2i temp = *b;
    *b = *a;
	*a = temp;
}

void draw_line(tt_image* image, int w1, int h1, int w2, int h2,
        tt_color color) {
	_check_border(image, w1, h1, w2, h2);

	bool steep = false;

    // if the line is steep, transpose the image.
	if (abs(w1-w2)<abs(h1-h2)) {
        _swap_int(&w1, &h1); 
		_swap_int(&w2, &h2); 
		steep = true; 
	}

    // if w1 is on the left, swap.
	if (w1 > w2) {
		_swap_int(&w1, &w2);
		_swap_int(&h1, &h2);
	}

	int dw = w2 - w1; 
	int dh = h2 - h1;
	if (w1 == w2) {
		if (h1 > h2) {
			_swap_int(&h1, &h2);
			_swap_int(&w1, &w2);
		}
		for (int h = h1; h <= h2; h++) {
			tt_set_color(image, w1, h, color);
		}
	}
	float t = (float)dh / (float)dw;

	for (int w = w1; w <= w2; w++) {
		int h = h1 + t * (w-w1);
		if (steep) {
			tt_set_color(image, h-1, w-1, color);
		} else {
			tt_set_color(image, w-1, h-1, color);
		}
	}
}




/*
 * pts: points [A, B, C]
 * P  : barycentric coordinate = (1-u-v) u v
 *
 * */
Vec3f _barycentric(Vec3f *pts, Vec3f P) { 
	assert(pts != NULL);
	// (AB.x AC.x PA.x)
	Vec3f vec_x = {
		.x = pts[2].x - pts[0].x,
		.y = pts[1].x - pts[0].x,
		.z = pts[0].x - P.x
	};

	// (AB.y AC.y PA.y)
	Vec3f vec_y = {
		.x = pts[2].y - pts[0].y,
		.y = pts[1].y - pts[0].y,
		.z = pts[0].y - P.y 
	};

	// (u v 1)
	Vec3f u = vec3f_cross(vec_x, vec_y); 

	if (fabs(u.z) > 1e-2) {
		Vec3f result = {
			.x = 1.f-(u.x+u.y)/u.z,
			.y = u.y/u.z,
			.z = u.x/u.z
		};
		return result;
		 
	} 
	Vec3f result = {
		.x = -1,
		.y = 1,
		.z = 1
	};
	return result;
}


void draw_triangle(tt_image *image, Vec3f *pts, float *zbuffer,
        tt_color color) {
	// bounding box ----------> x
	//              |  min
	//              |
	//              |           max
	//              V y

	Vec2i bboxmin = { .x = image->width-1, .y = image->height-1 };
	Vec2i bboxmax = { .x = 0, .y = 0 };
    // canvas
	Vec2i clamp = { .x = image->width-1, .y = image->height-1 };

    // calculate bounding boxes
	for (int i = 0; i < 3; i++) {
		bboxmin.x = (int)fmaxf(0, fminf((float)bboxmin.x, (float)pts[i].x));
		bboxmin.y = (int)fmaxf(0, fminf((float)bboxmin.y, (float)pts[i].y));

		bboxmax.x = (int)fminf((float)clamp.x, fmaxf((float)bboxmax.x, (float)pts[i].x));
		bboxmax.y = (int)fminf((float)clamp.y, fmaxf((float)bboxmax.y, (float)pts[i].y));
	}

	Vec3f P;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
            // get barycentric
			Vec3f bc_screen = _barycentric(pts, P);

            // if the point is not in the triangle, skip.
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
                continue;

            // calculate zbuffer value
			P.z = 0;
			P.z += pts[0].z * bc_screen.x;
			P.z += pts[1].z * bc_screen.y;
			P.z += pts[2].z * bc_screen.z;

            // if the pixel is on top, draw and update zbuffer.
			if (zbuffer[(int)((P.x-1)+(P.y-1)*image->width)] < P.z) {
				zbuffer[(int)((P.x-1)+(P.y-1)*image->width)] = P.z;
				tt_set_color(image, P.x-1, P.y-1, color);
			}
		}

	}
}

Matrix viewpoint(int x, int y, int w, int h, int depth) {
    Matrix m = m_identity(4);
    m.data[0*m.cols+3] = x+w/2.f;
    m.data[1*m.cols+3] = y+h/2.f;
    m.data[2*m.cols+3] = depth/2.f;

    m.data[0*m.cols+0] = w/2.f;
    m.data[1*m.cols+1] = h/2.f;
    m.data[2*m.cols+2] = depth/2.f;
    return m;
}


void draw_triangle_texture(Vec3i t0, Vec3i t1, Vec3i t2,
        Vec2i uv0, Vec2i uv1, Vec2i uv2, tt_image *image,
        float intensity, int *zbuffer, tobj_model *model) {
    if (t0.y==t1.y && t0.y==t2.y) return; // i dont care about degenerate triangles
    if (t0.y>t1.y) { _vec3i_swap(&t0, &t1); _vec2i_swap(&uv0, &uv1); }
    if (t0.y>t2.y) { _vec3i_swap(&t0, &t2); _vec2i_swap(&uv0, &uv2); }
    if (t1.y>t2.y) { _vec3i_swap(&t1, &t2); _vec2i_swap(&uv1, &uv2); }

    int total_height = t2.y-t0.y;
    // 从三角形的最低点到最高点
    for (int i=0; i<total_height; i++) {
        bool second_half = i>t1.y-t0.y || t1.y==t0.y;
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; // be careful: with above conditions no division by zero here

        // Vec3i A   =               t0  + Vec3f(t2-t0  )*alpha;
        // Vec3i B   = second_half ? t1  + Vec3f(t2-t1  )*beta : t0  + Vec3f(t1-t0  )*beta;
        // Vec2i uvA =               uv0 +      (uv2-uv0)*alpha;
        // Vec2i uvB = second_half ? uv1 +      (uv2-uv1)*beta : uv0 +      (uv1-uv0)*beta;

        Vec3i A = vec3i_add(t0, vec3f_to_i(vec3f_multiply_f(vec3i_to_f(vec3i_minus(t2, t0)), alpha)));
        Vec3i B = second_half ? vec3i_add(t1, vec3f_to_i(vec3f_multiply_f(vec3i_to_f(vec3i_minus(t2, t1)), beta))) : 
            vec3i_add(t0, vec3f_to_i(vec3f_multiply_f(vec3i_to_f(vec3i_minus(t1, t0)), beta)));
        Vec2i uvA = vec2i_add(uv0, vec2i_multiply(vec2i_minus(uv2, uv0), alpha));
        Vec2i uvB = second_half ? vec2i_add(uv1, vec2i_multiply(vec2i_minus(uv2, uv1), beta)) : 
            vec2i_add(uv0, vec2i_multiply(vec2i_minus(uv1, uv0), beta));
        
        if (A.x>B.x) { _vec3i_swap(&A, &B); _vec2i_swap(&uvA, &uvB); }
        for (int j=A.x; j<=B.x; j++) {
            float phi = B.x==A.x ? 1. : (float)(j-A.x)/(float)(B.x-A.x);
            // Vec3i P = Vec3f(A) + Vec3f(B-A)*phi;
            Vec3i P = vec3f_to_i(vec3f_add(vec3i_to_f(A), vec3f_multiply_f(vec3i_to_f(vec3i_minus(B, A)), phi)));
            // Vec2i uvP =     uvA +   (uvB-uvA)*phi;
            Vec2i uvP = vec2i_add(uvA, vec2i_multiply(vec2i_minus(uvB, uvA), phi));
            int idx = P.x+P.y*image->width;
            if (zbuffer[idx]<P.z) {
                zbuffer[idx] = P.z;
                tt_color color = tobj_diffuse(model, uvP);
                color.r *= intensity;
                color.g *= intensity;
                color.b *= intensity;
                tt_set_color(image, P.x, P.y, color);
            }
        }
    }
}



Vec3f world2screen(Vec3f v, const int width, const int height) {
	Vec3f result = {
		.x = (int)((v.x+1.)*width/2.+.5),
		.y = (int)((v.y+1.)*height/2.+.5),
		.z = v.z
	};
    return result;
}
