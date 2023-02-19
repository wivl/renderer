#include "tinytga.h"
#include "geometry.h"
#include "draw.h"
#include "tinyobj.h"
#include "matrix.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include <limits.h>


#define IMAGE_WIDTH 800
#define IMAGE_HEIGHT 800

int main(void) {
	srand(time(NULL));
	tobj_model *model = tobj_load_model("african_head.obj", TEXTURE_TGA);
    tobj_load_texture(model, "african_head_diffuse.tga");

	tt_color black = tt_make_color(0xFF000000);
	tt_color white = tt_make_color(0xFFFFFFFF);

	tt_image* image = tt_create(IMAGE_WIDTH, IMAGE_HEIGHT, black);

	
	Vec3f light_dir = { .x=0, .y=0, .z=-1 };
	Vec3f camera = { .x=0, .y=0, .z=3 };

	int *zbuffer = (int*)malloc(sizeof(int)*IMAGE_WIDTH*IMAGE_HEIGHT);
	for (int i = 0; i < IMAGE_HEIGHT*IMAGE_WIDTH; i++) {
		zbuffer[i] = INT_MIN;
	}

	Matrix Projection = m_identity(4);
    Matrix Viewpoint = viewpoint(IMAGE_WIDTH/8, IMAGE_HEIGHT/8, 
            IMAGE_WIDTH*3/4, IMAGE_HEIGHT*3/4, 255);
    m_set(&Projection, 3, 2, -1.f/camera.z);
    
    for (int i=0; i<model->f_capp; i++) {       // 每个面
        int *face = tobj_get_face(model, i);
        Vec3i screen_coords[3];
        Vec3f world_coords[3];
        for (int j=0; j<3; j++) {               // 每个点
            Vec3f v = model->verts[face[j]];
            screen_coords[j] =  vec3f_to_i(m_matrix_to_vec3f(m_multiply(m_multiply(Viewpoint, Projection), m_vec3f_to_matrix(v))));
            world_coords[j]  = v;
        }
        Vec3f n = vec3f_cross(vec3f_minus(world_coords[2], world_coords[0]), vec3f_minus(world_coords[1], world_coords[0]));
        vec3f_normalize(&n, 1);
        float intensity = vec3f_multiply_v(n, light_dir);
        if (intensity>0) {
            Vec2i uv[3];
            for (int k=0; k<3; k++) {
                uv[k] = tobj_get_uv(model, i, k);
            }
            draw_triangle_texture(screen_coords[0], screen_coords[1], screen_coords[2], uv[0], uv[1], uv[2], image, intensity, zbuffer, model);
        }
    }
    { // dump z-buffer (debugging purposes only)
        tt_image *zbimage = tt_create(IMAGE_WIDTH, IMAGE_HEIGHT, black);
        for (int i=0; i<IMAGE_WIDTH; i++) {
            for (int j=0; j<IMAGE_HEIGHT; j++) {
                tt_color color = {
                    .a = 0xFF,
                    .r = zbuffer[i+j*IMAGE_WIDTH],
                    .g = zbuffer[i+j*IMAGE_WIDTH],
                    .b = zbuffer[i+j*IMAGE_WIDTH],
                };
                tt_set_color(zbimage, i, j, color);
            }
        }
        tt_flip_vertically(zbimage);
        tt_save(zbimage, "zbuffer.tga");
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
