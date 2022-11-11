#include "syscall.h"

int main()
{
  char* filename;  
  ReadString(filename, 9);
  Create(filename);
  Halt();
}
