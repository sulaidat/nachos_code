// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"

#define LENGTH 255
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------

// Hàm User2System và hàm System2User sử dụng cho system call ReadString và PrintString
char *User2System(int virtAddr, int limit)
{
	int i; // chi so index
	int value;
	char *buffer = NULL;
	buffer = new char[limit + 1]; // can cho chuoi terminal
	if (buffer == NULL)
		return buffer;

	memset(buffer, 0, limit + 1);

	for (i = 0; i < limit; i++)
	{
		kernel->machine->ReadMem(virtAddr + i, 1, &value);
		buffer[i] = (char)value;
		if (value == 0)
			break;
	}
	return buffer;
}

/// @brief User2System string version (copy until reach null byte)
/// @param addr
/// @return pointer to string
char *User2System(int addr)
{
	int onechar;
	int len = 0;

	/* first find string len */
	kernel->machine->ReadMem(addr + len, 1, &onechar);
	while ((char)onechar != '\0')
	{
		len++;
		kernel->machine->ReadMem(addr + len, 1, &onechar);
	}

	/* now copy `len` byte to `str` and return */
	char *str = (char *)malloc(len + 1);
	for (int i = 0; i < len; i++)
	{
		kernel->machine->ReadMem(addr + i, 1, &onechar);
		str[i] = (char)onechar;
	}
	str[len] = '\0';

	return str;
}

void System2User(int virAddr, int length, char *buffer)
{
	int i = 0;
	int value = 1;
	while (i < length && value != 0)
	{
		value = (int)buffer[i];
		kernel->machine->WriteMem(virAddr + i, 1, value);
		i++;
	}
}

/// @brief Sytem2User string version (copy until reach null byte)
/// @param str
/// @param addr
void Sytem2User(char *str, int addr)
{
	int len = strlen(str);
	for (int i = 0; i < len; i++)
	{
		kernel->machine->WriteMem(addr + i, 1, str[i]);
	}
	kernel->machine->WriteMem(addr + len, 1, 0);
}

// Hàm tăng giá trị thanh ghi PC
// void IncreasePC()
// {
// 	int counter = kernel->machine->ReadRegister(PCReg);
//    	kernel->machine->WriteRegister(PrevPCReg, counter);
//     counter = kernel->machine->ReadRegister(NextPCReg);
//     kernel->machine->WriteRegister(PCReg, counter);
//    	kernel->machine->WriteRegister(NextPCReg, counter + 4);
// }

