#ifndef TERMINTERFACE_LIB
#define TERMINTERFACE_LIB

#include <stdio.h>
#include <stdlib.h>

#include "./textures.c"

typedef struct termInterface {
    int width,
        height,
        pointerX,
        pointerY;
    char* buffer;
    texture* texture;
} termInterface;

termInterface* termInterface_new(int width, int height, int colorLength) {
    termInterface* this = malloc(sizeof(termInterface));
    this->width = width;
    this->height = height;
    this->pointerX = 0;
    this->pointerY = 0;
    // buffer is a 2D array of characters (256 characters)
    this->buffer = malloc(width * height);
    this->texture = texture_new(width, height, colorLength);
    return this;
}

void termInterface_free(termInterface* this) {
    free(this->buffer);
    texture_free(this->texture);
    free(this);
}

void termInterface_clear(termInterface* this) {
    for (int i = 0; i < this->width * this->height * 2; i++) this->buffer[i] = 0;
}

void termInterface_print(termInterface* this) {
    // print the buffer
    for (int i = 0; i < this->width * this->height; i++) {
        // line break
        if (i % this->width == 0) printf("\n");
        // set color
        int x = i % this->texture->width,
            y = i / this->texture->width;
        color clr = texture_GetPixel(this->texture, x, y);
        printf("\033[38;2;%d;%d;%dm", clr.red, clr.green, clr.blue);
        // print character
        printf("%c", this->buffer[i]);
    }
    // reset color
    printf("\033[0m");
}

void termInterface_setPointer(termInterface* this, int x, int y) {
    this->pointerX = x;
    this->pointerY = y;
}

void termInterface_setChar(termInterface* this, int x, int y, char character, int color) {
    if (x < 0 || x >= this->width || y < 0 || y >= this->height) return;
    int i = (y * this->width + x);
    this->buffer[i] = character;
    texture_SetPixel(this->texture, x, y, color);
}

void termInterface_setString(termInterface* this, int x, int y, char* string, int color) {
    for (int i = 0; string[i] != 0; i++) termInterface_setChar(this, x + i, y, string[i], color);
}

void termInterface_setStringCentered(termInterface* this, int y, char* string, int color) {
    int x = this->width / 2 - strlen(string) / 2;
    termInterface_setString(this, x, y, string, color);
}

void termInterface_setStringCenteredX(termInterface* this, int x, int y, char* string, int color) {
    x -= strlen(string) / 2;
    termInterface_setString(this, x, y, string, color);
}

void termInterface_setStringCenteredY(termInterface* this, int x, int y, char* string, int color) {
    y -= strlen(string) / 2;
    termInterface_setString(this, x, y, string, color);
}

void termInterface_setFooter(termInterface* this, char* string, int color) {
    termInterface_setString(this, 0, this->height - 1, string, color);
}

void termInterface_setRectangle(termInterface* this, int x, int y, int width, int height, char character, int color) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            termInterface_setChar(this, x + i, y + j, character, color);
        }
    }
}

void termInterface_setLine(termInterface* this, int x1, int y1, int x2, int y2, char chr, int color) {
    // Bresenham's line algorithm
    int dx = abs(x2 - x1),
        sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1),
        sy = y1 < y2 ? 1 : -1;
    int err = dx + dy,
        e2;
    while (1) {
        termInterface_setChar(this, x1, y1, chr, color);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void termInterface_setRectangleLines(termInterface* this, int x1, int y1, int x2, int y2, char chr, int color) {
    termInterface_setLine(this, x1, y1, x2, y1, chr, color);
    termInterface_setLine(this, x2, y1, x2, y2, chr, color);
    termInterface_setLine(this, x2, y2, x1, y2, chr, color);
    termInterface_setLine(this, x1, y2, x1, y1, chr, color);
}

#endif