#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>  // Добавлен для использования write()
#include <stdlib.h>  // Добавлен для использования asprintf()
#include <string.h>


#define MIN_W 80
#define MIN_H 80
#define BIGCHAR_WIDTH 8  // Ширина "большого символа" в битах 
#define BIGCHAR_HEIGHT 8 // Высота "большого символа" в битах
#define MAX_LINES 10  
#define MAX_STR_LEN 20 
#define P 0x1 
#define Z 0x2 
#define M 0x4 
#define T 0x8 
#define E 0x10 
#define SIZE 128
#define ACS_CKBOARD 'a'
//static const int SIZE = 128; 
const int SIZE_COMMAND = 15; 
int memory[SIZE]; 
 
int accumulator; 
int command_counter; 
int flags_register; 
 
enum colors{
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    PURPLE,
    LIGHT_BLUE,  // Голубой
    WHITE,
    LIGHT_BLACK,
    LIGHT_RED,
    LIGHT_GREEN,
    LIGHT_YELLOW,
    LIGHT_BLUE_HIGH,  // Голубой светлый (исправлено дублирование)
    LIGHT_PURPLE,
    LIGHT_WHITE,
    COLORS_COUNT  // Количество цветов (для проверки границ)
};

static const char* COLORS_TEXT_STORAGE[] = {
    "\033[30m", "\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m", "\033[37m",
    "\033[90m", "\033[91m", "\033[92m", "\033[93m", "\033[94m", "\033[95m", "\033[96m", "\033[97m"
};

// Хранилище escape-последовательностей для цветов фона
static const char* COLORS_BG_STORAGE[] = {
    "\033[40m", "\033[41m", "\033[42m", "\033[43m", "\033[44m", "\033[45m", "\033[46m", "\033[47m",
    "\033[100m", "\033[101m", "\033[102m", "\033[103m", "\033[104m", "\033[105m", "\033[106m", "\033[107m"
};

