#include "shader.h"
#include "geometry.h"
#include "matrix.h"
#include "tinyobj.h"
#include "tinytga.h"
#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: What if multiple objects are in space?
// How to transform model space to world space?
// This renderer doesnt contain model -> world transform


// make shader
Shader* S_make() {
    Shader *shader = (Shader*)malloc(sizeof(Shader));
    if (shader == NULL) {
        assert(false && "err: no enough memmory");
    }
    shader->varying_uv = m_make(2, 3);
    shader->varying_tri = m_make(3, 3);
    return shader;
}

// TODO: Clip coordinate to screen coordinate
void S_set_viewport(int x, int y, int w, int h) {
    Matrix m = m_identity(4);
    m.data[0*m.cols+3] = x+w/2.f;
    m.data[1*m.cols+3] = y+h/2.f;
    m.data[2*m.cols+3] = DEPTH/2.f;

    m.data[0*m.cols+0] = w/2.f;
    m.data[1*m.cols+1] = h/2.f;
    m.data[2*m.cols+2] = DEPTH/2.f;
    Viewport = m;
}

// projection
// the result
void S_set_projection(float coeff) {
    Matrix projection = m_identity(4);
    m_set(&projection, 3, 2, coeff);
    // projection.data[3*projection.cols+2] = coeff;
    Projection = projection;
    
}

// See https://github.com/ssloy/tinyrenderer/wiki/Lesson-5:-Moving-the-camera#let-us-create-our-own-glulookat
// world to camera
// lookat()
void S_set_modelview(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f z = vec3f_minus(eye, center);
    vec3f_normalize(&z, 1);
    Vec3f x = vec3f_cross(up, z);
    vec3f_normalize(&x, 1);
    Vec3f y = vec3f_cross(z, x);
    vec3f_normalize(&y, 1);
    Matrix modelview = m_identity(4);

    modelview.data[0*modelview.cols+0] = x.x;
    modelview.data[1*modelview.cols+0] = y.x;
    modelview.data[2*modelview.cols+0] = z.x;
    modelview.data[0*modelview.cols+3] = -center.x;

    modelview.data[0*modelview.cols+1] = x.y;
    modelview.data[1*modelview.cols+1] = y.y;
    modelview.data[2*modelview.cols+1] = z.y;
    modelview.data[1*modelview.cols+3] = -center.y;

    modelview.data[0*modelview.cols+2] = x.z;
    modelview.data[1*modelview.cols+2] = y.z;
    modelview.data[2*modelview.cols+2] = z.z;
    modelview.data[2*modelview.cols+3] = -center.z;

    ModelView = modelview;
}

void S_init_shader(Shader *shader) {
    shader->uniform_M = m_multiply(Projection, ModelView);
    shader->uniform_MIT = m_invert_transpose(m_multiply(Projection, ModelView));

    // printf("[DEBUG]Final matrix:\n");
    // for (int i = 0; i < shader->final_matrix.rows; i++) {
    //    printf("[");
    //    for (int j = 0; j < shader->final_matrix.cols; j++) {
    //        printf("%f ", m_get(&shader->final_matrix, i, j));
    //    }
    //    printf("]\n");
    // }
}


/*
 * pts: points [A, B, C]
 * P  : barycentric coordinate = (1-u-v) u v
 *
 * */
