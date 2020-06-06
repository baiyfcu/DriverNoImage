#include "replaceData.h"
#include <ntdef.h>
#include <ntifs.h>

#include "inlineHook.h"


NTSTATUS _DoMemBase(pMemStruct objData);
NTSTATUS _DoMemRead(pMemStruct objData);
NTSTATUS _DoMemWrite(pMemStruct objData);
NTSTATUS _DoMemAlloc(pMemStruct objData);
NTSTATUS _DoMemProtect(pMemStruct objData);
void _EncryptDecryptBuf(char* buf, int size, int key);
typedef struct _Fun_Index
{
	PVOID funTableFlag;
	PVOID funAddr;
}Fun_Index, *pFun_Index;


typedef struct _Fun_Table
{
	int count;
	Fun_Index funIndex[100];
}Fun_Table,*pFun_Table;


Fun_Table g_funTable;

MyStruct* g_myStruct = NULL;
ZwProtectVirtualMemory_ g_ZwProtectVirtualMemory = NULL;
ULONG64 CalcWdkoptimizationCodeSize(UCHAR *adr) {
	UCHAR *orgi = adr;
	// C3 CC CC CC CC
	//ffff880`0307c3db c3              ret
	//	fffff880`0307c3dc cc              int     3
	//	fffff880`0307c3dd cc              int     3
	//fffff880`0307c3de cc              int     3
	//fffff880`0307c3df cc              int     3

/*
	while (adr[0] != 0XC3 && adr[1] != 0xCC && adr[2] != 0xCC && adr[3] != 0xCC && adr[4] != 0xCC)
	{
		adr++;
	}*/

	while (adr < orgi + 0x1000)
	{
		if (adr[0] == 0XC3 && adr[1] == 0xCC && adr[2] == 0xCC && adr[3] == 0xCC && adr[4] == 0xCC)
		{
			UCHAR a = adr[0];
			UCHAR b = adr[1];
			UCHAR c = adr[2];
			UCHAR d = adr[3];
			break;
		}
			
		adr++;
	}



	return ((adr - orgi) + 4 + 4);
}

PVOID CopyMyFun(ULONG64 funAddr, OUT ULONG* size)
{
	PVOID myIrpAddrNew = ExAllocatePoolWithTag(NonPagedPool, PAGE_SIZE, 0);
	RtlFillMemory(myIrpAddrNew, PAGE_SIZE, 0);
	KIRQL irql = WPOFFx64();
	ULONG64 funSize = CalcWdkoptimizationCodeSize((UCHAR*)funAddr);

	WPONx64(irql);
	if(size > NULL)
		*size = funSize;
	memcpy((PVOID)myIrpAddrNew, (PVOID)funAddr, funSize);
	int iiii = memcmp((PVOID)myIrpAddrNew, (PVOID)funAddr, funSize);
	ReplaceFun(myIrpAddrNew);
	return myIrpAddrNew;
}

void EncryptDecryptBuf(char* buf, int size, char key)
{
	for (int i = 0; i < size; i++)
	{
		buf[i] = buf[i] ^ key;
	}
}

void dealMyFun(ULONG64 srcFunAddr, INT key, OUT ULONG64* addr, OUT ULONG* size)
{
	*addr = CopyMyFun(srcFunAddr, size);
	//*(int*)(*addr + *size) = key;
	EncryptDecryptBuf((char*)*addr, *size, key);
}

