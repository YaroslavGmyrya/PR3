#include "sc_accumulatorHeader.h"
#include "sc_mainHeader.h"

extern int accumulator; 

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
  if (value >= 0x800) /* 0x800 = pow(2,15) */
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
  printf ("sc: %d hex: %x\n", value, value);
}
