#include "draw.h"
#include "tinytga.h"
/* define TINYOBJ_LOADER_C_IMPLEMENTATION for only *one* .c */

#define IMAGE_WIDTH 1920
#define IMAGE_HEIGHT 1080

int main(void) {
	tt_color pink = tt_get_color(0xFF986BEB);
	tt_color black = tt_get_color(0xFF000000);
	tt_image* image = tt_create(IMAGE_WIDTH, IMAGE_HEIGHT, pink);
	draw_line(image, 0, 0, 50, 1000, black);
	// draw_line(image, 500, 500, 600, 500, black);
	tt_save(image, "result.tga");
	return 0;
}
