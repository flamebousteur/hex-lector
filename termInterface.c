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
    // buffer is a 2D array of characters (256 characters, 256 colors | rgb (256, 256, 256))
    this->buffer = malloc(width * height);
    return this;
}

void termInterface_free(termInterface* this) {
    free(this->buffer);
    free(this);
}

void termInterface_clear(termInterface* this) {
    for (int i = 0; i < this->width * this->height * 2; i++) this->buffer[i] = 0;
}

void termInterface_print(termInterface* this) {
    // print the buffer
    printf("\n0\n");
    for (int i = 0; i < this->width * this->height; i++) {
        // line break
        if (i % this->width == 0) printf("\n");
        // set color
        printf("\033[38;2;%d;%d;%dm", this->texture->buffer[i * 3], this->texture->buffer[i * 3 + 1], this->texture->buffer[i * 3 + 2]);
        // print character
        printf("%c", this->buffer[i]);
    }
    // reset color
    printf("\033[0m");
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
#endif