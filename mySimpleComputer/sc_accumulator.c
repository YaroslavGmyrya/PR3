#include "../include/MySimpleComputer.h"
#include "../include/myTerm.h"

int
sc_accumulatorGet (int *value) 
{ 
    if (!value) 
    { 
        return -1; 
    } 
 
    *value = accumulator; 
    return 0; 
} 

int
sc_accumulatorInit (void) 
{ 
    accumulator = 0; 
    return 0; 
} 

int
sc_accumulatorSet (int value) 
{ 
    if (value >= 0x800) 
    { 
        return -1; 
    } 
 
    accumulator = value; 
    return 0; 
} 

void
sc_printAccumulator (void) 
{ 
    int value; 
    sc_accumulatorGet (&value);
    mt_gotoXY (2, 100); 
    printf ("sc: %d hex: %x\n", value, value); 
} 