Vec3f barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P) {
    // (AB.x AC.x PA.x)
    Vec3f vec_x = {
        .x = C.x - A.x,
        .y = B.x - A.x,
        .z = A.x - P.x
    };

    // (AB.y AC.y PA.y)
    Vec3f vec_y = {
        .x = C.y - A.y,
        .y = B.y - A.y,
        .z = A.y - P.y 
    };

    // (u v 1)
    Vec3f u = vec3f_cross(vec_x, vec_y); 

    if (fabs(u.z) > 1e-2) {
        return vec3f_make(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    } 
    return vec3f_make(-1, 1, 1);
}


Vec4f S_vertex_shader(Shader *shader, tobj_model *model, int iface, int nthvert) {
    // get nth vert of face i, and extend to 4 dimension
    Vec4f vertex = vec3f_to_4f(tobj_get_vert_from_face(model, iface, nthvert), 1);

    // calculate the transformed vertex
    vertex = m_multiply_vec4f(m_multiply(m_multiply(Viewport, Projection), ModelView), vertex);

    // set shader.varying_intensity
    vec3f_set(&shader->varying_intensity, nthvert,
            // normal * light to get intensity
            fmaxf(0, vec3f_multiply_v(tobj_get_normal(model, iface, nthvert), shader->light_dir)));

    return vertex;
}

bool S_fragment_shader(Shader *shader, Vec3f bar, tt_color *color) {
    // bar means barycentric coordinate of the current triangle. For every
    // triangle we calculate intensities of 3 vertices, to achieve a kind of 
    // gradient effect.
    float intensity = vec3f_multiply_v(shader->varying_intensity, bar);
    color->r = 0xFF * intensity;
    color->g = 0xFF * intensity;
    color->b = 0xFF * intensity;
    color->a = 0xFF;
    return false;
}

void S_draw_triangle(Shader *shader, tt_image *image, tobj_model *model,
        Vec4f *pts, int *zbuffer) {
    // bounding box min and max
    Vec2f bboxmin = { .x = FLT_MAX, .y = FLT_MAX };
    Vec2f bboxmax = { .x = -FLT_MAX, .y = -FLT_MAX };

    // calculate bounding boxes
    for (int i = 0; i < 3; i++) {
        bboxmin.x = fminf(bboxmin.x, pts[i].x/pts[i].w);
        bboxmin.y = fminf(bboxmin.y, pts[i].y/pts[i].w);

        bboxmax.x = fmaxf(bboxmax.x, pts[i].x/pts[i].w);
        bboxmax.y = fmaxf(bboxmax.y, pts[i].y/pts[i].w);
    }
    // for every Point in bounding box
    Vec2i P;
    tt_color color;

    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) { // for every pixels
                                                         // barycentric coordinate of P
            Vec3f c = barycentric(
                    vec2f_make(pts[0].x/pts[0].w, pts[0].y/pts[0].w),
                    vec2f_make(pts[1].x/pts[1].w, pts[1].y/pts[1].w),
                    vec2f_make(pts[2].x/pts[2].w, pts[2].y/pts[2].w),
                    vec2f_make(P.x, P.y)
                    );
            // get z axie
            float z = pts[0].z*c.x + pts[1].z*c.y + pts[2].z*c.z;
            float w = pts[0].w*c.x + pts[1].w*c.y + pts[2].w*c.z;
            // P's zbuffer
            // 0 ~ 255
            int frag_depth = fmaxf(0, fminf(255, (float)(int)(z/w+.5)));
            // if P in triangle and P at top
            if (c.x<0 || c.y<0 || c.z<0 || zbuffer[P.y*image->width+P.x] > frag_depth) 
                continue;
            bool discard = S_fragment_shader(shader, c, &color);
            if (!discard) {
                zbuffer[P.y*image->width+P.x] = frag_depth;
                tt_set_color(image, P.x, P.y, color);
            }
        }
    }
}


Vec4f S_vertex_shader_texture(Shader *shader, tobj_model *model, int iface, int nthvert) {
    // get uv from model and save to varying_uv
    m_set_col2(&(shader->varying_uv), nthvert, tobj_get_uv(model, iface, nthvert));
    // get nth vert of face i, and extend to 4 dimension with 1 
    Vec4f vertex = vec3f_to_4f(tobj_get_vert_from_face(model, iface, nthvert), 1);
    // calculate the transformed vertex
    vertex = m_multiply_vec4f(m_multiply(m_multiply(Viewport, Projection), m_multiply(m_multiply(Viewport, Projection), ModelView)), vertex);
    // set shader.varying_intensity
    vec3f_set(&shader->varying_intensity, nthvert,
            // calculate intensity (0~1)
            fmaxf(0, vec3f_multiply_v(tobj_get_normal(model, iface, nthvert), shader->light_dir)));
    return vertex;
}

