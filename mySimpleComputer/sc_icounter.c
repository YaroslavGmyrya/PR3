#include "../include/MySimpleComputer.h"
#include "../include/myTerm.h"

int
sc_icounterGet(int *value) { 
    if (!value) { 
        return -1; 
    } 
 
    *value = command_counter; 
    return 0; 
} 

int
sc_icounterInit(void) { 
    command_counter = 0; 
    return 0; 
} 

int
sc_icounterSet(int value) { 
    if (value >= 0x800) {  // 0x800 = pow(2,15) 
        return -1; 
    } 
 
    command_counter = value; 
    return 0; 
} 

void 
sc_printCounter(void) 
{
    int value;
    int row,col;

    mt_getscreensize(&row, &col);
    sc_icounterGet(&value);

    mt_gotoXY(7, col - 59);

    printf("T: %d    IC: %d", value, value);
}