#include "shader.h"
#include "geometry.h"
#include "matrix.h"
#include "tinyobj.h"
#include "tinytga.h"
#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>

// world to camera
void S_set_viewport(Shader *shader, int x, int y, int w, int h) {
    assert(shader != NULL);
    Matrix m = m_identity(4);
    m.data[0*m.cols+3] = x+w/2.f;
    m.data[1*m.cols+3] = y+h/2.f;
    m.data[2*m.cols+3] = 255.f/2.f;

    m.data[0*m.cols+0] = w/2.f;
    m.data[1*m.cols+1] = h/2.f;
    m.data[2*m.cols+2] = 255.f/2.f;
    shader->viewport = m;
    //printf("[DEBUG]viewport:\n");
    //for (int i = 0; i < m.rows; i++) {
    //    printf("[");
    //    for (int j = 0; j < m.cols; j++) {
    //        printf("%f ", m_get(&m, i, j));
    //    }
    //    printf("]\n");
    //}
}

// projection
void S_set_projection(Shader *shader, float coeff) {
    assert(shader != NULL);
    Matrix projection = m_identity(4);
    projection.data[3*projection.cols+2] = coeff;
    shader->projection = projection;
    //printf("[DEBUG]projection:\n");
    //for (int i = 0; i < projection.rows; i++) {
    //    printf("[");
    //    for (int j = 0; j < projection.cols; j++) {
    //        printf("%f ", m_get(&projection, i, j));
    //    }
    //    printf("]\n");
    //}
}

// model to world
void S_set_modelview(Shader *shader, Vec3f eye, Vec3f center, Vec3f up) {
    assert(shader != NULL);
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

    shader->modelview = modelview;
    //printf("[DEBUG]modelview:\n");
    //for (int i = 0; i < modelview.rows; i++) {
    //    printf("[");
    //    for (int j = 0; j < modelview.cols; j++) {
    //        printf("%f ", m_get(&modelview, i, j));
    //    }
    //    printf("]\n");
    //}

}

void S_get_final_matrix(Shader *shader) {
    shader->final_matrix = 
        m_multiply(m_multiply(shader->viewport, shader->projection), shader->modelview);
    
    printf("[DEBUG]Final matrix:\n");
    for (int i = 0; i < shader->final_matrix.rows; i++) {
        printf("[");
        for (int j = 0; j < shader->final_matrix.cols; j++) {
            printf("%f ", m_get(&shader->final_matrix, i, j));
        }
        printf("]\n");
    }
}

Vec4f S_vertex_shader(Shader *shader, tobj_model *model, int iface, int nthvert) {
    // get nth vert of face i, and extend to 4 dimension
    Vec4f vertex = vec3f_to_4f(tobj_get_vert_from_face(model, iface, nthvert), 1);
    vertex = m_multiply_vec4f(shader->final_matrix, vertex);
    // printf("[DEBUG]In vertex shader, vertex(4f): (%f, %f, %f, %f)\n", 
            // vertex.x, vertex.y, vertex.z, vertex.w);

    vec3f_set(&shader->varying_intensity, nthvert,
            fmaxf(0, vec3f_multiply_v(tobj_get_normal(model, iface, nthvert), shader->light_dir)));
    
    return vertex;
}
    

bool S_fragment_shader(Shader *shader, Vec3f bar, tt_color *color) {
    float intensity = vec3f_multiply_v(shader->varying_intensity, bar);
    color->r = 0xFF * intensity;
    color->g = 0xFF * intensity;
    color->b = 0xFF * intensity;
    color->a = 0xFF;
    // printf("[DEBUG]intensity: %f, color: (A:%u R:%u G:%u B:%u)\n", 
            // intensity, color->a, color->r, color->g, color->b);
    return false;
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

int tcount = 0;
int dcount = 0;

void S_draw_triangle(Shader *shader, tt_image *image, tobj_model *model,
        Vec4f *pts, int *zbuffer) {
    // FIX: 渲染的点数和三角形数量错误
    tcount++;
    // printf("[DEBUG]In draw_traingle\n");
    Vec2f bboxmin = { .x = FLT_MAX, .y = FLT_MAX };
	Vec2f bboxmax = { .x = -FLT_MAX, .y = -FLT_MAX };

    // calculate bounding boxes
	for (int i = 0; i < 3; i++) {
		bboxmin.x = fminf(bboxmin.x, pts[i].x/pts[i].w);
		bboxmin.y = fminf(bboxmin.y, pts[i].y/pts[i].w);

		bboxmax.x = fmaxf(bboxmax.x, pts[i].x/pts[i].w);
		bboxmax.y = fmaxf(bboxmax.y, pts[i].y/pts[i].w);
	}
    // printf("[DEBUG]bboxmax (%f, %f) bboxmin (%f, %f)\n", 
    //         bboxmax.x, bboxmax.y, bboxmin.x, bboxmin.y);
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
            tt_color buffer_color = { 
                .b = (uint8_t)fmaxf(0, fminf(255, (float)(int)(z/w+.5))),
                .g = 0,
                .r = 0,
                .a = 0xFF
            };
            if (c.x<0 || c.y<0 || c.z<0 || zbuffer[P.y*image->width+P.x] > frag_depth) 
                continue;
            bool discard = S_fragment_shader(shader, c, &color);
            // printf("[DEBUG]Fragment shader return %s\n", discard ? "TRUE" : "FALSE");
            if (!discard) {
                assert(color.a == 0xFF);
                printf("[DUBUG]Drawing dot %d triangle %d\n", ++dcount, tcount);
                // tt_set_color(zbuffer, P.x, P.y, buffer_color);
                zbuffer[P.y*image->width+P.x] = frag_depth;
                tt_set_color(image, P.x, P.y, color);
            }
        }
    }
    
}




