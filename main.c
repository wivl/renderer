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
    Vec3f eye = vec3f_make(2, 1, 4);
    Vec3f center = vec3f_make(0, 0, 0);
    Vec3f up = vec3f_make(0, 1, 0);

    Shader *shader = S_make();

    shader->light_dir = light_dir;
    
    S_set_modelview(shader, eye, center, up);
    S_set_viewport(shader, WIDTH/8, HEIGHT/8, WIDTH*3/4, HEIGHT*3/4);
    S_set_projection(shader, -1.f/vec3f_norm(vec3f_minus(eye, center)));
    S_get_final_matrix(shader);
    vec3f_normalize(&light_dir, 1);
    
    shader->light_dir = light_dir;



    tt_image *image = tt_create(WIDTH, HEIGHT, tt_make_color(0x00000000));

    int *zbuffer = (int*)malloc(sizeof(int)*WIDTH*HEIGHT);

    tobj_model *model = tobj_load_model("african_head.obj", TEXTURE_TGA);
    tobj_load_diffusemap(model, "african_head_diffuse.tga");
    tobj_load_normalmap(model, "african_head_nm.tga");

    for (int i = 0; i < model->f_capp; i++) {   // for every faces
        Vec4f screen_coords[3];
        for (int j = 0; j < 3; j++) {           // for every vertexes
            screen_coords[j] = S_vertex_shader_normalmapping(shader, model, i, j);
        }
        
        S_draw_triangle_normalmapping(shader, image, model, screen_coords, zbuffer);
    }

    tt_flip_vertically(image);

    tt_save(image, "image.tga");
    
}
