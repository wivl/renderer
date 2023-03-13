#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include "tinytga.h"
#include "tinyobj.h"
#include "matrix.h"
#include "geometry.h"
#include "shader.h"

#define WIDTH 1440
#define HEIGHT 1440

void shadow_shading(tobj_model *model, Vec3f light_dir, Vec3f eye, Vec3f center, Vec3f up);
void phong_shading(tobj_model *model, Vec3f light_dir, Vec3f eye, Vec3f center, Vec3f up);

int main(void) {
    
    tobj_model *model = tobj_load_model("diablo3_pose.obj", TEXTURE_TGA);
    tobj_load_diffusemap(model, "diablo3_pose_diffuse.tga");
    tobj_load_normalmap(model, "diablo3_pose_nm.tga");
    tobj_load_specularmap(model, "diablo3_pose_spec.tga");


    Vec3f light_dir = vec3f_make(1, 1, 1);
    Vec3f       eye = vec3f_make(1, 1, 4);
    Vec3f    center = vec3f_make(0, 0, 0);
    Vec3f        up = vec3f_make(0, 1, 0);
    vec3f_normalize(&light_dir, 1);

    phong_shading(model, light_dir, eye, center, up);

}





void phong_shading(tobj_model *model, Vec3f light_dir, Vec3f eye, Vec3f center, Vec3f up) {
    Shader *shader = S_make();

    shader->light_dir = light_dir;
    
    S_set_modelview(eye, center, up);
    S_set_viewport(WIDTH/8, HEIGHT/8, WIDTH*3/4, HEIGHT*3/4);
    S_set_projection(-1.f/vec3f_norm(vec3f_minus(eye, center)));
    vec3f_normalize(&light_dir, 1);
    
    shader->light_dir = light_dir;

    shader->uniform_M = m_multiply(Projection, ModelView);
    shader->uniform_MIT = m_invert_transpose(m_multiply(Projection, ModelView));


    tt_image *image = tt_create(WIDTH, HEIGHT, tt_make_color(0x00000000));

    int *zbuffer = (int*)malloc(sizeof(int)*WIDTH*HEIGHT);

    
    for (int i = 0; i < model->f_capp; i++) {   // for every faces
        Vec4f screen_coords[3];
        for (int j = 0; j < 3; j++) {           // for every vertexes
            screen_coords[j] = S_vertex_shader_specular(shader, model, i, j);
        }
        
        S_draw_triangle_specular(shader, image, model, screen_coords, zbuffer);
    }

    tt_flip_vertically(image);

    tt_save(image, "render.tga");
}

void shadow_shading(tobj_model *model, Vec3f light_dir, Vec3f eye, Vec3f center, Vec3f up) {
    Matrix M;
    tt_color black = tt_make_color(0xFF000000);
    tt_color blank = tt_make_color(0x00000000);

    float *zbuffer = (float*)malloc(sizeof(float)*WIDTH*HEIGHT);
    for (int i = 0; i < WIDTH*HEIGHT; i++) {
        zbuffer[i] = -FLT_MAX;
    }

    float *shadowbuffer = (float*)malloc(sizeof(float)*WIDTH*HEIGHT);
    for (int i = 0; i < WIDTH*HEIGHT; i++) {
        shadowbuffer[i] = -FLT_MAX;
    }

    {
        // rendering the depth buffer
        tt_image *depthframe = tt_create(WIDTH, HEIGHT, blank);
        Shader *depthshader = S_make();
        S_set_modelview(light_dir, center, up);
        S_set_viewport(WIDTH/8, HEIGHT/8, WIDTH*3/4, HEIGHT*3/4);
        S_set_projection(0);
        S_init_shader(depthshader);
        depthshader->light_dir = light_dir;

        // M = depthshader->final_matrix;

        for (int i = 0; i < model->f_capp; i++) {   // for every faces
            Vec4f screen_coords[3];
            for (int j = 0; j < 3; j++) {           // for every vertex
                screen_coords[j] = S_vertex_shader_depth(depthshader, model, i, j);
            }
            S_draw_triangle_depth(depthshader, depthframe, model, screen_coords, shadowbuffer);
        }

        tt_flip_vertically(depthframe);
        tt_save(depthframe, "depth.tga");

    }

    M = m_multiply(m_multiply(Viewport, Projection), ModelView);

    {
        // rendering the framebuffer
        tt_image *image = tt_create(WIDTH, HEIGHT, blank);
        Shader *shader = S_make();
        

        S_set_modelview(eye, center, up);
        S_set_viewport(WIDTH/8, HEIGHT/8, WIDTH*3/4, HEIGHT*3/4);
        S_set_projection(-1.f/vec3f_norm(vec3f_minus(eye, center)));
        S_init_shader(shader);
        vec3f_normalize(&light_dir, 1);
        shader->light_dir = light_dir;

        shader->uniform_M = ModelView;
        Matrix tmp = m_multiply(m_multiply(Viewport, Projection), ModelView);
        shader->uniform_Mshadow = m_multiply(M, m_inverse(&tmp));
        printf("[DEBUG]uniform_M:\n");
        for (int i = 0; i < shader->uniform_M.rows; i++) {
            printf("[");
            for (int j = 0; j < shader->uniform_M.cols; j++) {
                printf("%f ", m_get(&shader->uniform_M, i, j));
            }
            printf("]\n");
        }

        printf("[DEBUG]uniform_Mshadow:\n");
        for (int i = 0; i < shader->uniform_Mshadow.rows; i++) {
            printf("[");
            for (int j = 0; j < shader->uniform_Mshadow.cols; j++) {
                printf("%f ", m_get(&shader->uniform_Mshadow, i, j));
            }
            printf("]\n");
        }
        for (int i = 0; i < model->f_capp; i++) {   // for every faces
            Vec4f screen_coords[3];
            for (int j = 0; j < 3; j++) {           // for every vertexes
                screen_coords[j] = S_vertex_shader_shadow(shader, model, i, j);
            }
            S_draw_triangle_shadow(shader, image, model, screen_coords, zbuffer, shadowbuffer);
        }
        tt_flip_vertically(image);
        tt_save(image, "render.tga");
    }

}
