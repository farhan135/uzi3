#ifndef _STDGR_H
#define _STDGR_H

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define SCREEN_MAX_CHAR 2000
#define SCREEN_MAX_MEM 3840

#define BLACK          0x00
#define BLUE           0x01
#define GREEN          0x02
#define CYAN           0x03
#define RED            0x04
#define MAGENTA        0x05
#define BROWN          0x06
#define LIGHT_GRAY     0x07
#define DARK_GRAY      0x08
#define BRIGHT_BLUE    0x09
#define BRIGHT_GREEN   0x0A
#define BRIGHT_CYAN    0x0B
#define BRIGHT_RED     0x0C
#define BRIGHT_MAGENTA 0x0D
#define BRIGHT_YELLOW  0x0E
#define BRIGHT_WHITE   0x0F

#define FOREGROUND BRIGHT_WHITE
#define BACKGROUND BRIGHT_RED
//#define COLOR FOREGROUND | (BACKGROUND << 4)

#include"/home/farhan/Desktop/myOS_UZI/uzi3/header/stdtype.h"

uint8_t xCursor,yCursor;

void setBkgColor(uint8_t color);
void setCharColor(uint8_t color);
void gotoXY(uint8_t x,uint8_t y);
void clearScreen();
void clearAtScreen(uint8_t x, uint8_t y);
void putc(uint8_t ch);
void puts(char *msg);
void updateCursor(uint8_t y,uint8_t x);
void putcXY(uint8_t ch,uint8_t x,uint8_t y);
void putsXY(char *msg,uint8_t x,uint8_t y);

#endif
