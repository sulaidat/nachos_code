#include "syscall.h"

// int main()
// {
//     char *filename;
//     ReadString(filename, 20);
//     Create(filename);
//     PrintNum(Open(filename));
//     // pause to check if file is opened
//     // using this command: 
//     // ll /proc/$(pidof nachos)/fd
//     ReadChar();   
//     PrintNum(Close(6));     // fd của file vừa tạo, lưu vào biến thì lỗi wtf TT
//     // pause to check if file is closed
//     ReadChar();
//     Halt();
// }

int main()
{
    // char* filename;
    // ReadString(filename, 20);
    // int fd = Open(filename);
    int fd = Open("tuyendeptrai");
    PrintNum(fd);
    ReadChar();
    PrintNum(Close(fd));
    ReadChar();
    Halt();
}