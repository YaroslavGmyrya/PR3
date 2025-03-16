#pragma once

#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MIN_W 80
#define MIN_H 80
#define MAX_LINES 10

enum colors {
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    PURPLE,
    LIGHT_BLUE,
    WHITE,
    LIGHT_BLACK,
    LIGHT_RED,
    LIGHT_GREEN,
    LIGHT_YELLOW,
    LIGHT_BLUE_HIGH,
    LIGHT_PURPLE,
    LIGHT_WHITE,
    COLORS_COUNT
};

typedef struct {
    int address;
    char type;
    int value;
} IOEntry;



extern IOEntry io_log[MAX_LINES]; 
extern char* COLORS_TEXT_STORAGE[];
extern char* COLORS_BG_STORAGE[];

int mt_clrscr(void);
int mt_gotoXY(int x, int y);
int mt_getscreensize(int *rows, int *cols);
int mt_setbgcolor(enum colors color);
int mt_setfgcolor(enum colors color);
int mt_setdefaultcolor(void);
int mt_setcursorvisible(int value);
int mt_delline(void);