// Очистка экрана
int mt_clrscr(void) {
    if (write(1, "\033[H\033[2J", 7) == -1) {
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
    char *escp;
    if (asprintf(&escp, "\033[%d;%dH", x + 1, y + 1) == -1) {  // Исправлено: терминалы используют 1-based индексы
        return -1;
    }
    if (write(1, escp, strlen(escp)) == -1) {
        free(escp);  // Освобождаем память, выделенную asprintf
        return -1;
    }

    free(escp);  // Освобождаем память
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
    if (write(1, COLORS_BG_STORAGE[color], 5) == -1) {  // Исправлено: длина escape-последовательности
        return -1;
    }
    return 0;
}

// Установка цвета текста
int mt_setfgcolor(enum colors color) {
    if (color < 0 || color >= COLORS_COUNT) {  // Проверка границ
        return -1;
    }
    if (write(1, COLORS_TEXT_STORAGE[color], 5) == -1) {  // Исправлено: длина escape-последовательности
        return -1;
    }
    return 0;
}

// Сброс цвета к стандартному
int mt_setdefaultcolor(void) {
    if (write(1, "\033[0m", 4) == -1) {  // Исправлено: длина escape-последовательности
        return -1;
    }
    return 0;
}

// Управление видимостью курсора
int mt_setcursorvisible(int value) {
    if (value != 0 && value != 1) {  // Исправлено: логика проверки
        return -1;
    }
    const char *sequence = value ? "\033[?25h" : "\033[?25l";  // Включение/выключение курсора
    if (write(1, sequence, 6) == -1) {  // Исправлено: длина escape-последовательности
        return -1;
    }
    return 0;
}

// Удаление текущей строки
int mt_delline(void) {
    if (write(1, "\033[K", 3) == -1) {  // Исправлено: длина escape-последовательности
        return -1;
    }
    return 0;
}

int sc_memoryInit(void) { 
    for (int i = 0; i < SIZE; i++) { 
        memory[i] = 0; 
    } 
    return 0; 
} 
 
int sc_memorySet(int address, int value) { 
    if (address >= SIZE && value >= 0x800) { 
        return -1; 
    } 
 
    memory[address] = value; 
    return 0; 
} 
 
int sc_memoryGet(int address, int *value) { 
    if (address >= SIZE || address < 0 || !value) { 
        return -1; 
    } 
 
    *value = memory[address]; 
    return 0; 
} 
 
int sc_memorySave(char *filename) { 

    if (!filename)
        return -1;

    FILE *file = fopen(filename, "wb"); 
 
    if (!file) { 
        return -1; 
    } 
 
    int result = fwrite(memory, sizeof(int), SIZE, file); 

    fclose(file); 
 
    if (result < SIZE) { 
        return -1; 
    } 
 
    return 0; 
} 
 
int sc_memoryLoad(char *filename) { 

     if (!filename)
        return -1;

    FILE *file = fopen(filename, "rb"); 
 
    if (!file) { 
        return -1; 
    } 
 
    int result = fread(memory, sizeof(int), SIZE, file); 

    fclose(file); 
 
    if (result < SIZE) { 
        return -1; 
    } 
 
    return 0; 
} 
 
int sc_regInit(void) { 

    flags_register = 0; 

    return 0; 
} 
 
int sc_regSet(int reg, int value) { 

   if ((reg != P && reg != Z && reg != M && reg != T && reg != E) || (value != 0 && value != 1)) {
    return -1;
    }

    if (value)
        flags_register |= reg;
    else
        flags_register &= ~reg;

    return 0; 
}
 
int sc_regGet(int reg, int *value) { 

    if ((reg != P && reg != Z && reg != M && reg != T && reg != E) || !value) {
        return -1;
    }

    if(flags_register & reg)
        *value = 1;

    else
        *value = 0;

    return 0; 
}
 
int sc_accumulatorInit(void) { 
    accumulator = 0; 
    return 0; 
} 
 
int sc_accumulatorSet(int value) { 
    if (value >= 0x800) {  // 0x800 = pow(2,15) 
        return -1; 
    } 
 
    accumulator = value; 
    return 0; 
} 
 
int sc_accumulatorGet(int *value) { 
    if (!value) { 
        return -1; 
    } 
 
    *value = accumulator; 
    return 0; 
} 
 
int sc_icounterInit(void) { 
    command_counter = 0; 
    return 0; 
} 
 
int sc_icounterSet(int value) { 
    if (value >= 0x800) {  // 0x800 = pow(2,15) 
        return -1; 
    } 
 
    command_counter = value; 
    return 0; 
} 
 
int sc_icounterGet(int *value) { 
    if (!value) { 
        return -1; 
    } 
 
    *value = command_counter; 
    return 0; 
} 
 
int sc_commandEncode(int sign, int command, int operand, int *value) {
    if (!value) return -1;

    if (sign != 0 && sign != 1) { 
        return -1; 
    } 

    if (command < 0 || command > 0x7F) { // 7 бит на команду
        return -1; 
    } 

    if (operand < 0 || operand > 0x7F) { // 7 бит на операнд
        return -1; 
    } 

    int result = 0;

    // Устанавливаем знак (бит 14)
    result |= (sign << 14); 

    // Устанавливаем команду (биты 13-7)
    result |= (command << 7);

    // Устанавливаем операнд (биты 6-0)
    result |= operand;

    *value = result; 
    return 0; 
}

int sc_commandDecode(int value, int *sign, int *command, int *operand) { 
    if (!sign || !operand) { 
        return -1; 
    } 

    // Получаем знак (бит 14)
    *sign = (value >> 14) & 1;

    // Получаем команду (биты 13-7)
    *command = (value >> 7) & 0x7F; 

    // Получаем операнд (биты 6-0)
    *operand = value & 0x7F; 

    return 0; 
}
 
int sc_commandValidate(int command) { 
    if (command >= 0x800) { 
        return -1; 
    } 
 
    return 0; 
} 
 
void printCell (int address, enum colors fg, enum colors bg) { 

    int row =  address / 10 + 1; 
    int col = (address % 10) * 5 + 1; 

    if (address >= 128 || address < 0) { 
        return; 
    } 

    mt_setfgcolor(fg);
    mt_setbgcolor(bg);
    mt_gotoXY(col, row);
    
    printf("%d\n", memory[address]); 

} 

 
void printFlags(void) {
    int value_P;
    int value_Z;
    int value_M;
    int value_T;
    int value_E;

    int rows;
    int cols;

    // Получаем размеры терминала
    mt_getscreensize(&rows, &cols);

    mt_gotoXY(160, 1);

    // Получаем значения флагов
    sc_regGet(P, &value_P);
    sc_regGet(Z, &value_Z);
    sc_regGet(M, &value_M);
    sc_regGet(T, &value_T);
    sc_regGet(E, &value_E);

    // Устанавливаем позицию для вывода флагов (например, в правом верхнем углу)
     // cols - 10 — отступ от правого края, 1 — первая строка

    // Выводим флаги
    value_P ? printf("P ") : printf("_ ");
    value_Z ? printf("Z ") : printf("_ ");
    value_M ? printf("M ") : printf("_ ");
    value_T ? printf("T ") : printf("_ ");
    value_E ? printf("E ") : printf("_ ");

    printf("\n");

    // Сбрасываем цвет и возвращаем курсор в начало
    mt_setdefaultcolor();
     // Возвращаем курсор в начало терминала
}
 
void printBinary(int number) { 
    for (int i = 14; i >= 0; i--) { 
        int bit = (number >> i) & 1; 
        printf("%d", bit); 
    } 
    printf("\n"); 
} 
 
void printDecodedCommand(int value) { 
    mt_gotoXY(1, 15);
    printf("DEC: %d | ", value); 
    printf("OCT: %o | ", value); 
    printf("HEX: %x   bin: ", value); 
    printBinary(value); 
} 
 
void printAccumulator(void) { 
    int value; 
    sc_accumulatorGet(&value);
    mt_gotoXY(130, 1); 
    printf("sc: %d hex: %x\n", value, value); 
} 

void sc_printCounter(void) {
    int value;

    sc_icounterGet(&value);

    mt_gotoXY(130, 2);

    printf("T: %d    IC: %d\n", value, value);

}

typedef struct {
    int address;
    char type;
    int value;
} IOEntry;

IOEntry io_log[MAX_LINES]; // Хранение последних операций

void addIOEntry(int address, char type, int value) {
    // Сдвигаем все строки вверх
    for (int i = 1; i < MAX_LINES; i++) {
        io_log[i - 1] = io_log[i];
    }
    
    // Добавляем новую строку в конец
    io_log[MAX_LINES - 1].address = address;
    io_log[MAX_LINES - 1].type = type;
    io_log[MAX_LINES - 1].value = value;
}

void printIOLog() {
    mt_gotoXY(133,20);
    printf("IN-OUT:\n");
    for (int i = 0, j = 1; i < MAX_LINES; i++) {
        if (io_log[i].type != 0) { // Печатаем только заполненные строки
            mt_gotoXY(133,20+j);
            printf("%03d %c %04X\n", io_log[i].address, io_log[i].type, io_log[i].value);
            j++;
        }
    }
}

void printCommand(void){
    int value;
    int sign;
    int command;
    int operand;
    mt_gotoXY(160, 15);
    if (command_counter < SIZE && command_counter >= 0){
        sc_memoryGet(command_counter,&value);
        sc_commandDecode(value, &sign, &command, &operand);

        if (sign){
            printf("- %x : %x\n", command, operand);
        }

        else{
            printf("+ %x : %x\n", command, operand);
        }
    }

    else{
        printf("! + FF : FF\n");
    }
    

}

int bc_strlen (char * str) {                                    // Латиница и цифры находятся в диапозоне U+0000 до U+007F и занимают 1 байт
    int count_char = 0;
    if (!str)                                                   // Кириллица символы и цифры находятся в диапозоне U+0400 до U+04FF и занимают 2 байт
        return -1;

    while (*str){

        if (*str & 0x80 == 0){                                   // 0x80 == 128 or 10000000 (узнаем восьмой бит) Латиница/цифра
            count_char++;
            str++;
        }  
        
        else if ((*str & 0xE0) == 0xC0) {                        // 0xE0 = 11100000, 0xC0 = 11000000 (узнаем первые 2 бита первого байта) Кириллица
            count_char++;
            str += 2;
        }   

        else
            return 0;                                              
    }

    return count_char;
}

int switch_table(void){
    char* switch_table = "\033(0";

    write(1,switch_table, strlen(switch_table));

    return 0;
}

int return_table(void){
    char* switch_table = "\033(B";

    write(1,switch_table, strlen(switch_table));

    return 0;
}


int bc_printA (char * str){

    if(!str){
        return -1;
    }

   switch_table();

   for(int i = 0; i < bc_strlen(str); i++){
        printf("%c ", str[i]);
   }
}

int bc_box(int x1, int y1, int x2, int y2, enum colors box_fg, enum colors box_bg, char *header, enum colors header_fg, enum colors header_bg){

   if (!(x2 > 1 && y2 > 1 && box_fg < COLORS_COUNT && box_bg < COLORS_COUNT && header_fg < COLORS_COUNT && header_bg < COLORS_COUNT)) {
    return -1;
}

mt_setbgcolor(box_bg);
mt_setfgcolor(box_fg);

char tl = 'l', tr = 'k', bl = 'm', br = 'j'; 
char hl = 'q', vl = 'x'; 

switch_table();

mt_gotoXY(x1,y1);

putchar(tl);

for(int i = 1; i < x2 - 1; i++)
    putchar(hl);
mt_gotoXY(x1,y1 + y2 -1);
putchar(tr);

for (int i = 1; i < x2 - 1; i++) {
    mt_gotoXY(x1 + i, y1);
    putchar(vl);

    mt_gotoXY(x1 + i, y1 + y2 - 1);
    putchar(vl);
}

mt_gotoXY(x1 + x2 -1,y1);

putchar(bl);

for(int i = 1; i < x2 - 1; i++)
    putchar(hl);

mt_gotoXY(x1 + x2 - 1,y1 + y2 - 1);
putchar(br);

if (header) {        
    int len = bc_strlen(header);
    if (len > y2 - 2) 
        len = y2 - 2;        
    int header_x = x1;
    int header_y = (y2 - len + 2)/2;        
    mt_setfgcolor(header_fg);
    mt_setbgcolor(header_bg);        
    mt_gotoXY(header_x, header_y);
    printf("%s", header);    
    }

return_table();

return 0;

}

int bc_setbigcharpos(int *big, int x, int y, int value) {
    if (x < 0  && y < 0 || y >= BIGCHAR_WIDTH && x >= BIGCHAR_WIDTH || !big || (value != 0 && value != 1)) { 
        return -1; 
    } 

    int index = x / 4; // Выбираем число (верхняя или нижняя часть)
    int bit_position = y + (x % 4) * 8; // Смещение в числе

    if (value) { 
        big[index] |= (1 << bit_position); // Устанавливаем бит
    } else { 
        big[index] &= ~(1 << bit_position); // Сбрасываем бит
    }

    return 0;
}

// Получает значение бита в большом символе
int bc_getbigcharpos(int *big, int x, int y, int *value) {
    if (x < 0 && y < 0 || y >= BIGCHAR_WIDTH && x >= BIGCHAR_WIDTH || !big || !value) { 
        return -1; 
    } 

    int index = x / 4;
    int bit_position = y + (x % 4) * 8;

    *value = (big[index] >> bit_position) & 1; // Читаем бит

    return 0;
}

// Выводит большой символ
int bc_printbigchar(int *big, int x, int y, int fg, int bg) {
    switch_table();
    mt_setbgcolor(bg);
    mt_setfgcolor(fg);
    for (int i = 0; i < BIGCHAR_WIDTH; i++) { 
        mt_gotoXY(x + i, y); // Перемещаем курсор
        for (int j = 0; j < BIGCHAR_WIDTH; j++) { 
            int value;
            bc_getbigcharpos(big, i, j, &value); 
            printf("%c",value ? ACS_CKBOARD : '#'); // Вывод символа
        }
    }

    return_table();
    mt_setdefaultcolor();
    return 0;
}

int bc_bigcharwrite (int fd, int * big, int count){
    if (fd < 3 && big){
        return -1;
    }

    int bytes_written = write(fd, big, count * 2 * sizeof(int));
    if (bytes_written != count * 2 * sizeof(int)) {
        return -1; 
    }

    return 0; 
}

int bc_bigcharread(int fd, int *big, int need_count, int *count) {
    if (fd < 3 || !big || !count || need_count <= 0) {
        if (count) 
            *count = 0;
        return -1; 
    }

    int bytes_read = read(fd, big, need_count * 2 * sizeof(int));
    if (bytes_read < 0) {
        *count = 0;
        return -1; // Ошибка чтения
    }

    *count = bytes_read / (2 * sizeof(int)); // Вычисляем, сколько символов удалось прочитать
    return 0; // Успешное чтение
}

// Главная функция для тестирования
int main() {
    int font[][2] = {
    {0x3C666666, 0x6666663C}, // 0
    {0x18181818, 0x18181818}, // 1
    {0x3C66060C, 0x1830607E}, // 2
    {0x3C66060C, 0x0C06663C}, // 3
    {0x0C1C3C6C, 0x7E0C0C0C}, // 4
    {0x7E60603C, 0x0606663C}, // 5
    {0x3C60603C, 0x3C66663C}, // 6
    {0x7E060C18, 0x30303030}, // 7
    {0x3C66663C, 0x3C66663C}, // 8
    {0x3C66663C, 0x0C183060}, // 9
    {0x3C66663C, 0x3C66663C}, // A
    {0x3E66663E, 0x3E66663E}, // B
    {0x3C660606, 0x0606663C}, // C
    {0x3E666666, 0x6666663E}, // D
    {0x7E06063E, 0x3E06067E}, // E
    {0x7E06063E, 0x06060606}, // F
    {0x081C3E3E, 0x3E3E1C08}  // +
};
    int big[2] = {0, 0}; // Пустой символ
    // Установим крестик в центре
    bc_setbigcharpos(big, 4-1, 4-1, 1);
    bc_setbigcharpos(big, 5-1, 4-1, 1);
    bc_setbigcharpos(big, 6-1, 4-1, 1);
    bc_setbigcharpos(big, 5-1, 3-1, 1);
    bc_setbigcharpos(big, 5-1, 5-1, 1);

    // Проверяем получение битов
    int value;
    bc_getbigcharpos(big, 3, 3, &value);
    printf("Bit at (3,3): %d\n", value);
    bc_getbigcharpos(big, 4, 4, &value);
    printf("Bit at (4,4): %d\n", value);
    bc_getbigcharpos(big, 0, 0, &value);
    printf("Bit at (0,0): %d\n", value);
    bc_getbigcharpos(big, 3, 4, &value);
    printf("Bit at (3,4): %d\n", value);
    bc_getbigcharpos(big, 4, 3, &value);
    printf("Bit at (4,3): %d\n", value);

    // Выводим большой символ
    bc_printbigchar(font[2], 10, 15, WHITE, BLACK); // Белый на черном

    mt_gotoXY(0,0);

    return 0;
}
