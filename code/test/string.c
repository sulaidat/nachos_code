#include "syscall.h"

int main()
{
  char* result;
  ReadString(result,9);
  PrintString(result);

  int test = 0;

  Halt();
  /* not reached */
}
