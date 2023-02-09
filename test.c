#include "tinytga.h"
#include "geometry.h"
#include "draw.h"
#include "tinyobj.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#define IMAGE_WIDTH 800
#define IMAGE_HEIGHT 800

int main(void) {
	srand(time(NULL));
	tobj_model *model = tobj_load_model("african_head.obj");

	tt_color black = tt_make_color(0xFF000000);
	tt_color white = tt_make_color(0xFFFFFFFF);

	tt_image* image = tt_create(IMAGE_WIDTH, IMAGE_HEIGHT, black);

	
	Vec3f light_dir = { .x=0, .y=0, .z=-1 };

	float *zbuffer = (float*)malloc(sizeof(float)*IMAGE_WIDTH*IMAGE_HEIGHT);
	for (int i = 0; i < IMAGE_HEIGHT*IMAGE_WIDTH; i++) {
		zbuffer[i] = -FLT_MAX;
	}

	for (int i=0; i<model->f_capp; i++) { // 每个面
        int *face = model->faces[i];
		Vec3f pts[3];
		Vec3f world_coords[3];
        for (int j=0; j<3; j++) { // 每个点
			world_coords[j] = model->verts[face[j]-1];
			pts[j] = world2screen(model->verts[face[j]-1], IMAGE_WIDTH, IMAGE_HEIGHT);
        }
		Vec3f n = vec3f_cross(vec3f_minus(world_coords[2], world_coords[0]), vec3f_minus(world_coords[1], world_coords[0]));
		vec3f_normalize(&n, 1);
		float intensity = vec3f_multiply_v(n, light_dir);
		// tt_color color = { .a = 0xFF, .r = rand()%255, .g = rand()%255, .b = rand()%255};
		if (intensity > 0) {
			tt_color color = { .a = 0xFF, .r = intensity*255, .g = intensity*255, .b = intensity*255};
			draw_triangle(image, pts, zbuffer, color);
		}
		
    }

	
	tt_flip_vertically(image);
	tt_save(image, "saved.tga");
	return 0;
	
	// tt_image *frame = tt_create(200, 200, black);
	// Vec2i A = { .x = 10, .y = 10 };
	// Vec2i B = { .x = 100, .y = 30 };
	// Vec2i C = { .x = 190, .y = 160 };
	// Vec2i pts[3] = {A, B, C};


	// draw_triangle(frame, pts, white);

	// tt_save(frame, "triangle.tga");
}
