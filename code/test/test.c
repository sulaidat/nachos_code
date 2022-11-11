#include "syscall.h"

int main()
{
    int num = ReadNum();
    PrintNum(num);
    // PrintChar(ReadChar());
    // PrintNum(RandomNum());
    Halt();
}
