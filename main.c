#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tinytga.h"
#include "tinyobj.h"
#include "matrix.h"
#include "geometry.h"
#include "shader.h"

#define WIDTH 800
#define HEIGHT 800

int main(void) {
    tobj_load_model("african_head.obj", NO_TEXTURE);

    Vec3f light_dir = vec3f_make(1, 1, 1);
    Vec3f eye = vec3f_make(0, 4, 4);
    Vec3f center = vec3f_make(0, 0, 0);
    Vec3f up = vec3f_make(0, 1, 0);

    Shader shader = {
        .light_dir = light_dir,
        .eye = eye,
        .center = center,
        .up = up
    };
    
    S_set_modelview(&shader, eye, center, up);
    S_set_viewport(&shader, WIDTH/8, HEIGHT/8, WIDTH*3/4, HEIGHT*3/4);
    S_set_projection(&shader, -1.f/vec3f_norm(vec3f_minus(eye, center)));
    S_get_final_matrix(&shader);
    vec3f_normalize(&light_dir, 1);
    
    shader.light_dir = light_dir;

#define pink 0xFFFFB6C1

    tt_image *image = tt_create(WIDTH, HEIGHT, tt_make_color(0x00000000));
    // tt_image *zbuffer = tt_create(WIDTH, HEIGHT, tt_make_color(0x00000000));

    int *zbuffer = (int*)malloc(sizeof(int)*WIDTH*HEIGHT);

    tobj_model *model = tobj_load_model("african_head.obj", TEXTURE_TGA);
    tobj_load_texture(model, "african_head_diffuse.tga");

    printf("[DEBUG]fcapp: %lu\n", model->f_capp);
    for (int i = 0; i < model->f_capp; i++) {   // for every faces
        Vec4f screen_coords[3];
        for (int j = 0; j < 3; j++) {           // for every vertexes
            screen_coords[j] = S_vertex_shader(&shader, model, i, j);
        }
        // printf("[DEBUG]Ver shader set varying_intensity to (%f, %f, %f)\n",
        //    shader.varying_intensity.x, 
        //    shader.varying_intensity.y,
        //    shader.varying_intensity.z);
        //printf("[DEBUG]After v shader: (%f, %f) (%f, %f) (%f, %f)\n", 
        //        screen_coords[0].x, screen_coords[0].y,
        //        screen_coords[1].x, screen_coords[1].y,
        //        screen_coords[2].x, screen_coords[2].y
        //        );
        S_draw_triangle(&shader, image, model, screen_coords, zbuffer);
    }

    tt_flip_vertically(image);
    // tt_flip_vertically(zbuffer);

    tt_save(image, "image.tga");
    // tt_save(zbuffer, "zbuffer.tga");
    
}
