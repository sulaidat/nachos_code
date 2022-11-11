#include "syscall.h"
#include "../userprog/syscall.h"

int main()
{
    int fd = Open("tuyendeptrai");
    // PrintNum(fd);
    char buf[0x101];
    int nbytes = Read(&buf, 0x100, fd);
    Write(&buf, nbytes, 1);
    Halt();
}