bool S_fragment_shader_texture(Shader *shader, tobj_model *model, Vec3f bar, tt_color *color) {
    // same as above
    float intensity = vec3f_multiply_v(shader->varying_intensity, bar);
    // get uv coordinate using barycentric coordinate, same as above
    Vec2f uv = m_multiply_vec3f_2(shader->varying_uv, bar);
    *color = tobj_diffuse(model, uv);
    // get current pixel color: color * intensity
    tt_color_intensity(color, intensity);
    return false;
}

// same as above
void S_draw_triangle_texture(Shader *shader, tt_image *image, tobj_model *model,
        Vec4f *pts, int *zbuffer) {
    Vec2f bboxmin = { .x = FLT_MAX, .y = FLT_MAX };
    Vec2f bboxmax = { .x = -FLT_MAX, .y = -FLT_MAX };

    // calculate bounding boxes
    for (int i = 0; i < 3; i++) {
        bboxmin.x = fminf(bboxmin.x, pts[i].x/pts[i].w);
        bboxmin.y = fminf(bboxmin.y, pts[i].y/pts[i].w);

        bboxmax.x = fmaxf(bboxmax.x, pts[i].x/pts[i].w);
        bboxmax.y = fmaxf(bboxmax.y, pts[i].y/pts[i].w);
    }
    Vec2i P;
    tt_color color;

    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) { // for every pixels
            Vec3f c = barycentric(
                    vec2f_make(pts[0].x/pts[0].w, pts[0].y/pts[0].w),
                    vec2f_make(pts[1].x/pts[1].w, pts[1].y/pts[1].w),
                    vec2f_make(pts[2].x/pts[2].w, pts[2].y/pts[2].w),
                    vec2f_make(P.x, P.y)
                    );
            float z = pts[0].z*c.x + pts[1].z*c.y + pts[2].z*c.z;
            float w = pts[0].w*c.x + pts[1].w*c.y + pts[2].w*c.z;
            int frag_depth = fmaxf(0, fminf(255, (float)(int)(z/w+.5)));
            if (c.x<0 || c.y<0 || c.z<0 || zbuffer[P.y*image->width+P.x] > frag_depth) 
                continue;
            bool discard = S_fragment_shader_texture(shader, model, c, &color);
            if (!discard) {
                zbuffer[P.y*image->width+P.x] = frag_depth;
                tt_set_color(image, P.x, P.y, color);
            }
        }
    }
}

Vec4f S_vertex_shader_normalmapping(Shader *shader, tobj_model *model, int iface, int nthvert) {
    // get uv coordinate from .object file
    m_set_col2(&(shader->varying_uv), nthvert, tobj_get_uv(model, iface, nthvert));

    // get nth vert of face i, and extend to 4 dimension
    Vec4f vertex = vec3f_to_4f(tobj_get_vert_from_face(model, iface, nthvert), 1);

    // calculate the transformed vertex
    vertex = m_multiply_vec4f(m_multiply(m_multiply(Viewport, Projection), ModelView), vertex);

    return vertex;

}

bool S_fragment_shader_normalmapping(Shader *shader, tobj_model *model, Vec3f bar, tt_color *color) {
    // same as above
    Vec2f uv = m_multiply_vec3f_2(shader->varying_uv, bar);

    // transform vectors
    // uniform_MIT eqs to (Projection*ModelView).invert_transpose(), see https://github.com/ssloy/tinyrenderer/wiki/Lesson-5:-Moving-the-camera#transformation-of-normal-vectors
    // Now we don't actually care about the camera view since we focus on the color of the pixel. So here we just use uniform_M and uniform_MIT.
    Vec3f n = vec4f_to_3f(m_multiply_vec4f(shader->uniform_MIT, 
                vec3f_to_4f(tobj_get_normal_from_map(model, uv), 1)));
    vec3f_normalize(&n, 1);

    // light_dir in world coordinate
    Vec3f l = vec4f_to_3f(m_multiply_vec4f(shader->uniform_M, vec3f_to_4f(shader->light_dir, 1)));
    vec3f_normalize(&l, 1);

    
    // same as above
    float intensity = fmaxf(0, vec3f_multiply_v(n, l));
    *color = tobj_diffuse(model, uv);
    tt_color_intensity(color, intensity);
    return false;
}