void ExceptionHandler(ExceptionType which)
{
	int type = kernel->machine->ReadRegister(2);
	DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

	switch (which)
	{
	// Xử lý exceptions
	case NoException:
		kernel->interrupt->Halt();
		return;
	case PageFaultException:
		cerr << "\nNo valid translation found.\n";
		ASSERT(FALSE);
		break;
	case ReadOnlyException:
		cerr << "\nWrite attempted to page marked \"read-only\".\n";
		ASSERT(FALSE);
		break;
	case BusErrorException:
		cerr << "\nTranslation resulted in an invalid physical address.\n";
		ASSERT(FALSE);
		break;
	case AddressErrorException:
		cerr << "\nUnaligned reference or one that was beyond the end of the address space.\n";
		ASSERT(FALSE);
		break;
	case OverflowException:
		cerr << "\nInteger overflow in add or sub.\n";
		ASSERT(FALSE);
		break;
	case IllegalInstrException:
		cerr << "\nUnimplemented or reserved instr.\n"
			 << which << endl;
		ASSERT(FALSE);
		break;
	case NumExceptionTypes:
		cerr << "\nRuntime Exception: \n";
		ASSERT(FALSE);
		break;

	case SyscallException:
		switch (type)
		{
		// Systemcall ReadNum
		case SC_ReadNum:
		{
			DEBUG(dbgSys, "[dbgSys] Into SC_ReadNum");

			int result = SysReadNum();
			// cout << result << endl;
			kernel->machine->WriteRegister(2, result);

			DEBUG(dbgSys, "[dbgSys] End SC_ReadNum");
			// IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}
		// Systemcall PrintNum
		case SC_PrintNum:
		{
			DEBUG(dbgSys, "[dbgSys] Into SC_PrintNum");

			int num = kernel->machine->ReadRegister(4);
			int result = SysPrintNum(num);
			kernel->machine->WriteRegister(2, result);

			DEBUG(dbgSys, "[dbgSys] End SC_PrintNum");
			// IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}
		// Systemcall ReadChar
		case SC_ReadChar:
		{
			DEBUG(dbgSys, "[dbgSys] Into SC_ReadChar");

			char result = SysReadChar();
			kernel->machine->WriteRegister(2, result);

			DEBUG(dbgSys, "[dbgSys] End SC_ReadChar");
			// IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}
		// Systemcall PrintChar
		case SC_PrintChar:
		{
			DEBUG(dbgSys, "[dbgSys] Into SC_PrintChar");

			char arg = kernel->machine->ReadRegister(4);
			SysPrintChar(arg);

			DEBUG(dbgSys, "[dbgSys] End SC_PrintChar");
			// IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}
		// Systemcall RandomNum
		case SC_RandomNum:
		{
			DEBUG(dbgSys, "[dbgSys] Into SC_RandomNum");

			int result = SysRandomNum();
			cout << "random num: " << result << endl;
			kernel->machine->WriteRegister(2, result);

			DEBUG(dbgSys, "[dbgSys] End SC_RandomNum");
			// IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}
		// Systemcall ReadString
		case SC_ReadString:
		{
			int length;
			int virAddr;
			DEBUG(dbgSys, "[dbgSys] Into SC_ReadString");
			length = kernel->machine->ReadRegister(5);
			char *buf = new char[length];
			SysReadString(buf, length);
			DEBUG(dbgSys, "Buffer is: ");
			DEBUG(dbgSys, buf);
			virAddr = kernel->machine->ReadRegister(4);
			System2User(virAddr, length, buf);
			delete[] buf;
			DEBUG(dbgSys, "[dbgSys] End SC_ReadString");
			// IncreasePC();
			return;
		}
		// Systemcall PrintString
		case SC_PrintString:
		{
			DEBUG(dbgSys, "[dbgSys] Into SC_PrintString");
			int virAddr;
			virAddr = kernel->machine->ReadRegister(4);
			char *buf = new char[LENGTH];
			buf = User2System(virAddr, LENGTH + 1);
			SysPrintString(buf);
			DEBUG(dbgSys, "[dbgSys] End SC_PrintString");
			delete[] buf;
			// IncreasePC();
			return;
		}
		// Systemcall Create
		case SC_Create:
		{
			DEBUG(dbgSys, "[dbgSys] Into SC_Create");

			int virtAddr = kernel->machine->ReadRegister(4);
			char *filename = User2System(virtAddr);

			if (SysCreate(filename))
				kernel->machine->WriteRegister(2, 0);
			else
				kernel->machine->WriteRegister(2, -1);

			free(filename);

			DEBUG(dbgSys, "[dbgSys] End SC_Create");
			return;
		}
		// Systemcall Open
		case SC_Open:
		{
			DEBUG(dbgSys, "[dbgSys] Into SC_Open");

			int virtAddr = kernel->machine->ReadRegister(4);
			char *filename = User2System(virtAddr);

			int fd = SysOpen(filename);
			if (fd != -1)
				kernel->machine->WriteRegister(2, fd);
			else
				kernel->machine->WriteRegister(2, -1);

			DEBUG(dbgSys, "[dbgSys] End SC_Open");
			return;
		}
		// Systemcall Close
		case SC_Close:
		{
			DEBUG(dbgSys, "[dbgSys] Into SC_Close");

			int fd = kernel->machine->ReadRegister(4);
			kernel->machine->WriteRegister(2, SysClose(fd));

			DEBUG(dbgSys, "[dbgSys] End SC_Close");
			return;
		}
		// Systemcall Read
		case SC_Read:
		{
			DEBUG(dbgSys, "[dbgSys] Into SC_Read");

			int viraddr_buffer = kernel->machine->ReadRegister(4);
			int count = kernel->machine->ReadRegister(5);
			int fd = kernel->machine->ReadRegister(6);

			if (count < 0)
			{
				DEBUG(dbgSys, "[dbgSys] Invalid number of bytes");
				kernel->machine->WriteRegister(2, -1);
				return;
			}
			char *buf = (char *)malloc(count);
			int nbytes_read = SysRead(fd, buf, count);
			kernel->machine->WriteRegister(2, nbytes_read);
			if (nbytes_read > 0)
			{
				System2User(viraddr_buffer, nbytes_read, buf);
			}
			free(buf);

			DEBUG(dbgSys, "[dbgSys] End SC_Read");
			return;
		}
		// Systemcall Write
		case SC_Write:
		{
			DEBUG(dbgSys, "[dbgSys] Into SC_Write");

			int viraddr_buffer = kernel->machine->ReadRegister(4);
			int count = kernel->machine->ReadRegister(5);
			int fd = kernel->machine->ReadRegister(6);

			if (count < 0)
			{
				DEBUG(dbgSys, "[dbgSys] Invalid number of bytes");
				kernel->machine->WriteRegister(2, -1);
				return;
			}
			char *buf = User2System(viraddr_buffer, count);
			int nbytes_write = SysWrite(fd, buf, count);
			if (nbytes_write < 0)
			{
				DEBUG(dbgSys, "[dbgSys] Write fail");
				kernel->machine->WriteRegister(2, -1);
				free(buf);
				return;
			}
			kernel->machine->WriteRegister(2, nbytes_write);
			free(buf);

			DEBUG(dbgSys, "[dbgSys] End SC_Write");
			return;
		}

		case SC_Halt:
		{
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

			SysHalt();

			ASSERTNOTREACHED();
			break;
		}

		case SC_Add:
		{
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");

			/* Process SysAdd Systemcall*/
			int result;
			result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
							/* int op2 */ (int)kernel->machine->ReadRegister(5));

			DEBUG(dbgSys, "Add returning with " << result << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);

			// /* Modify return point */
			// {
			// 	/* set previous programm counter (debugging only)*/
			// 	kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

			// 	/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
			// 	kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

			// 	/* set next programm counter for brach execution */
			// 	kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
			// }

			return;
			ASSERTNOTREACHED();
			break;
		}

		default:
			cerr << "Unexpected system call " << type << "\n";
			break;
		}
		break;
	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
		break;
	}
	ASSERTNOTREACHED();
}