#include <stddef.h>
#include <stdint.h>
#include "header/k_string.h"

#if defined(__linux__)
#error "This code must be compiled with a cross-compiler"
#elif !defined(__i386__)
#error "This code must be compiled with an x86-elf compiler"
#endif

#define COL(b, f) (uint8_t)((b << 4) | f)
#define put_c(c) put_char(c, row, col)
#define put_s(s) put_string(s, row, col)

volatile uint16_t *vga_buffer = (uint16_t *)0xB8000;
// Entries in the VGA buffer take the binary form BBBBFFFFCCCCCCCC, where:
// - B is the background color
// - F is the foreground color
// - C is the ASCII character
const int VGA_COLS = 80;
const int VGA_ROWS = 25;

int asm_test();

enum vga_color
{
    BLACK = 0x00,
    BLUE = 0x01,
    GREEN = 0x02,
    CYAN = 0x03,
    RED = 0x04,
    MAGENTA = 0x05,
    BROWN = 0x06,
    WHITE = 0x07,
    GRAY = 0x08,
    LIGHT_BLUE = 0x09,
    LIGHT_GREEN = 0x0A,
    LIGHT_CYAN = 0x0B,
    LIGHT_RED = 0x0C,
    LIGHT_MAGENTA = 0x0D,
    YELLOW = 0x0E,
    BRIGHT_WHITE = 0x0F
};

int col = 0;
int row = 0;
uint8_t color = COL(BLACK, BRIGHT_WHITE);

void clear_term()
{
    for (int c = 0; c < VGA_COLS; c++)
    {
        for (int r = 0; r < VGA_ROWS; r++)
        {
            const size_t index = (VGA_COLS * r) + c;
            vga_buffer[index] = ((uint16_t)color << 8) | ' ';
        }
    }
}

void put_char(char ch, int r, int c)
{
    switch (ch)
    {
    case '\n':
        row++;
        col = 0;
        break;
    default:;
        const size_t index = (VGA_COLS * r) + c;
        vga_buffer[index] = ((uint16_t)color << 8) | ch;
        col++;
        break;
    }
    if (col >= VGA_COLS)
    {
        col = 0;
        row++;
    }
    if (row >= VGA_ROWS)
    {
        col = 0;
        row = 0;
    }
}

void put_string(const char *str, int r, int c)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        put_c(str[i]);
    }
}

void kernal_main_func()
{
    clear_term();

    put_s("Hello, World!\n");
    put_c('0'+k_strlen("Test"));
    for (;;)
    {
        //put_c((row * VGA_COLS) + col);
    }
}