// same as above
void S_draw_triangle_normalmapping(Shader *shader, tt_image *image, tobj_model *model,
        Vec4f *pts, int *zbuffer) {
    Vec2f bboxmin = { .x = FLT_MAX, .y = FLT_MAX };
    Vec2f bboxmax = { .x = -FLT_MAX, .y = -FLT_MAX };

    // calculate bounding boxes
    for (int i = 0; i < 3; i++) {
        bboxmin.x = fminf(bboxmin.x, pts[i].x/pts[i].w);
        bboxmin.y = fminf(bboxmin.y, pts[i].y/pts[i].w);

        bboxmax.x = fmaxf(bboxmax.x, pts[i].x/pts[i].w);
        bboxmax.y = fmaxf(bboxmax.y, pts[i].y/pts[i].w);
    }
    Vec2i P;
    tt_color color;

    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) { // for every pixels
            Vec3f c = barycentric(
                    vec2f_make(pts[0].x/pts[0].w, pts[0].y/pts[0].w),
                    vec2f_make(pts[1].x/pts[1].w, pts[1].y/pts[1].w),
                    vec2f_make(pts[2].x/pts[2].w, pts[2].y/pts[2].w),
                    vec2f_make(P.x, P.y)
                    );
            float z = pts[0].z*c.x + pts[1].z*c.y + pts[2].z*c.z;
            float w = pts[0].w*c.x + pts[1].w*c.y + pts[2].w*c.z;
            int frag_depth = fmaxf(0, fminf(255, (float)(int)(z/w+.5)));
            if (c.x<0 || c.y<0 || c.z<0 || zbuffer[P.y*image->width+P.x] > frag_depth) 
                continue;
            bool discard = S_fragment_shader_normalmapping(shader, model, c, &color);
            if (!discard) {
                zbuffer[P.y*image->width+P.x] = frag_depth;
                tt_set_color(image, P.x, P.y, color);
            }
        }
    }
}

Vec4f S_vertex_shader_specular(Shader *shader, tobj_model *model, int iface, int nthvert) {
    m_set_col2(&(shader->varying_uv), nthvert, tobj_get_uv(model, iface, nthvert));

    // get nth vert of face i, and extend to 4 dimension
    Vec4f vertex = vec3f_to_4f(tobj_get_vert_from_face(model, iface, nthvert), 1);

    // calculate the transformed vertex
    vertex = m_multiply_vec4f(m_multiply(m_multiply(Viewport, Projection), ModelView), vertex);

    return vertex;
}

bool S_fragment_shader_specular(Shader *shader, tobj_model *model, Vec3f bar, tt_color *color) {
    // Same as above
    Vec2f uv = m_multiply_vec3f_2(shader->varying_uv, bar);
    Vec3f n = vec4f_to_3f(m_multiply_vec4f(shader->uniform_MIT, 
                vec3f_to_4f(tobj_get_normal_from_map(model, uv), 1)));
    vec3f_normalize(&n, 1);
    Vec3f l = vec4f_to_3f(m_multiply_vec4f(shader->uniform_M, vec3f_to_4f(shader->light_dir, 1)));
    vec3f_normalize(&l, 1);

    // Calculate reflected light, see https://github.com/ssloy/tinyrenderer/wiki/Lesson-6:-Shaders-for-the-software-renderer#specular-mapping
    // Vec3f r = (n*(n*l*2.f) - l).normalize();   // reflected light
    Vec3f r = vec3f_minus(vec3f_multiply_f(n, vec3f_multiply_v(n, l)*2.f), l);   // reflected light
    vec3f_normalize(&r, 1);

    // Specular relection part intensity
    float spec = powf(fmaxf(r.z, 0.0f), tobj_get_specular(model, uv));
    // Diffuse relection part intensity
    float diff = fmaxf(0.f, vec3f_multiply_v(n, l));

    *color = tobj_diffuse(model, uv);

    // Magic
    color->b = fminf(5+color->b*(diff+0.6*spec), 255);
    color->g = fminf(5+color->g*(diff+0.6*spec), 255);
    color->r = fminf(5+color->r*(diff+0.6*spec), 255);

    return false;
}

