#include "vga.h"
#include "common.h"
#include "math.h"
#include "vincent.h" // Our font

/* only valid for 800x600x16M */
void putpixel(uint8_t* vram, uint32_t x, uint32_t y, uint32_t color) {
    unsigned where = XY_TO_OFFSET(x,y);
    vram[where] = color & 255;              // BLUE
    vram[where + 1] = (color >> 8) & 255;   // GREEN
    vram[where + 2] = (color >> 16) & 255;  // RED
}

void fillrect(uint8_t* vram, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    unsigned char *where = vram+XY_TO_OFFSET(x,y);
    int i, j;
 
    for (i = 0; i < w; i++) {
        for (j = 0; j < h; j++) {
			where[j*3] = color & 255;              // BLUE
			where[j*3 + 1] = (color >> 8) & 255;   // GREEN
			where[j*3 + 2] = (color >> 16) & 255;  // RED
        }
        where+=2400;
    }
}

void drawline(uint8_t* vram, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color) {
	uint32_t dx = abs(x1-x0);
	uint32_t dy = abs(y1-y0);
	uint8_t sx,sy = -1;
    if (x0 < x1) {sx = 1;}
    if (y0 < y1) {sy = 1;}
    uint32_t err = dx-dy;
    uint32_t err2;
    
    while((x0 != x1) & (y0 != y1)) {
		putpixel(vram, x0, y0, color);
		err2 = 2*err;
		if (err2 > -dy) {
			err = err - dy;
			x0 = x0+sx;
		}
		if (err2 < dx) {
			err = err + dx;
			y0 = y0+sy;
		}
	}
}

void draw_string(uint8_t* vram, uint32_t x, uint32_t y, char* input, uint32_t color) {
    vram += XY_TO_OFFSET(x,y);
    while(*input) {
        draw_char(vram, 0, 0, (char)*input, color);
        vram += 8*3;
        input++;
    }
}

void draw_char(uint8_t* vram, uint32_t x, uint32_t y, char character, uint32_t color) {
    int row;
    int column;
    vram += XY_TO_OFFSET(x,y);
 
    for (row = 0; row < 8; row++) {
		for (column = 0; column < 8; column++) {
			uint8_t out = vincent_data[(uint8_t)character][row];
			if (out<<column & 0x01<<column) {
				*(uint32_t *)vram = color;
			}
			vram += 3;
		}
		vram += 2400;
    }
}
