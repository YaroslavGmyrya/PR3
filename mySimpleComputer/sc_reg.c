#include "../include/MySimpleComputer.h"
#include "../include/myTerm.h"

int
sc_regGet(int reg, int *value) 
{ 

    if ((reg != P && reg != Z && reg != M && reg != T && reg != E) || !value) 
    {
        return -1;
    }

    if(flags_register & reg)
        *value = 1;

    else
        *value = 0;

    return 0; 
}

int
sc_regSet(int reg, int value) 
{ 

   if ((reg != P && reg != Z && reg != M && reg != T && reg != E) || (value != 0 && value != 1)) 
   {
    return -1;
   }

    if (value)
        flags_register |= reg;
    else
        flags_register &= ~reg;

    return 0; 
}

int
sc_regInit(void) { 

    flags_register = 0; 

    return 0; 
} 

void
sc_printFlags(void) 
{
    int value_P;
    int value_Z;
    int value_M;
    int value_T;
    int value_E;

    int rows;
    int cols;

    mt_getscreensize(&rows, &cols);

    mt_gotoXY(2, cols - 30);

    sc_regGet(P, &value_P);
    sc_regGet(Z, &value_Z);
    sc_regGet(M, &value_M);
    sc_regGet(T, &value_T);
    sc_regGet(E, &value_E);

    value_P ? printf("P ") : printf("_ ");
    value_Z ? printf("Z ") : printf("_ ");
    value_M ? printf("M ") : printf("_ ");
    value_T ? printf("T ") : printf("_ ");
    value_E ? printf("E ") : printf("_ ");

   
    mt_setdefaultcolor();
}