// same as above
void S_draw_triangle_specular(Shader *shader, tt_image *image, tobj_model *model,
        Vec4f *pts, int *zbuffer) {
    Vec2f bboxmin = { .x = FLT_MAX, .y = FLT_MAX };
    Vec2f bboxmax = { .x = -FLT_MAX, .y = -FLT_MAX };

    // calculate bounding boxes
    for (int i = 0; i < 3; i++) {
        bboxmin.x = fminf(bboxmin.x, pts[i].x/pts[i].w);
        bboxmin.y = fminf(bboxmin.y, pts[i].y/pts[i].w);

        bboxmax.x = fmaxf(bboxmax.x, pts[i].x/pts[i].w);
        bboxmax.y = fmaxf(bboxmax.y, pts[i].y/pts[i].w);
    }
    Vec2i P;
    tt_color color;

    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) { // for every pixels
            Vec3f c = barycentric(
                    vec2f_make(pts[0].x/pts[0].w, pts[0].y/pts[0].w),
                    vec2f_make(pts[1].x/pts[1].w, pts[1].y/pts[1].w),
                    vec2f_make(pts[2].x/pts[2].w, pts[2].y/pts[2].w),
                    vec2f_make(P.x, P.y)
                    );
            float z = pts[0].z*c.x + pts[1].z*c.y + pts[2].z*c.z;
            float w = pts[0].w*c.x + pts[1].w*c.y + pts[2].w*c.z;
            int frag_depth = z/w;
            if (c.x<0 || c.y<0 || c.z<0 || zbuffer[P.y*image->width+P.x] > frag_depth) 
                continue;
            bool discard = S_fragment_shader_specular(shader, model, c, &color);
            if (!discard) {
                zbuffer[P.y*image->width+P.x] = frag_depth;
                tt_set_color(image, P.x, P.y, color);
            }
        }
    }
}

// get depthbuffer to help generating shadow
Vec4f S_vertex_shader_depth(Shader *shader, tobj_model *model, int iface, int nthvert) {
    // get nth vert of face i, and extend to 4 dimension
    Vec4f vertex = vec3f_to_4f(tobj_get_vert_from_face(model, iface, nthvert), 1);

    // calculate the transformed vertex
    vertex = m_multiply_vec4f(m_multiply(m_multiply(Viewport, Projection), ModelView), vertex);

    // varying_tri:
    // [x1 x2 x3]
    // [y1 y2 y3]
    // [z1 z2 z3]
    m_set_col3(&(shader->varying_tri), nthvert, vec4f_to_3f(vec4f_multiply_f(vertex, 1.f/vertex.w)));

    return vertex;
    // And in fragment shader:

}

bool S_fragment_shader_depth(Shader *shader, tobj_model *model, Vec3f bar, tt_color *color) {
    // we calculate p as:
    // [x1 x2 x3]   [bar1
    // [y1 y2 y3] *  bar2 ,
    // [z1 z2 z3]    bar3]
    // And as a rusult vector p is [xbar, ybar, zbar]
    Vec3f p = m_multiply_vec3f_3(shader->varying_tri, bar);
    *color = tt_make_color(0xFFFFFFFF);

    // We only care about z-value
    tt_color_intensity(color, fabsf(p.z/DEPTH)); // we only cares about shadowbuffer
    return false;
}

