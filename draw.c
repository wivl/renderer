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

void draw_line(tt_image* image, uint16_t w1, uint16_t h1, uint16_t w2, uint16_t h2, tt_color color) {
	_check_border(image, w1, h1, w2, h2);
	bool steep = false;
	if (abs(w1-w2)<abs(h1-h2)) { // if the line is steep, we transpose the image 
        _swap_u16(&w1, &h1); 
        _swap_u16(&w2, &h2); 
        steep = true; 
    }
	if (w1 > w2) {
		_swap_u16(&w1, &w2);
		_swap_u16(&h1, &h2);
	}
	int16_t dw = (int16_t)w2 - (int16_t)w1; 
	int16_t dh = (int16_t)h2 - (int16_t)h1;
	// printf("[DEV]dw: %d\tdh: %d\n", dw, dh);
	if (w1 == w2) {
		if (h1 > h2) {
			_swap_u16(&h1, &h2);
			_swap_u16(&w1, &w2);
		}
		for (uint16_t h = h1; h <= h2; h++) {
			tt_set_color(image, w1, h, color);
		}
	}
	float t = (float)dh / (float)dw;
	// printf("[DEV]t: %f\n", t);
	for (uint16_t w = w1; w <= w2; w++) {
		uint16_t h = h1 + t * (w-w1);
		// printf("[DEV]w: %u\th: %u\n", w, h);
		if (steep) {
			tt_set_color(image, h, w, color);
		} else {
			tt_set_color(image, w, h, color);
		}
	}
}

