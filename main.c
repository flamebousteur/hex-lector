#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "./termInterface.c"
#include "./textures.c"
#include "./input.c"

typedef struct {
    int length;
    char* buffer;
} fl;

fl getFileContent(char* filename, long start, long offset) {
    fl result;
    FILE* file = fopen(filename, "rb");
    // if offset goes beyond the end of the file, set it to the end of the file
    if (start + offset > filelength(fileno(file))) offset = filelength(fileno(file)) - start;
    result.buffer = malloc(offset);
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    } else {
        fseek(file, start, SEEK_SET);
        fread(result.buffer, offset, 1, file);
        fclose(file);
    }
    return result;
}

int main(int argc, char** argv) {
    // active the ascii escape sequences for windows 10
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(console, &consoleMode);
    consoleMode |= 0x0004;
    SetConsoleMode(console, consoleMode);

    // terminal size
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    printf("columns: %d, rows: %d\n", columns, rows);

    termInterface* term = termInterface_new(columns, rows, 3);
    termInterface_clear(term);
    for (int i = 0; i < term->width * term->height; i++) {
        term->buffer[i] = ' ';
        int x = i % term->width,
            y = i / term->width;
        // gradient color red to green
        int rgb = 0x000000;
        texture_SetPixel(term->texture, x, y, rgb);
    }
    // print the file content at the center of the screen (print the file in hex format)
    fl file = getFileContent("main.c", 0, 10);
    for (int i = 0; i < file.length; i++) {
        int x = i % 16,
            y = i / 16;
        termInterface_setChar(term, x, y, file.buffer[i], 0xFFFFFF);
    }
    printf("\033[1;1H\033[2J");
    termInterface_print(term);
    termInterface_free(term);
    return 0;
}