void S_draw_triangle_depth(Shader *shader, tt_image *image, tobj_model *model,
        Vec4f *pts, float *zbuffer) {
    Vec2f bboxmin = { .x = FLT_MAX, .y = FLT_MAX };
    Vec2f bboxmax = { .x = -FLT_MAX, .y = -FLT_MAX };

    // calculate bounding boxes
    for (int i = 0; i < 3; i++) {
        bboxmin.x = fminf(bboxmin.x, pts[i].x/pts[i].w);
        bboxmin.y = fminf(bboxmin.y, pts[i].y/pts[i].w);

        bboxmax.x = fmaxf(bboxmax.x, pts[i].x/pts[i].w);
        bboxmax.y = fmaxf(bboxmax.y, pts[i].y/pts[i].w);
    }
    Vec2i P;
    tt_color color;

    

    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) { // for every pixels
            Vec3f c = barycentric(
                    vec2f_make(pts[0].x/pts[0].w, pts[0].y/pts[0].w),
                    vec2f_make(pts[1].x/pts[1].w, pts[1].y/pts[1].w),
                    vec2f_make(pts[2].x/pts[2].w, pts[2].y/pts[2].w),
                    vec2f_make(P.x, P.y)
                    );
            float z = pts[0].z*c.x + pts[1].z*c.y + pts[2].z*c.z;
            float w = pts[0].w*c.x + pts[1].w*c.y + pts[2].w*c.z;
            // more depth space (255 to 2000)
            int frag_depth = z/w;
            if (c.x<0 || c.y<0 || c.z<0 || zbuffer[P.y*image->width+P.x] > frag_depth) 
                continue;
            bool discard = S_fragment_shader_depth(shader, model, c, &color);
            if (!discard) {
                zbuffer[P.y*image->width+P.x] = frag_depth;
                tt_set_color(image, P.x, P.y, color);
            }
        }
    }

}

Vec4f S_vertex_shader_shadow(Shader *shader, tobj_model *model, int iface, int nthvert) {
    m_set_col2(&(shader->varying_uv), nthvert, tobj_get_uv(model, iface, nthvert));

    // get nth vert of face i, and extend to 4 dimension
    Vec4f vertex = vec3f_to_4f(tobj_get_vert_from_face(model, iface, nthvert), 1);

    // calculate the transformed vertex
    vertex = m_multiply_vec4f(m_multiply(m_multiply(Viewport, Projection), ModelView), vertex);

    // same as above
    m_set_col3(&(shader->varying_tri), nthvert, vec4f_to_3f(vec4f_multiply_f(vertex, 1.f/vertex.w)));

    return vertex;


}

bool S_fragment_shader_shadow(Shader *shader, tt_image *image, tobj_model *model, float *shadowbuffer, Vec3f bar, tt_color *color) {
    // shadowbuffer_point
    // uniform_Mshadow: transform framebuffer screen coordinates to shadowbuffer screen coordinates
    // TODO: uniform_Mshadow
    Vec4f sb_p = m_multiply_vec4f(shader->uniform_Mshadow, 
            // below returns xbar ybar zbar 1
            vec3f_to_4f(m_multiply_vec3f_3(shader->varying_tri, bar), 1));  // corresponding point in the shadow buffer
    sb_p = vec4f_multiply_f(sb_p, 1.f/sb_p.w);

    // index in the shadowbuffer array
    int idx = (int)sb_p.x + (int)sb_p.y * image->width; 
    // Shadow intensity and magic
    float shadow = .3+.7*(shadowbuffer[idx]<sb_p.z); 

    // same as above
    Vec2f uv = m_multiply_vec3f_2(shader->varying_uv, bar);
    Vec3f n = vec4f_to_3f(m_multiply_vec4f(shader->uniform_MIT, 
                vec3f_to_4f(tobj_get_normal_from_map(model, uv), 1)));
    vec3f_normalize(&n, 1);
    Vec3f l = vec4f_to_3f(m_multiply_vec4f(shader->uniform_M, vec3f_to_4f(shader->light_dir, 1)));
    vec3f_normalize(&l, 1);
    Vec3f r = vec3f_minus(vec3f_multiply_f(n, vec3f_multiply_v(n, l)*2.f), l);   // reflected light
    vec3f_normalize(&r, 1);

    float spec = powf(fmaxf(r.z, 0.0f), tobj_get_specular(model, uv));
    float diff = fmaxf(0.f, vec3f_multiply_v(n, l));
    *color = tobj_diffuse(model, uv);

    // Add shadow coeff
    color->b = fminf(20+color->b*shadow*(1.2*diff+0.6*spec), 255);
    color->g = fminf(20+color->g*shadow*(1.2*diff+0.6*spec), 255);
    color->r = fminf(20+color->r*shadow*(1.2*diff+0.6*spec), 255);
    return false;
}

