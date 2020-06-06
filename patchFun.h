#ifndef PATCH_FUN_H_
#define PATCH_FUN_H_

#include <ntdef.h>
#include <ntifs.h>

#define CACHE_LINE  8  
#define CACHE_ALIGN __declspec(align(CACHE_LINE))  

typedef struct CACHE_ALIGN memStruct
{
	__int64    m_userPid;
	__int64    m_pid;
	__int64    m_addr;
	__int64    m_buffer;
	__int64    m_size;
} MemStruct, *pMemStruct;

typedef void (*EncryptDecryptBuf_)(char* buf, int size, char key);
typedef NTSTATUS(*DoMemFun_)(pMemStruct objData);
typedef PIO_STACK_LOCATION(*IoGetCurrentIrpStackLocation_)(_In_ PIRP Irp);
typedef VOID(FASTCALL *IofCompleteRequest_)(_In_ PIRP Irp, _In_ CCHAR PriorityBoost);
typedef NTSTATUS(*PsLookupProcessByProcessId_)(_In_ HANDLE ProcessId, _Outptr_ PEPROCESS *Process);
typedef VOID(*KeAttachProcess_)(_Inout_ PRKPROCESS Process);


typedef PVOID(*PsGetProcessSectionBaseAddress_)(__in PEPROCESS Process);
typedef VOID(*KeDetachProcess_)(VOID);
typedef LONG_PTR(*FASTCALL ObfDereferenceObject_)(_In_ PVOID Object);
typedef NTSTATUS(*MmCopyVirtualMemory_)(IN PEPROCESS FromProcess,IN CONST VOID *FromAddress,IN PEPROCESS ToProcess,OUT PVOID ToAddress,IN SIZE_T BufferSize,IN KPROCESSOR_MODE PreviousMode,OUT PSIZE_T NumberOfBytesCopied);
typedef NTSTATUS(*ZwAllocateVirtualMemory_)(_In_ HANDLE ProcessHandle,_Inout_ PVOID *BaseAddress,_In_ ULONG_PTR ZeroBits,_Inout_ PSIZE_T RegionSize,_In_ ULONG AllocationType,_In_ ULONG Protect);
typedef NTSTATUS(__stdcall *ZwProtectVirtualMemory_)(HANDLE ProcessHandle, PVOID *BaseAddress, PULONG ProtectSize, ULONG NewProtect, PULONG OldProtect);

typedef VOID(*KeStackAttachProcess_)(_Inout_ PRKPROCESS PROCESS, _Out_ PRKAPC_STATE ApcState);
typedef VOID(*KeUnstackDetachProcess_)(_In_ PRKAPC_STATE ApcState);

BOOLEAN KernelMemCopy(PVOID dst, PVOID src, SIZE_T lenth, SIZE_T memsetlen);
ULONG64 CalcWdkoptimizationCodeSize(UCHAR *adr);
NTSTATUS DoMemBase(pMemStruct objData);
NTSTATUS DoMemRead(pMemStruct objData);
NTSTATUS DoMemWrite(pMemStruct objData);
NTSTATUS DoMemAlloc(pMemStruct objData);
NTSTATUS DoMemProtect(pMemStruct objData);

NTSTATUS FASTCALL myIrpDispath(IN PDEVICE_OBJECT DeviceObject, IN OUT PIRP Irp);

#endif