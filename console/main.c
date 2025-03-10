#include "../include/MySimpleComputer.h"

int
main ()
{
  int value, status, sign, command, operand;

  sc_memoryInit ();
  sc_accumulatorInit ();
  sc_icounterInit ();
  sc_regInit ();

  sc_memorySet (0, 10);
  sc_memorySet (1, 20);
  sc_memorySet (2, 30);
  sc_memorySet (3, 40);
  sc_memorySet (4, 50);
  sc_memorySet (128, 50);
  sc_memorySet (-1, 50);
  sc_memorySet (55, 50);

  printf ("Memory content:");

  for (int i = 0; i < SIZE; i++)
    {
      if (i % 10 == 0)
        printf ("\n");
      sc_memoryGet (i, &value);
      printf ("%d ", value);
    }
  printf ("\n");

  status = sc_memorySet (0, 0x1000);
  printf ("Invalid memory set status: %d (expected -1)\n", status);

  sc_regSet (P, 1);
  sc_regSet (Z, 1);
  sc_regSet (M, 0);
  sc_regSet (T, 1);
  sc_regSet (E, 0);
  printf ("Flags after setting: ");
  sc_printFlags ();

  status = sc_regSet (99, 1);
  printf ("Invalid flag set status: %d (expected -1)\n", status);

  sc_accumulatorSet (255);
  sc_accumulatorGet (&value);
  printf ("Accumulator value: %d (expected 255)\n", value);

  status = sc_accumulatorSet (0x1000);
  printf ("Invalid accumulator set status: %d (expected -1)\n", status);

  sc_icounterSet (42);
  sc_icounterGet (&value);
  printf ("Instruction Counter: %d (expected 42)\n", value);

  status = sc_icounterSet (0x1000);
  printf ("Invalid instruction counter set status: %d (expected -1)\n",
          status);

  sc_memoryGet (0, &value);
  sc_printDecodedCommand (value);
  sc_accumulatorGet (&value);
  sc_printDecodedCommand (value);

  sc_commandEncode (0, 0x1F, 0x7F, &value);
  printf ("Encoded command: DEC = %d, HEX = %X, OCT = %o\n", value, value,
          value);

  return 0;
}