// same as above
void S_draw_triangle_shadow(Shader *shader, tt_image *image, tobj_model *model,
        Vec4f *pts, float *zbuffer, float *shadowbuffer) {
    Vec2f bboxmin = { .x = FLT_MAX, .y = FLT_MAX };
    Vec2f bboxmax = { .x = -FLT_MAX, .y = -FLT_MAX };

    // calculate bounding boxes
    for (int i = 0; i < 3; i++) {
        bboxmin.x = fminf(bboxmin.x, pts[i].x/pts[i].w);
        bboxmin.y = fminf(bboxmin.y, pts[i].y/pts[i].w);

        bboxmax.x = fmaxf(bboxmax.x, pts[i].x/pts[i].w);
        bboxmax.y = fmaxf(bboxmax.y, pts[i].y/pts[i].w);
    }
    Vec2i P;
    tt_color color;

    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) { // for every pixels
            Vec3f c = barycentric(
                    vec2f_make(pts[0].x/pts[0].w, pts[0].y/pts[0].w),
                    vec2f_make(pts[1].x/pts[1].w, pts[1].y/pts[1].w),
                    vec2f_make(pts[2].x/pts[2].w, pts[2].y/pts[2].w),
                    vec2f_make(P.x, P.y)
                    );
            float z = pts[0].z*c.x + pts[1].z*c.y + pts[2].z*c.z;
            float w = pts[0].w*c.x + pts[1].w*c.y + pts[2].w*c.z;
            int frag_depth = z/w;
            if (c.x<0 || c.y<0 || c.z<0 || zbuffer[P.y*image->width+P.x] > frag_depth) 
                continue;
            bool discard = S_fragment_shader_shadow(shader, image, model, shadowbuffer, c, &color);
            if (!discard) {
                zbuffer[P.y*image->width+P.x] = frag_depth;
                tt_set_color(image, P.x, P.y, color);
            }
        }
    }
}

// TODO: Advanced
Vec4f S_vertex_shader_AO(Shader *shader, tobj_model *model, int iface, int nthvert) {
    // get nth vert of face i, and extend to 4 dimension
    Vec4f vertex = vec3f_to_4f(tobj_get_vert_from_face(model, iface, nthvert), 1);
    // calculate the transformed vertex
    vertex = m_multiply_vec4f(m_multiply(Projection, ModelView), vertex);

    m_set_col4(&(shader->varying_tri), nthvert, vertex);

    return vertex;
}

bool S_fragment_shader_AO(Shader *shader, Vec3f fragcoord, Vec3f bar, tt_color *color) {
    *color = tt_make_color(0xFF000000);
    return false;
}

float max_elevation_angle(float *zbuffer, Vec2f P, Vec2f dir) {
    float max_angle = 0;
    return max_angle;
}



// TODO: 
void S_draw_triangle_AO(Shader *shader, tt_image *image, tobj_model *model,
        Vec4f *pts, float *zbuffer) {

}





