#include "../include/MySimpleComputer.h"
#include "../include/myTerm.h"

int
sc_memoryGet(int address, int *value) 
{ 
    if (address >= SIZE || address < 0 || !value) 
    { 
        return -1; 
    } 
 
    *value = memory[address]; 
    return 0; 
} 

int
sc_memoryInit(void) 
{ 
    for (int i = 0; i < SIZE; i++) 
    { 
        memory[i] = 0; 
    } 
    return 0; 
} 

int
sc_memoryLoad(char *filename) 
{ 

    if (!filename)
      return -1;

    FILE *file = fopen(filename, "rb"); 
 
    if (!file) 
    { 
        return -1; 
    } 
 
    int result = fread(memory, sizeof(int), SIZE, file); 

    fclose(file); 
 
    if (result < SIZE) 
    { 
        return -1; 
    } 
 
    return 0; 
}

int
sc_memorySave(char *filename) 
{ 

    if (!filename)
        return -1;

    FILE *file = fopen(filename, "wb"); 
 
    if (!file) 
    { 
        return -1; 
    } 
 
    int result = fwrite(memory, sizeof(int), SIZE, file); 

    fclose(file); 
 
    if (result < SIZE) 
    { 
        return -1; 
    } 
 
    return 0; 
}

int
sc_memorySet(int address, int value) 
{ 
    if (address >= SIZE && value >= 0x800) 
    { 
        return -1; 
    } 
 
    memory[address] = value; 
    return 0; 
} 

void
sc_printCell (int address, enum colors fg, enum colors bg) 
{ 

    int row =  address / 10 + 2; 
    int col = (address % 10) * 7 + 3; 

    if (address >= 128 || address < 0) 
    { 
        return; 
    } 

    mt_setfgcolor(fg);
    mt_setbgcolor(bg);
    mt_gotoXY(row, col);
    
    printf("%d", memory[address]); 

}
