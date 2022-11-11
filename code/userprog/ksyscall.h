/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__
#define __USERPROG_KSYSCALL_H__

#include "kernel.h"
#include "synchconsole.h"

void SysHalt()
{
    kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
    return op1 + op2;
}

int isSpace(char c)
{
    if (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')
    {
        return 1;
    }
    return 0;
}

int isNum(char c)
{
    if (c <= '9' && c >= '0')
    {
        return 1;
    }
    return 0;
}

// space (0x20, ' ')
// form feed (0x0c, '\f')
// line feed (0x0a, '\n')
// carriage return (0x0d, '\r')
// horizontal tab (0x09, '\t')
// vertical tab (0x0b, '\v')
// -2147483648 to 2147483647 (max 11 character)

/// @brief  Read a string from stdin and convert it into an integer, any input that out of length (11 for negative int, 10 for positive int) return 0,
///         any input that bigger than 2147483647 or smaller than -2147483648 is automatically converted into 2147483647 or -2147483648
///         (I have skipped situations like 3000000000)
/// @return int
int SysReadNum()
{
    char c = kernel->synchConsoleIn->GetChar();
    // max int len is 11 (-2147483648), last byte is for null terminated
    char buf[12];
    int cnt = 0;

    // skip white space
    while (isSpace(c))
        c = kernel->synchConsoleIn->GetChar();

    if (c == '-')
    {
        // negative int -> max_int_len = 11
        buf[cnt++] = c;
        c = kernel->synchConsoleIn->GetChar();
        while (isNum(c) && cnt < 11)
        {
            buf[cnt++] = c;
            c = kernel->synchConsoleIn->GetChar();
        }
    }
    else
    {
        // positive int -> max_int_len = 10
        while (isNum(c) && cnt < 10)
        {
            buf[cnt++] = c;
            c = kernel->synchConsoleIn->GetChar();
        }
    }

    // now c is not a number digit anymore
    // if c is not a white space nor null byte, SysReadNum fail, return 0
    if (!isSpace(c) && c != 0)
    {
        return 0;
    }

    // safely null terminated
    buf[cnt] = 0;
    //  now I can call atoi comfortably :v
    return atoi(buf);
}

/// @brief Print num to stdout
/// @param num
/// @return number of characters printed
int SysPrintNum(int num)
{
    if (num == 0)
    {
        kernel->synchConsoleOut->PutChar('0');
        return 1;
    }
    int neg = 1;
    if (num < 0)
    {
        kernel->synchConsoleOut->PutChar('-');
        // num = -num
        // must not do this! -(-2147483648) = 2**64-2147483648 = 0
        neg = -1;
    }
    char buf[11];
    int cnt = 0;
    while (num)
    {
        // -2147483648 % 10 return -8, not 2
        buf[cnt++] = neg == -1 ? num % 10 * (-1) + 48 : (num % 10) + 48;
        num /= 10;
    }
    for (int i = cnt - 1; i >= 0; i--)
    {
        kernel->synchConsoleOut->PutChar(buf[i]);
    }
    return cnt;
}

/// @brief Read a char from stdin
/// @return char
int SysReadChar()
{
    return kernel->synchConsoleIn->GetChar();
}

/// @brief Print char c to stdout
/// @param c
void SysPrintChar(char c)
{
    kernel->synchConsoleOut->PutChar(c);
}

/// @brief Generate a random integer, seed with time(0)
/// @return random int
int SysRandomNum()
{
    RandomInit(time(0));
    return RandomNumber();
}

int isEnter(char c)
{
    return (c == '\n' ? 1 : 0);
}

void SysReadString(char *buf, int length)
{
    char c = kernel->synchConsoleIn->GetChar();
    int index = 0;
    while (!isEnter(c))
    {
        if (index < length)
        {
            buf[index] = c;
        }
        index++;
        c = kernel->synchConsoleIn->GetChar();
    }
}

void SysPrintString(char *buf)
{
    int index = 0;
    while ((buf[index] != '\0'))
    {
        kernel->synchConsoleOut->PutChar(buf[index]);
        index++;
    }
}

bool SysCreate(char *filename)
{
    int len = strlen(filename);

    if (len == 0)
    {
        DEBUG(dbgSys, "[dbgSys] File name can't be empty");
        return false;
    }
    if (!kernel->fileSystem->Create(filename))
    {
        DEBUG(dbgSys, "[dbgSys] Error while creating file");
        return false;
    }
    return true;
}

int SysOpen(char *filename)
{
    if (!filename)
    {
        DEBUG(dbgSys, "[dbgSys] Invalid pointer");
        return -1;
    }
    int len = strlen(filename);
    if (len == 0)
    {
        DEBUG(dbgSys, "[dbgSys] File name can't be empty");
        return -1;
    }
    return OpenForReadWrite(filename, false);
}

bool SysClose(int fd)
{
    if (fd < 0)
    {
        DEBUG(dbgSys, "[dbgSys] Invalid file descriptor");
        return false;
    }
    if (Close(fd) == 1)
    {
        return true;
    }
    return false;
}

int SysRead(OpenFileId fd, char *buf, int count)
{
    if (fd < 0)
    {
        DEBUG(dbgSys, "[dbgSys] Invalid file descriptor");
        return -1;
    }

    return ReadPartial(fd, buf, count);
}

int SysWrite(OpenFileId fd, char *buf, int count)
{
    if (fd < 0)
    {
        DEBUG(dbgSys, "[dbgSys] Invalid file descriptor");
        return -1;
    }
    return WriteFile(fd, buf, count);
}


#endif /* ! __USERPROG_KSYSCALL_H__ */
