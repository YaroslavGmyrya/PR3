#include "./MySimpleComputer.h"

int
sc_commandDecode (int value, int *sign, int *command, int *operand)
{
  if (!sign || !operand)
    {
      return -1;
    }

  *sign = (value >> 14) & 1;
  *command = (value >> 7) & 0x7F;
  *operand = value & 0x7F;

  return 0;
}

int
sc_commandEncode (int sign, int command, int operand, int *value)
{
  if (!value)
    {
      return -1;
    }

  if (sign != 0 && sign != 1)
    {
      return -1;
    }

  if (command < 0 || command > 0x7F)
    {
      return -1;
    }

  if (operand < 0 || operand > 0x7F)
    {
      return -1;
    }

  int result = 0;
  result |= (sign << 14);
  result |= (command << 7);
  result |= operand;
  *value = result;

  return 0;
}

int
sc_commandValidate (int command)
{
  if (command >= 0x800)
    {
      return -1;
    }

  return 0;
}

void
sc_printBinary (int number)
{
  for (int i = 14; i >= 0; i--)
    {
      int bit = (number >> i) & 1;
      printf ("%d", bit);
    }
  printf ("\n");
}

void
sc_printDecodedCommand (int value)
{
  printf ("DEC: %d | ", value);
  printf ("OCT: %o | ", value);
  printf ("HEX: %x   bin: ", value);
  sc_printBinary (value);
}
