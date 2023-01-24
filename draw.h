#include "tinytga.h"

typedef struct {
	uint16_t w;
	uint16_t h;
} Point;

/* draw line */
void draw_line(tt_image* image, Point start, Point end, tt_color color);
