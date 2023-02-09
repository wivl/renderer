#include "tinytga.h"
#include "geometry.h"


/* draw line */
void draw_line(tt_image* image, int w1, int h1, int w2, int h2, tt_color color);

void draw_triangle(tt_image *image, Vec3f *pts, float *zbuffer, tt_color color);

Vec3f world2screen(Vec3f v, const int width, const int height);
