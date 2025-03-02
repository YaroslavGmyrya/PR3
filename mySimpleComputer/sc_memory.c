#include "./MySimpleComputer.h"

int
sc_memoryGet (int address, int *value)
{
  if (address >= SIZE || address < 0 || !value)
    {
      return -1;
    }

  *value = memory[address];
  return 0;
}

int
sc_memoryInit (void)
{
  for (int i = 0; i < SIZE; i++)
    {
      memory[i] = 0;
    }
  return 0;
}

int
sc_memoryLoad (char *filename)
{
  if (!filename)
    return -1;

  FILE *file = fopen (filename, "rb");
  if (!file)
    {
      return -1;
    }

  int result = fread (memory, sizeof (int), SIZE, file);
  fclose (file);

  if (result < SIZE)
    {
      return -1;
    }

  return 0;
}

int
sc_memorySave (char *filename)
{
  if (!filename)
    return -1;

  FILE *file = fopen (filename, "wb");
  if (!file)
    {
      return -1;
    }

  int result = fwrite (memory, sizeof (int), SIZE, file);
  fclose (file);

  if (result < SIZE)
    {
      return -1;
    }

  return 0;
}

int
sc_memorySet (int address, int value)
{
  if (address >= SIZE || value >= 0x800)
    {
      return -1;
    }

  memory[address] = value;
  return 0;
}

