#include "../userprog/syscall.h"

#include "syscall.h"

int main()
{
    int i;
    int buf[7];
    int dec[7];
    char hex[2];
    int k, index;
    int m;
    PrintString("He nhi phan\tHe thap phan\tHe thap luc phan\tHien thi\n");
    for(i=32;i<=126;i++)
    {
        int thapphan=i;
        /* biến j lưu độ dài các số dư
        biến buf lưu lại các giá trị dư sau khi chia (chưa đảo lại thứ tự)
        VD: 42--->buf=[0,1,0,1,0,1];  j=5 */
        int j=0;
        while (thapphan!=0)
        {
            dec[j]= thapphan%2;
            thapphan=thapphan/2;
            j++;
        }
        j--;
        m=0;
        index=1;
        thapphan=i;
        /* Chuyển thập phân thành thập lục phân
        VD:42----> 2A----->hex[0]=2, hex[1]=A
        */
        while(thapphan!=0)
        {
            int du=thapphan%16;
            hex[index]= du<10? du+48: du +55;
            thapphan=thapphan/16;
            index--;
        }
        for (k=0;k<=7;k++)
            if (k==7-j)
            {
                PrintNum(dec[j]);
                j--;
            }
            else
                PrintNum(0);
        //In ra màn hình
        PrintString("\t\t");
        PrintNum(i);
        PrintString("\t\t");
        PrintChar(hex[0]);
        PrintChar(hex[1]);
        PrintString("\t\t");
        PrintChar((char)i);
        PrintChar('\n');
    }
    Halt();
}