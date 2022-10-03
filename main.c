#include <stdio.h>
#include <windows.h>

#include "./termInterface.c"
#include "./textures.c"

int main() {
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

    termInterface* term = termInterface_new(columns, 10, 3);
    printf("0\n");
//    termInterface_clear(term);
    printf("1\n");
//    printf("\033[1;1H\033[2J");
    printf("a\n");
    for (int i = 0; i < term->width * term->height; i++) {
        term->buffer[i] = '#';
        int x = i % term->width,
            y = i / term->width;
        texture_SetPixel(term->texture, x, y, 0x00FF00);
    }
    printf("b\n");
    termInterface_print(term);
    printf("c\n");
    termInterface_free(term);
    return 0;
}