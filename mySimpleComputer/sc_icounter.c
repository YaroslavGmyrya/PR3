#include "sc_icounterHeader.h"
#include "sc_mainHeader.h"

int
sc_icounterGet (int *value)
{
  if (!value)
    {
      return -1;
    }

  *value = command_counter;
  return 0;
}

int
sc_icounterInit (void)
{
  command_counter = 0;
  return 0;
}

int
sc_icounterSet (int value)
{
  if (value >= 0x800) /* 0x800 = pow(2,15) */
    {
      return -1;
    }

  command_counter = value;
  return 0;
}

void
sc_printCounter (void)
{
  int value;
  sc_icounterGet (&value);
  printf ("T: %d    IC: %d\n", value, value);
}
