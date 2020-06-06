#include <ntdef.h>
#include <ntifs.h>
#include "patchFun.h"


//系统函数申明
NTKERNELAPI PVOID PsGetProcessSectionBaseAddress(__in PEPROCESS Process);
NTKERNELAPI NTSTATUS MmCopyVirtualMemory(
	IN PEPROCESS FromProcess,
	IN CONST VOID *FromAddress,
	IN PEPROCESS ToProcess,
	OUT PVOID ToAddress,
	IN SIZE_T BufferSize,
	IN KPROCESSOR_MODE PreviousMode,
	OUT PSIZE_T NumberOfBytesCopied
);

//////////////////////////////////////////////////////////////////////////
typedef struct _MyStruct
{
	__int32 _DoMemBaseKey;
	__int64 _DoMemBaseAddr;
	__int32   _DoMemBaseSize;

	__int32     _DoMemReadKey;
	__int64 _DoMemReadAddr;
	__int32   _DoMemReadSize;

	__int32     _DoMemWriteKey;
	__int64 _DoMemWriteAddr;
	__int32   _DoMemWriteSize;

	__int32     _DoMemAllocKey;
	__int64 _DoMemAllocAddr;
	__int32   _DoMemAllocSize;

	__int32     _DoMemProtectKey;
	__int64 _DoMemProtectAddr;
	__int32   _DoMemProtectSize;

}MyStruct, *pMyStruct;

extern MyStruct* g_myStruct;
extern ZwProtectVirtualMemory_ g_ZwProtectVirtualMemory;


#define DEBUG_TEST 0

#if DEBUG_TEST

//辅助功能函数
#define FLAG_EncryptDecryptBuf                   EncryptDecryptBuf

//自定义函数和结构
#define FLAG_MYSTRUCT                            g_myStruct
#define FLAG_DoMemBase                           DoMemBase
#define FLAG_DoMemRead                           DoMemRead
#define FLAG_DoMemWrite                          DoMemWrite
#define FLAG_DoMemAlloc                          DoMemAlloc
#define FLAG_DoMemProtect                        DoMemProtect


//系统API
#define FLAG_IoGetCurrentIrpStackLocation        IoGetCurrentIrpStackLocation    //抛弃不使用，inline函数不可翻译使用
#define FLAG_IofCompleteRequest                  IofCompleteRequest
#define FLAG_PsLookupProcessByProcessId          PsLookupProcessByProcessId
#define FLAG_KeAttachProcess                     KeAttachProcess
#define FLAG_PsGetProcessSectionBaseAddress      PsGetProcessSectionBaseAddress
#define FLAG_KeDetachProcess                     KeDetachProcess
#define FLAG_ObfDereferenceObject                ObfDereferenceObject
#define FLAG_MmCopyVirtualMemory                 MmCopyVirtualMemory
#define FLAG_ZwAllocateVirtualMemory             ZwAllocateVirtualMemory
#define FLAG_ZwProtectVirtualMemory              g_ZwProtectVirtualMemory
#define FLAG_KeStackAttachProcess                KeStackAttachProcess
#define FLAG_KeUnstackDetachProcess              KeUnstackDetachProcess
#define FLAG_MM_HIGHEST_USER_ADDRESS             MM_HIGHEST_USER_ADDRESS

#else

//辅助功能函数
#define FLAG_EncryptDecryptBuf                   0x68888000c9e8f000

//自定义函数和结构
#define FLAG_MYSTRUCT                            0x88888000c9e8f000
#define FLAG_DoMemBase                           0x88888010c9e8f000
#define FLAG_DoMemRead                           0x88888020c9e8f000
#define FLAG_DoMemWrite                          0x88888030c9e8f000
#define FLAG_DoMemAlloc                          0x88888040c9e8f000
#define FLAG_DoMemProtect                        0x88888050c9e8f000


//系统API
#define FLAG_IoGetCurrentIrpStackLocation        0x78888010c9e8f000    //抛弃不使用，inline函数不可翻译使用
#define FLAG_IofCompleteRequest                  0x78888020c9e8f000
#define FLAG_PsLookupProcessByProcessId          0x78888030c9e8f000
#define FLAG_KeAttachProcess                     0x78888040c9e8f000
#define FLAG_PsGetProcessSectionBaseAddress      0x78888050c9e8f000
#define FLAG_KeDetachProcess                     0x78888060c9e8f000
#define FLAG_ObfDereferenceObject                0x78888070c9e8f000
#define FLAG_MmCopyVirtualMemory                 0x78888080c9e8f000
#define FLAG_ZwAllocateVirtualMemory             0x78888090c9e8f000
#define FLAG_ZwProtectVirtualMemory              0x788880a0c9e8f000
#define FLAG_KeStackAttachProcess                0x788880b0c9e8f000
#define FLAG_KeUnstackDetachProcess              0x788880c0c9e8f000
#define FLAG_MM_HIGHEST_USER_ADDRESS             0x788880d0c9e8f000

#endif


//////////////////////////////////////////////////////////////////////////
//调试使用符号，需要同步更新同上，只是名字不同，标志相同
//辅助功能函数
#define FLAG_EncryptDecryptBuf_                   0x68888000c9e8f000

//自定义函数和结构
#define FLAG_MYSTRUCT_                             0x88888000c9e8f000
#define FLAG_DoMemBase_                            0x88888010c9e8f000
#define FLAG_DoMemRead_                            0x88888020c9e8f000
#define FLAG_DoMemWrite_                           0x88888030c9e8f000
#define FLAG_DoMemAlloc_                           0x88888040c9e8f000
#define FLAG_DoMemProtect_                         0x88888050c9e8f000


//系统API
#define FLAG_IoGetCurrentIrpStackLocation_         0x78888010c9e8f000    //抛弃不使用，inline函数不可翻译使用
#define FLAG_IofCompleteRequest_                   0x78888020c9e8f000
#define FLAG_PsLookupProcessByProcessId_           0x78888030c9e8f000
#define FLAG_KeAttachProcess_                      0x78888040c9e8f000
#define FLAG_PsGetProcessSectionBaseAddress_       0x78888050c9e8f000
#define FLAG_KeDetachProcess_                      0x78888060c9e8f000
#define FLAG_ObfDereferenceObject_                 0x78888070c9e8f000
#define FLAG_MmCopyVirtualMemory_                  0x78888080c9e8f000
#define FLAG_ZwAllocateVirtualMemory_              0x78888090c9e8f000
#define FLAG_ZwProtectVirtualMemory_               0x788880a0c9e8f000
#define FLAG_KeStackAttachProcess_                 0x788880b0c9e8f000
#define FLAG_KeUnstackDetachProcess_               0x788880c0c9e8f000
#define FLAG_MM_HIGHEST_USER_ADDRESS_              0x788880d0c9e8f000

void InitFunTable();
BOOLEAN ReplaceFun(UCHAR *Original);
PVOID CopyMyFun(ULONG64 funAddr, OUT ULONG* size);
void EncryptDecryptBuf(char* buf, int size, char key);