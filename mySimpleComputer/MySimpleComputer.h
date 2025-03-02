#pragma once

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h> 
#define MAX_LINES 10
#define MAX_STR_LEN 20
#define P 0x1
#define Z 0x2
#define M 0x4
#define T 0x8
#define E 0x10
#define SIZE 128
extern const int SIZE_COMMAND;
extern int memory[SIZE];
extern int accumulator;
extern int command_counter;
extern int flags_register;
int sc_memoryInit (void);
int sc_memorySet (int address, int value);
int sc_memoryGet (int address, int *value);
int sc_memorySave (char *filename);
int sc_memoryLoad (char *filename);
int sc_regInit (void);
int sc_regSet (int reg, int value);
int sc_regGet (int reg, int *value);
int sc_accumulatorInit (void);
int sc_accumulatorSet (int value);
int sc_accumulatorGet (int *value);
int sc_icounterInit (void);
int sc_icounterSet (int value);
int sc_icounterGet (int *value);
int sc_commandEncode (int sign, int command, int operand, int *value);
int sc_commandDecode (int value, int *sign, int *command, int *operand);
int sc_commandValidate (int command);
void sc_printCell (int address);
void sc_printFlags (void);
void sc_printBinary (int number);
void sc_printDecodedCommand (int value);
void sc_printAccumulator (void);
void sc_printCounter (void);
void sc_printBinary (int number);
int sc_memoryInit (void);
int sc_memorySet (int address, int value);
int sc_memoryGet (int address, int *value);
int sc_memorySave (char *filename);
int sc_memoryLoad (char *filename);
void sc_printCell (int address);
