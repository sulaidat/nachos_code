/* sort.c 
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */


/*
#define UNIX
#define UNIX_DEBUG
*/

#ifdef UNIX
#include <stdio.h>
#define Exit exit
#else
#include "syscall.h"
#endif /* UNIX */

int main()
{
    int i, j, tmp, size, increase;
    int A[100];
    // Đọc độ dài mảng và kiểm tra
    PrintString("Nhap do dai mang (<=100): ");
    size=ReadNum();
    while(size>=100 || size<=0)
    {
        PrintString("Do dai khong hop le. Xin moi nhap lai");
        PrintChar('\n');
        PrintString("Nhap do dai mang (<=100): ");
        size=ReadNum();
    }
    /* Đọc giá trị mảng do người dùng nhập */
    for (i = 0; i < size; i++) {
        A[i] = ReadNum();
    }
    PrintString("Nhap 0 de sap xep giam dan");
    PrintChar('\n');
    PrintString("Nhap 1 de sap xep tang dan");
    PrintChar('\n');
    //Đọc số do người dùng nhập để sắp xếp tăng dần hoặc giảm dần
    increase=ReadNum();
    while(increase!=1 && increase!=0)
    {
        PrintString("Khong hop le. Xin moi nhap lai: ");
        increase=ReadNum();
    }
    /* Sắp xếp theo Bubble Sort */
    // Sắp xếp giảm dần
    if (increase==0)
    {
        for (i = 0; i < size; i++) {
            for (j = 0; j < size-1; j++) 
            {
                if (A[j] < A[j + 1]) 
                {	
                    tmp = A[j];
                    A[j] = A[j + 1];
                    A[j + 1] = tmp;
                }
            }
        }
    }
    // Sắp xếp tăng dần
    else
    {
        for (i = 0; i < size; i++) {
            for (j = 0; j < size-1; j++) 
            {
                if (A[j] > A[j + 1]) 
                {	
                    tmp = A[j];
                    A[j] = A[j + 1];
                    A[j + 1] = tmp;
                }
            }
        }
    }
    // In ra màn hình
    for (i = 0; i < size; i++) {
       PrintNum(A[i]);
       PrintChar(' ');
    }
    PrintChar('\n');
    Halt();
}
