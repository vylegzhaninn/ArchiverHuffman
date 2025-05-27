#include "out.h"

int utf8_strlen(const char* str) {
    int len = 0;
    while (*str) {
        if ((*str & 0xC0) != 0x80)
            len++;
        str++;
    }
    return len;
}


void print_border(int width, int mode) {
    if (mode)
        printf("┏");
    else 
        printf("┗");
    for (int i = 0; i < width - 2; i++) printf("━");
    if (mode)
        printf("┓\n");
    else   
        printf("┛\n");
}

void print_line(const char* text, int width) {
    printf("┃ %s", text);
    int len = utf8_strlen(text);
    int spaces = width - 3 - len;
    for (int i = 0; i < spaces; i++) printf(" ");
    printf("┃\n");
}