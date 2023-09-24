#include <drivers/screen.hpp>
#include <inttypes.h>

#define SCREEN_COLOR_CODE 0x07

uint8_t x = 0;
uint8_t y = 0;

void screenClear()
{
    uint16_t *screen = (uint16_t *) 0xB8000;

    x = 0;
    y = 0;

    for (uint16_t i=0; i < 80*25; i++) {
        screen[i] = 0;
    }
}

void screenPrintChar(char c)
{
    uint16_t *screen = (uint16_t *) 0xB8000;

    if (++x == 80) {
        x = 0;
        y++;
    }
    
    if (y == 25) {
        y = 0;
        screenClear();
    }

    if (c == '\n') {
        x = 0;
        y++;
    } else {
        screen[y * 80 + x] = c | (SCREEN_COLOR_CODE << 8);
    }
}

void screenPrint(char *str)
{
    while(*str != '\0') {
        screenPrintChar(*str++);       
    }
}