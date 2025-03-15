#include "../include/myTerm.h"

// Очистка экрана
int mt_clrscr(void) {
    fflush(stdout);
    if (write(1, "\033[H\033[2J", 8) == -1) {
        return -1;
    }
    return 0;
}

// Перемещение курсора на позицию (x, y)
int mt_gotoXY(int x, int y) {
    if (!(x >= 0 && y >= 0)) {
        return -1;
    }

    fflush(stdout);

    int length = snprintf(NULL, 0, "\033[%d;%dH", x + 1, y + 1);
    if (length < 0) {
        return -1;
    }
ё
    char *escp = malloc(length + 1); 
    if (!escp) {
        return -1;
    }ё

    snprintf(escp, length + 1, "\033[%d;%dH", x + 1, y + 1);

    if (write(1, escp, length) == -1) {
        free(escp);
        return -1;
    }

    fflush(stdout); 
    free(escp);
    return 0;
}

// Получение размера экрана
int mt_getscreensize(int *rows, int *cols) {
    struct winsize ws;

    if (!ioctl(1, TIOCGWINSZ, &ws) && rows && cols) {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    } else {
        return -1;
    }
}

// Установка цвета фона
int mt_setbgcolor(enum colors color) {
    if (color < 0 || color >= COLORS_COUNT) {  // Проверка границ
        return -1;
    }
    if (write(1, COLORS_BG_STORAGE[color], 5) == -1) {  
        return -1;
    }
    return 0;
}

// Установка цвета текста
int mt_setfgcolor(enum colors color) {
    if (color < 0 || color >= COLORS_COUNT) {  // Проверка границ
        return -1;
    }
    if (write(1, COLORS_TEXT_STORAGE[color], 5) == -1) {  
        return -1;
    }
    return 0;
}

// Сброс цвета к стандартному
int mt_setdefaultcolor(void) {
    if (write(1, "\033[0m", 4) == -1) {  
        return -1;
    }
    return 0;
}

// Управление видимостью курсора
int mt_setcursorvisible(int value) {
    if (value != 0 && value != 1) {  
        return -1;
    }
    const char *sequence = value ? "\033[?25h" : "\033[?25l";  
    if (write(1, sequence, 6) == -1) {  
        return -1;
    }
    return 0;
}

// Удаление текущей строки
int mt_delline(void) {
    if (write(1, "\033[K", 3) == -1) {  
        return -1;
    }
    return 0;
}
