#include <stddef.h>
#include <stdint.h>

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
    const uint16_t clear = ((uint16_t)color << 8) | ' ';
    for (int c = 0; c < VGA_COLS; c++)
    {
        for (int r = 0; r < VGA_ROWS; r++)
        {
            const size_t index = (VGA_COLS * r) + c;
            vga_buffer[index] = clear;
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
    int j = 0;
    clear_term();
    for (;;)
    {
        for (int i = 0; i < 0xFFFFA; i++)
        {
            color = COL(GRAY, WHITE);
            const uint16_t t_char = ((uint16_t)color << 8) | 'M';
            vga_buffer[0] = t_char;
            vga_buffer[(VGA_ROWS - 1) * VGA_COLS] = t_char;
            vga_buffer[VGA_COLS - 1] = t_char;
            vga_buffer[VGA_COLS * VGA_ROWS - 1] = t_char;
            vga_buffer[(VGA_COLS * VGA_ROWS - 1) / 2] = t_char;
        }
        switch (j % 16)
        {
        case 0:
            color = COL(BLACK, WHITE);
            break;
        case 1:
            color = COL(BLUE, WHITE);
            break;
        case 2:
            color = COL(GREEN, WHITE);
            break;
        case 3:
            color = COL(CYAN, WHITE);
            break;
        case 4:
            color = COL(RED, WHITE);
            break;
        case 5:
            color = COL(MAGENTA, WHITE);
            break;
        case 6:
            color = COL(BROWN, WHITE);
            break;
        case 7:
            color = COL(WHITE, WHITE);
            break;
        case 8:
            color = COL(GRAY, WHITE);
            break;
        case 9:
            color = COL(LIGHT_BLUE, WHITE);
            break;
        case 10:
            color = COL(LIGHT_GREEN, WHITE);
            break;
        case 11:
            color = COL(LIGHT_CYAN, WHITE);
            break;
        case 12:
            color = COL(LIGHT_RED, WHITE);
            break;
        case 13:
            color = COL(LIGHT_MAGENTA, WHITE);
            break;
        case 14:
            color = COL(YELLOW, WHITE);
            break;
        case 15:
            color = COL(BRIGHT_WHITE, WHITE);
            break;
        }
        clear_term();
        j++;
        //put_c((row * VGA_COLS) + col);
    }
}