void InitFunTable()
{
	UNICODE_STRING DestinationString;

	g_funTable.count = 0;

	//////////////////////////////////////////////////////////////////////////

	//系统API
	g_funTable.funIndex[g_funTable.count].funTableFlag = DEBUG_TEST < 1? FLAG_IoGetCurrentIrpStackLocation : FLAG_IoGetCurrentIrpStackLocation_;
	g_funTable.funIndex[g_funTable.count].funAddr = IoGetCurrentIrpStackLocation;
	g_funTable.count++;

	g_funTable.funIndex[g_funTable.count].funTableFlag = DEBUG_TEST < 1 ? FLAG_IofCompleteRequest : FLAG_IofCompleteRequest_;
	g_funTable.funIndex[g_funTable.count].funAddr = IofCompleteRequest;
	g_funTable.count++;

	g_funTable.funIndex[g_funTable.count].funTableFlag = DEBUG_TEST < 1 ? FLAG_PsLookupProcessByProcessId : FLAG_PsLookupProcessByProcessId_;
	g_funTable.funIndex[g_funTable.count].funAddr = PsLookupProcessByProcessId;
	g_funTable.count++;

	g_funTable.funIndex[g_funTable.count].funTableFlag = DEBUG_TEST < 1 ? FLAG_KeAttachProcess : FLAG_KeAttachProcess_;
	g_funTable.funIndex[g_funTable.count].funAddr = KeAttachProcess;
	g_funTable.count++;

	g_funTable.funIndex[g_funTable.count].funTableFlag = DEBUG_TEST < 1 ? FLAG_PsGetProcessSectionBaseAddress : FLAG_PsGetProcessSectionBaseAddress_;
	g_funTable.funIndex[g_funTable.count].funAddr = PsGetProcessSectionBaseAddress;
	g_funTable.count++;

	g_funTable.funIndex[g_funTable.count].funTableFlag = DEBUG_TEST < 1 ? FLAG_KeDetachProcess : FLAG_KeDetachProcess_;
	g_funTable.funIndex[g_funTable.count].funAddr = KeDetachProcess;
	g_funTable.count++;

	g_funTable.funIndex[g_funTable.count].funTableFlag = DEBUG_TEST < 1 ? FLAG_ObfDereferenceObject : FLAG_ObfDereferenceObject_;
	g_funTable.funIndex[g_funTable.count].funAddr = ObfDereferenceObject;
	g_funTable.count++;

	g_funTable.funIndex[g_funTable.count].funTableFlag = DEBUG_TEST < 1 ? FLAG_MmCopyVirtualMemory : FLAG_MmCopyVirtualMemory_;
	g_funTable.funIndex[g_funTable.count].funAddr = MmCopyVirtualMemory;
	g_funTable.count++;

	g_funTable.funIndex[g_funTable.count].funTableFlag = DEBUG_TEST < 1 ? FLAG_ZwAllocateVirtualMemory : FLAG_ZwAllocateVirtualMemory_;
	g_funTable.funIndex[g_funTable.count].funAddr = ZwAllocateVirtualMemory;
	g_funTable.count++;

	g_funTable.funIndex[g_funTable.count].funTableFlag = DEBUG_TEST < 1 ? FLAG_ZwProtectVirtualMemory : FLAG_ZwProtectVirtualMemory_;
	RtlInitUnicodeString(&DestinationString, L"ZwProtectVirtualMemory");
	g_funTable.funIndex[g_funTable.count].funAddr = (ZwProtectVirtualMemory_)MmGetSystemRoutineAddress(&DestinationString);
	g_ZwProtectVirtualMemory = g_funTable.funIndex[g_funTable.count].funAddr;
	g_funTable.count++;

	g_funTable.funIndex[g_funTable.count].funTableFlag = DEBUG_TEST < 1 ? FLAG_KeStackAttachProcess : FLAG_KeStackAttachProcess_;
	g_funTable.funIndex[g_funTable.count].funAddr = KeStackAttachProcess;
	g_funTable.count++;

	g_funTable.funIndex[g_funTable.count].funTableFlag = DEBUG_TEST < 1 ? FLAG_KeUnstackDetachProcess : FLAG_KeUnstackDetachProcess_;
	g_funTable.funIndex[g_funTable.count].funAddr = KeUnstackDetachProcess;
	g_funTable.count++;

	g_funTable.funIndex[g_funTable.count].funTableFlag = DEBUG_TEST < 1 ? FLAG_MM_HIGHEST_USER_ADDRESS : FLAG_MM_HIGHEST_USER_ADDRESS_;
	g_funTable.funIndex[g_funTable.count].funAddr = &MM_HIGHEST_USER_ADDRESS;
	g_funTable.count++;

	//功能函数

	g_funTable.funIndex[g_funTable.count].funTableFlag = DEBUG_TEST < 1 ? FLAG_EncryptDecryptBuf : FLAG_EncryptDecryptBuf_;
	g_funTable.funIndex[g_funTable.count].funAddr = CopyMyFun(_EncryptDecryptBuf, NULL);
	g_funTable.count++;

	//内部函数

	g_funTable.funIndex[g_funTable.count].funTableFlag = DEBUG_TEST < 1 ? FLAG_MYSTRUCT : FLAG_MYSTRUCT_;
	g_funTable.funIndex[g_funTable.count].funAddr = ExAllocatePoolWithTag(NonPagedPool, PAGE_SIZE, 0);
	g_funTable.count++;
	
	g_funTable.funIndex[g_funTable.count].funTableFlag = 0x68888000c9e8f0cc;
	g_funTable.funIndex[g_funTable.count].funAddr = g_funTable.funIndex[g_funTable.count - 1].funAddr;
	g_funTable.count++;

	//开始填充数据
	g_myStruct = (MyStruct*)(g_funTable.funIndex[g_funTable.count-1].funAddr);

	g_myStruct->_DoMemBaseKey = 1;
	dealMyFun(_DoMemBase, g_myStruct->_DoMemBaseKey, &g_myStruct->_DoMemBaseAddr, &g_myStruct->_DoMemBaseSize);
	g_myStruct->_DoMemReadKey = 1;
	dealMyFun(_DoMemRead, g_myStruct->_DoMemReadKey, &g_myStruct->_DoMemReadAddr, &g_myStruct->_DoMemReadSize);
	g_myStruct->_DoMemWriteKey = 1;
	dealMyFun(_DoMemWrite, g_myStruct->_DoMemWriteKey, &g_myStruct->_DoMemWriteAddr, &g_myStruct->_DoMemWriteSize);
	g_myStruct->_DoMemAllocKey = 1;
	dealMyFun(_DoMemAlloc, g_myStruct->_DoMemAllocKey, &g_myStruct->_DoMemAllocAddr, &g_myStruct->_DoMemAllocSize);
	g_myStruct->_DoMemProtectKey = 1;
	dealMyFun(_DoMemProtect, g_myStruct->_DoMemProtectKey, &g_myStruct->_DoMemProtectAddr, &g_myStruct->_DoMemProtectSize);
	//////////////////////////////////////////////////////////////////////////

/*
	ULONG  size = 0;
	g_funTable.funIndex[g_funTable.count].funTableFlag = FLAG_DoMemBase;
	g_funTable.funIndex[g_funTable.count].funAddr = CopyMyFun((ULONG64)_DoMemBase, &size);
	g_funTable.count++;

	g_funTable.funIndex[g_funTable.count].funTableFlag = FLAG_DoMemRead;
	g_funTable.funIndex[g_funTable.count].funAddr = CopyMyFun((ULONG64)_DoMemRead, &size);
	g_funTable.count++;

	g_funTable.funIndex[g_funTable.count].funTableFlag = FLAG_DoMemWrite;
	g_funTable.funIndex[g_funTable.count].funAddr = CopyMyFun(_DoMemWrite, &size);
	g_funTable.count++;

	g_funTable.funIndex[g_funTable.count].funTableFlag = FLAG_DoMemAlloc;
	g_funTable.funIndex[g_funTable.count].funAddr = CopyMyFun((ULONG64)_DoMemAlloc, &size);
	g_funTable.count++;

	g_funTable.funIndex[g_funTable.count].funTableFlag = FLAG_DoMemProtect;
	g_funTable.funIndex[g_funTable.count].funAddr = CopyMyFun((ULONG64)_DoMemProtect, &size);
	g_funTable.count++;*/

}


BOOLEAN ReplaceFun(UCHAR *Original)
{
	UCHAR* indexStart = Original;
	for (int i = 0; i < 0x1000 - 8; i++)
	{
		if (*(int*)(indexStart) == 0xCCCC)
			break;
		else
		{
			ULONG64 CL = *(ULONG64*)indexStart;
			//DbgPrint("CL:%llx\n", CL);
			for (int j = 0; j < g_funTable.count; j++)
			{
				ULONG64 UL = g_funTable.funIndex[j].funTableFlag;
				
				
				if (g_funTable.funIndex[j].funTableFlag == *(ULONG64*)indexStart)
				//if(indexStart[0] == UL[0] && indexStart[1] == UL[1] && indexStart[2] == UL[2] && indexStart[3] == UL[3] && indexStart[4] == UL[4])
				{
					*(ULONG64*)indexStart = g_funTable.funIndex[j].funAddr;
					break;
				}
			}
		}
		indexStart = indexStart + 1;
	}

	return TRUE;
}