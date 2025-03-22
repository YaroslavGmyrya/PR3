#include "../include/myTerm.h"
#include "../include/MySimpleComputer.h"

void
sc_addIOEntry (int address, char type, int value)
{
    for (int i = 1; i < MAX_LINES; i++)
    {
        io_log[i - 1] = io_log[i];
    }
    
    io_log[MAX_LINES - 1].address = address;
    io_log[MAX_LINES - 1].type = type;
    io_log[MAX_LINES - 1].value = value;
}

void 
sc_printTerm () 
{
    mt_gotoXY(20,100);
    printf ("IN-OUT:\n");
    for (int i = 0, j = 1; i < MAX_LINES; i++) 
    {
        if (io_log[i].type != 0) 
        { 
            mt_gotoXY (20+j,100);
            printf("%03d %c %04X\n", io_log[i].address, io_log[i].type, io_log[i].value);
            j++;
        }
    }
}