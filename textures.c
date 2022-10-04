#ifndef TEXTURES_LIB
#define TEXTURES_LIB

#include <math.h>
#include <stdio.h>

typedef struct {
    int width, height,
        colorLength; // 1 alpha, 2 why not, 3 rgb, 4 rgba
    int* buffer;
} texture;

typedef struct {
    int red, green, blue, alpha;
} color;

int colorToInt(color clr, int colorLength) {
    if (colorLength >= 1) return clr.alpha;
    if (colorLength >= 2) return clr.alpha << 8 | clr.red;
    if (colorLength >= 3) return clr.red << 16 | clr.green << 8 | clr.blue;
    if (colorLength >= 4) return clr.alpha << 24 | clr.red << 16 | clr.green << 8 | clr.blue;
    return 0;
}

texture* texture_new(int width, int height, int colorLength) {
    texture* this = malloc(sizeof(texture));
    this->width = width;
    this->height = height;
    this->colorLength = colorLength;
    this->buffer = malloc(width * height * colorLength * sizeof(int));
    return this;
}

int texture_SetPixel(texture* this, int x, int y, int color) {
    if (x < 0 || x >= this->width || y < 0 || y >= this->height) return 1;
    int i = (y * this->width + x) * this->colorLength;
    // color 0xAA
    if (this->colorLength == 1) this->buffer[i] = color;
    // color 0xAABB
    if (this->colorLength == 2) {
        this->buffer[i] = color >> 8;
        this->buffer[i + 1] = color;
    }
    // color 0xRRGGBB
    if (this->colorLength == 3) {
        this->buffer[i] = color >> 16;
        this->buffer[i + 1] = color >> 8;
        this->buffer[i + 2] = color;
    }
    // color 0xRRGGBBAA
    if (this->colorLength == 4) {
        this->buffer[i] = color >> 24;
        this->buffer[i + 1] = color >> 16;
        this->buffer[i + 2] = color >> 8;
        this->buffer[i + 3] = color;
    }
    return 0;
}

color texture_GetPixel(texture* this, int x, int y) {
    color color = {0, 0, 0, 0};
    if (x < 0 || x >= this->width || y < 0 || y >= this->height) return color;
    int i = (y * this->width + x) * this->colorLength;
    // color 0xAA
    if (this->colorLength == 1) {
        color.alpha = this->buffer[i];
        color.red = this->buffer[i];
        color.green = this->buffer[i];
        color.blue = this->buffer[i];
    }
    // color 0xAABB
    if (this->colorLength == 2) {
        color.alpha = this->buffer[i];
        color.red = this->buffer[i + 1];
        color.green = this->buffer[i + 1];
        color.blue = this->buffer[i + 1];
    }
    // color 0xRRGGBB
    if (this->colorLength == 3) {
        color.red = this->buffer[i];
        color.green = this->buffer[i + 1];
        color.blue = this->buffer[i + 2];
    }
    // color 0xRRGGBBAA
    if (this->colorLength == 4) {
        color.alpha = this->buffer[i];
        color.red = this->buffer[i + 1];
        color.green = this->buffer[i + 2];
        color.blue = this->buffer[i + 3];
    }
    return color;
}

void texture_free(texture* this) {
    free(this->buffer);
    free(this);
}

typedef struct {
    float x, y;
} vector2;

// return gradient texture



float gradientXY(int x, int y, int w, int h) {

}

// perlin noise
// from wikipedia https://en.wikipedia.org/wiki/Perlin_noise
/* Function to linearly interpolate between a0 and a1
 * Weight w should be in the range [0.0, 1.0]
 */
float interpolate(float a0, float a1, float w) {
    /* // You may want clamping by inserting:
     * if (0.0 > w) return a0;
     * if (1.0 < w) return a1;
     */
    return (a1 - a0) * w + a0;
    /* // Use this cubic interpolation [[Smoothstep]] instead, for a smooth appearance:
     * return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
     *
     * // Use [[Smootherstep]] for an even smoother result with a second derivative equal to zero on boundaries:
     * return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
     */
}

/* Create pseudorandom direction vector
 */
vector2 randomGradient(int ix, int iy, float seed) {
    // No precomputed gradients mean this works for any number of grid coordinates
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2; // rotation width
    unsigned a = ix, b = iy;
    a *= 3284157443; b ^= a << s | a >> w-s;
    b *= 1911520717; a ^= b << s | b >> w-s;
    a *= 2048419325;
    float random = a * (seed / 255.0);
    vector2 v;
    v.x = cos(random); v.y = sin(random);
    return v;
}

// Computes the dot product of the distance and gradient vectors.
float dotGridGradient(int ix, int iy, float x, float y, float seed) {
    // Get gradient from integer coordinates
    vector2 gradient = randomGradient(ix, iy, seed);

    // Compute the distance vector
    float dx = x - (float)ix;
    float dy = y - (float)iy;

    // Compute the dot-product
    return (dx*gradient.x + dy*gradient.y);
}

// Compute Perlin noise at coordinates x, y
float perlinXY(float x, float y, float seed) {
    // Determine grid cell coordinates
    int x0 = (int)floor(x),
        x1 = x0 + 1,
        y0 = (int)floor(y),
        y1 = y0 + 1;

    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    float sx = x - (float)x0,
          sy = y - (float)y0;

    // Interpolate between grid point gradients
    float n0, n1, ix0, ix1, value;

    n0 = dotGridGradient(x0, y0, x, y, seed);
    n1 = dotGridGradient(x1, y0, x, y, seed);
    ix0 = interpolate(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y, seed);
    n1 = dotGridGradient(x1, y1, x, y, seed);
    ix1 = interpolate(n0, n1, sx);

    value = interpolate(ix0, ix1, sy);
    return value; // Will return in range -1 to 1. To make it in range 0 to 1, multiply by 0.5 and add 0.5
}

texture* perlin(int width, int height, float seed) {
    texture *tex;
    tex->width = width;
    tex->height = height;
    tex->colorLength = 1;
    tex->buffer = malloc(width * height * tex->colorLength * sizeof(char));
    for (int i = 0; i < width * height * tex->colorLength; i += tex->colorLength) {
        float x = (float)(i / tex->colorLength % width) / width;
        float y = (float)(i / tex->colorLength / width) / height;
        float value = perlinXY(x, y, seed);
        tex->buffer[i] = (int)(value * 255);
    }
    return tex;
}
#endif