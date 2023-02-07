#include "draw.h"
#include "geometry.h"
#include "tinytga.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

void _check_border(tt_image* image, uint16_t w1, uint16_t h1, uint16_t w2, uint16_t h2) {
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

void draw_line(tt_image* image, int w1, int h1, int w2, int h2, tt_color color) {
	_check_border(image, w1, h1, w2, h2);
	bool steep = false;
	if (abs(w1-w2)<abs(h1-h2)) { // if the line is steep, we transpose the image 
		_swap_int(&w1, &h1); 
		_swap_int(&w2, &h2); 
		steep = true; 
	}
	if (w1 > w2) {
		_swap_int(&w1, &w2);
		_swap_int(&h1, &h2);
	}
	int dw = w2 - w1; 
	int dh = h2 - h1;
	// printf("[DEV]dw: %d\tdh: %d\n", dw, dh);
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
	// printf("[DEV]t: %f\n", t);
	for (int w = w1; w <= w2; w++) {
		int h = h1 + t * (w-w1);
		// printf("[DEV]w: %u\th: %u\n", w, h);
		if (steep) {
			// printf("[DEV]w: %u h: %u\n", w, h);
			tt_set_color(image, h-1, w-1, color);
		} else {
			// printf("[DEV]w: %u h: %u\n", w, h);
			tt_set_color(image, w-1, h-1, color);
		}
	}
}




/*
 * pts: points [A, B, C]
 * P  : barycentric coordinate = (1-u-v) u v
 *
 * */
Vec3f _barycentric(Vec2i *pts, Vec2i P) { 
	assert(pts != NULL);
	// (AB.x AC.x PA.x)
	Vec3f vec_x = {
		.x = (float)pts[1].x - (float)pts[0].x,
		.y = (float)pts[2].x - (float)pts[0].x,
		.z = (float)pts[0].x - (float)P.x
	};

	// (AB.y AC.y PA.y)
	Vec3f vec_y = {
		.x = (float)pts[1].y - (float)pts[0].y,
		.y = (float)pts[2].y - (float)pts[0].y,
		.z = (float)pts[0].y - (float)P.y 
	};

	// (u v 1)
	Vec3f u = vec3f_cross(vec_x, vec_y); 

	if (fabs(u.z) < 1) {
		Vec3f result = {
			.x = -1,
			.y = 1,
			.z = 1
		};
		return result;
	}
	Vec3f result = {
		.x = 1.f-(u.x+u.y)/u.z,
		.y = u.x/u.z,
		.z = u.y/u.z
	};
	return result;
}


void draw_triangle(tt_image *image, Vec2i *pts, tt_color color) {
	// bounding box ----------> x
	//              |  min
	//              |
	//              |           max
	//              V y

	Vec2i bboxmin = { .x = image->width-1, .y = image->height-1 };
	Vec2i bboxmax = { .x = 0, .y = 0 };
	Vec2i clamp = { .x = image->width-1, .y = image->height-1 };

	for (int i = 0; i < 3; i++) {
		bboxmin.x = (int)fmaxf(0, fminf((float)bboxmin.x, (float)pts[i].x));
		bboxmin.y = (int)fmaxf(0, fminf((float)bboxmin.y, (float)pts[i].y));

		bboxmax.x = (int)fminf((float)clamp.x, fmaxf((float)bboxmax.x, (float)pts[i].x));
		bboxmax.y = (int)fminf((float)clamp.y, fmaxf((float)bboxmax.y, (float)pts[i].y));
	}

	// printf("[DEV]min: (%d, %d)\n", bboxmin.x, bboxmin.y);
	// printf("[DEV]max: (%d, %d)\n", bboxmax.x, bboxmax.y);
	// printf("[DEV]clamp: (%d, %d)\n", clamp.x, clamp.y);

	Vec2i P;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
			Vec3f bc_screen = _barycentric(pts, P);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) {
				continue;
			}
			// printf("[DEV]u = (%f, %f, %f)\n", bc_screen.x, bc_screen.y, bc_screen.z);
			// printf("[DEV]P.x: %d\tP.y: %d\tin\n", P.x, P.y);
			tt_set_color(image, P.x-1, P.y-1, color);
		}

	}
}
