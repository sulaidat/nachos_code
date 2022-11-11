#include "syscall.h"

#define MAX_LENGTH 255

int main(){
    PrintString("Nhom do an he dieu hanh\n");
    PrintString("Thanh vien\n");
    PrintString("20120218 - Nguyen Dinh Tri\n");
    PrintString("20120354 - Dang Huynh Cuu Quan\n");
    PrintString("20120621 - Nguyen Quang Tuyen\n");
    PrintString("* Chuong trinh sort \n");
    PrintString("\tChuong trinh cho nguoi dung nhap vao n (n<=100) la do dai cua mang. Bien A[100] la mang chua phan tu.\n");
    PrintString(" Sau khi nhap du phan tu chuong trinh se hoi nguoi dung sap xep theo thu tu tang dan hay giam dan.");
    PrintChar('\n');
    PrintString(" Chuong trinh dung thuat toan bubble sort de sap xep. Duyet bat dau tu vi tri dau tien, xet lan luot tung cap 2 phan tu, ");
    PrintString("phan tu nao lon hon hon se chuyen sang phai(voi thu tu tang dan). Sau vong lap dau ta sẽ dua duoc phan tu lon nhat xuong cuoi mang. ");
    PrintString("Sang vòng thứ hai, ta tiếp tục bắt đầu ở vị trí đầu tiên như vậy và đưa được phần tử lớn thứ hai về vị trí thứ hai ở cuối dãy. Tuong tu cho cac vong lap tiep theo\n");

    PrintString("* Chuong trinh ascii \n");
    PrintString("\tChuong trinh in ra ma nhi phan, ma thap phan, ma thap luc phan, hien thi cua cac ma ascii in duoc (tu 32->126 trong he thap phan) \n");

    PrintChar('\n');
    Halt();
}