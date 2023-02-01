#include "draw.h"
#include "tinytga.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

void _check_border(tt_image* image, uint16_t w1, uint16_t h1, uint16_t w2, uint16_t h2) {
	assert(w1 <= image->width && h1 <= image->height);
	assert(w2 <= image->width && h2 <= image->height);
}

void _swap_u16(uint16_t *a, uint16_t *b) {
	uint16_t temp = *b;
	*b = *a;
	*a = temp;
}
void _swap_int(int *a, int *b) {
	int temp = *b;
	*b = *a;
	*a = temp;
}

void draw_line(tt_image* image, int w1, int h1, int w2, int h2, tt_color color) {
	_check_border(image, w1, h1, w2, h2);
	bool steep = false;
	if (abs(w1-w2)<abs(h1-h2)) { // if the line is steep, we transpose the image 
        _swap_int(&w1, &h1); 
        _swap_int(&w2, &h2); 
        steep = true; 
    }
	if (w1 > w2) {
		_swap_int(&w1, &w2);
		_swap_int(&h1, &h2);
	}
	int dw = w2 - w1; 
	int dh = h2 - h1;
	// printf("[DEV]dw: %d\tdh: %d\n", dw, dh);
	if (w1 == w2) {
		if (h1 > h2) {
			_swap_int(&h1, &h2);
			_swap_int(&w1, &w2);
		}
		for (int h = h1; h <= h2; h++) {
			tt_set_color(image, w1, h, color);
		}
	}
	float t = (float)dh / (float)dw;
	// printf("[DEV]t: %f\n", t);
	for (int w = w1; w <= w2; w++) {
		int h = h1 + t * (w-w1);
		// printf("[DEV]w: %u\th: %u\n", w, h);
		if (steep) {
			// printf("[DEV]w: %u h: %u\n", w, h);
			tt_set_color(image, h-1, w-1, color);
		} else {
			// printf("[DEV]w: %u h: %u\n", w, h);
			tt_set_color(image, w-1, h-1, color);
		}
	}
}

void line(int x0, int y0, int x1, int y1, tt_image *image, tt_color color) {
    bool steep = false;
    if (abs(x0-x1)<abs(y0-y1)) {
        _swap_int(&x0, &y0);
        _swap_int(&x1, &y1);
        steep = true;
    }
    if (x0>x1) {
        _swap_int(&x0, &x1);
        _swap_int(&y0, &y1);
    }

    for (int x=x0; x<=x1; x++) {
        float t = (x-x0)/(float)(x1-x0);
        int y = y0*(1.-t) + y1*t;
        if (steep) {
            tt_set_color(image, y-1, x-1, color);
        } else {
            tt_set_color(image, x-1, y-1, color);
        }
    }
}
