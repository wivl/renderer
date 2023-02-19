#include "tinytga.h"
#include "geometry.h"
#include "matrix.h"
#include "tinyobj.h"


/* draw line */
void draw_line(tt_image* image, int w1, int h1, int w2, int h2, tt_color color);

void draw_triangle(tt_image *image, Vec3f *pts, float *zbuffer, tt_color color);

Vec3f world2screen(Vec3f v, const int width, const int height);

Matrix viewpoint(int x, int y, int w, int h, int depth);

void draw_triangle_texture(Vec3i t0, Vec3i t1, Vec3i t2,
        Vec2i uv0, Vec2i uv1, Vec2i uv2, tt_image *image,
        float intensity, int *zbuffer, tobj_model *model);


