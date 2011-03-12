#ifndef VGA_H
#define VGA_H

#include "common.h"

#define RGB_UINT32(r,g,b) (b+(g<<8)+(r<<16))
// x * bytes per pixel + y * bytes per line
#define XY_TO_OFFSET(x,y) (x*3+y*2400)

void putpixel(uint8_t* vram, uint32_t x, uint32_t y, uint32_t color);
void fillrect(uint8_t* vram, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
void drawline(uint8_t* vram, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color);
void draw_string(uint8_t* vram, uint32_t x, uint32_t y, char* input, uint32_t color);
void draw_char(uint8_t* vram, uint32_t x, uint32_t y, char character, uint32_t color);

#endif
