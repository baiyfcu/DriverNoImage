#include "patchFun.h"
#include "replaceData.h"


#define DIRECTORY_TABLE_BASE    0x028 


extern void* g_luOldAddr;

//////////////////////////////////////////////////////////////////////////


#define DEVICE_MY_INDEX 0x860
#define IRQ_CODE_MEM_BASE CTL_CODE(FILE_DEVICE_UNKNOWN,DEVICE_MY_INDEX,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IRQ_CODE_MEM_READ CTL_CODE(FILE_DEVICE_UNKNOWN,DEVICE_MY_INDEX + 1,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IRQ_CODE_MEM_WRITE CTL_CODE(FILE_DEVICE_UNKNOWN,DEVICE_MY_INDEX + 2,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IRQ_CODE_MEM_ALLOC CTL_CODE(FILE_DEVICE_UNKNOWN,DEVICE_MY_INDEX + 3,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IRQ_CODE_MEM_PROTECT CTL_CODE(FILE_DEVICE_UNKNOWN,DEVICE_MY_INDEX + 4,METHOD_BUFFERED,FILE_ANY_ACCESS)



BOOLEAN KernelMemCopy(PVOID dst, PVOID src, SIZE_T lenth, SIZE_T memsetlen)
{
	BOOLEAN Ret = FALSE;

	//////////////////////////////////////////////////////////////////////////
	KIRQL irql;
	KIRQL curreirql;

	if (dst)
	{
		curreirql = KeGetCurrentIrql();
		if (curreirql < DISPATCH_LEVEL)
		{
			irql = KeRaiseIrqlToDpcLevel();
		}
		UINT64 cr0 = __readcr0();
		cr0 &= 0xfffffffffffeffff;
		__writecr0(cr0);
		_disable();
		memset(dst, 0x90, memsetlen);
		memcpy(dst, src, lenth);
		cr0 = __readcr0();
		cr0 |= 0x10000;
		_enable();
		__writecr0(cr0);
		if (curreirql < DISPATCH_LEVEL)
		{
			KeLowerIrql(irql);
		}
		Ret = TRUE;
	}
	return Ret;
}


NTSTATUS DoMemBase(pMemStruct objData)
{
	PEPROCESS targetProcess;
	PVOID value = 0;
	NTSTATUS ntStatus = ((PsLookupProcessByProcessId_)FLAG_PsLookupProcessByProcessId)((HANDLE)objData->m_pid, &targetProcess);

	if (ntStatus != STATUS_SUCCESS || !targetProcess)
		return STATUS_FATAL_APP_EXIT;
	__try
	{
		((KeAttachProcess_)FLAG_KeAttachProcess)((PKPROCESS)targetProcess);
		value = ((PsGetProcessSectionBaseAddress_)FLAG_PsGetProcessSectionBaseAddress)(targetProcess);
		((KeDetachProcess_)FLAG_KeDetachProcess)();
		((ObfDereferenceObject_)FLAG_ObfDereferenceObject)(targetProcess);

		memcpy(objData->m_buffer, &value, sizeof(__int64));
	}
	__except (GetExceptionCode())
	{
		((ObfDereferenceObject_)FLAG_ObfDereferenceObject)(targetProcess);
		return STATUS_FATAL_APP_EXIT;
	}
	return STATUS_SUCCESS;
}

NTSTATUS DoMemRead(pMemStruct objData)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;

	
	IN PVOID BaseAddress = objData->m_addr;
	IN UINT32 BufferSize = objData->m_size;
	OUT PVOID Buffer = objData->m_buffer;

	PEPROCESS TargetProcess;
	PEPROCESS CurrentProcess;
	{
		if (((PCHAR)BaseAddress + BufferSize < (PCHAR)BaseAddress) ||
			((PCHAR)Buffer + BufferSize < (PCHAR)Buffer) ||
			((PVOID)((PCHAR)BaseAddress + BufferSize) > FLAG_MM_HIGHEST_USER_ADDRESS) ||
			((PVOID)((PCHAR)Buffer + BufferSize) > FLAG_MM_HIGHEST_USER_ADDRESS))
		{
			return STATUS_FATAL_APP_EXIT;
		}
		else
		{
			ntStatus = ((PsLookupProcessByProcessId_)FLAG_PsLookupProcessByProcessId)((HANDLE)objData->m_pid, &TargetProcess);
			if (NT_SUCCESS(ntStatus))
			{
				ntStatus = ((PsLookupProcessByProcessId_)FLAG_PsLookupProcessByProcessId)((HANDLE)objData->m_userPid, &CurrentProcess);
				if (NT_SUCCESS(ntStatus))
				{
					ULONG transferred;
					((MmCopyVirtualMemory_)FLAG_MmCopyVirtualMemory)(TargetProcess, BaseAddress, CurrentProcess, Buffer, BufferSize, KernelMode, &transferred);
					((ObfDereferenceObject_)FLAG_ObfDereferenceObject)(TargetProcess);
					((ObfDereferenceObject_)FLAG_ObfDereferenceObject)(CurrentProcess);
				}
				else
					((ObfDereferenceObject_)FLAG_ObfDereferenceObject)(TargetProcess);
			}
		}
	}
	return STATUS_SUCCESS;
}

NTSTATUS DoMemWrite(pMemStruct objData)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;


	IN PVOID BaseAddress = objData->m_addr;
	IN UINT32 BufferSize = objData->m_size;
	OUT PVOID Buffer = objData->m_buffer;

	PEPROCESS TargetProcess;
	PEPROCESS CurrentProcess;
	{
		if (((PCHAR)BaseAddress + BufferSize < (PCHAR)BaseAddress) ||
			((PCHAR)Buffer + BufferSize < (PCHAR)Buffer) ||
			((PVOID)((PCHAR)BaseAddress + BufferSize) > FLAG_MM_HIGHEST_USER_ADDRESS) ||
			((PVOID)((PCHAR)Buffer + BufferSize) > FLAG_MM_HIGHEST_USER_ADDRESS))
		{
			return STATUS_FATAL_APP_EXIT;
		}
		else
		{
			ntStatus = ((PsLookupProcessByProcessId_)FLAG_PsLookupProcessByProcessId)((HANDLE)objData->m_pid, &TargetProcess);
			if (NT_SUCCESS(ntStatus))
			{
				ntStatus = ((PsLookupProcessByProcessId_)FLAG_PsLookupProcessByProcessId)((HANDLE)objData->m_userPid, &CurrentProcess);
				if (NT_SUCCESS(ntStatus))
				{
					ULONG transferred;
					((MmCopyVirtualMemory_)FLAG_MmCopyVirtualMemory)(CurrentProcess, Buffer, TargetProcess, BaseAddress, BufferSize, KernelMode, &transferred);
					((ObfDereferenceObject_)FLAG_ObfDereferenceObject)(TargetProcess);
					((ObfDereferenceObject_)FLAG_ObfDereferenceObject)(CurrentProcess);
				}
				else
					((ObfDereferenceObject_)FLAG_ObfDereferenceObject)(TargetProcess);
			}
		}
	}
	return STATUS_SUCCESS;
}

NTSTATUS DoMemAlloc(pMemStruct objData)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;

	PEPROCESS TargetProcess;
	ntStatus = ((PsLookupProcessByProcessId_)FLAG_PsLookupProcessByProcessId)((HANDLE)objData->m_pid, &TargetProcess);
	ULONG nSize = objData->m_size;
	if (NT_SUCCESS(ntStatus))
	{
		//PVOID DriverBuffer = ExAllocatePoolWithTag(NonPagedPool, objData->m_size, 'Sys');

		//在目标进程申请内存
		((KeAttachProcess_)FLAG_KeAttachProcess)(TargetProcess);
		void* newAddr = NULL;
		if (!NT_SUCCESS(((ZwAllocateVirtualMemory_)FLAG_ZwAllocateVirtualMemory)(-1, (PVOID*)&newAddr, 0, &nSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)))
			ntStatus = STATUS_FATAL_APP_EXIT;
		else
			ntStatus = STATUS_SUCCESS;
		//*(ULONG64*)DriverBuffer = (ULONG64)newAddr;
		((KeDetachProcess_)FLAG_KeDetachProcess)();
		
		((ObfDereferenceObject_)FLAG_ObfDereferenceObject)(TargetProcess);

		memcpy(objData->m_buffer, &newAddr, sizeof(__int64));
/*
		if (NT_SUCCESS(ntStatus))
		{
			ntStatus = PsLookupProcessByProcessId((HANDLE)objData->m_userPid, &TargetProcess);
			KeStackAttachProcess(TargetProcess, &apc_state);
			memcpy(objData->m_buffer, &newAddr, sizeof(ULONG64));
			KeUnstackDetachProcess(&apc_state);
			ObfDereferenceObject(TargetProcess);
		}*/
		//ExFreePool(DriverBuffer);
	}
	return STATUS_SUCCESS;
}

NTSTATUS DoMemProtect(pMemStruct objData)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;

	IN UINT32 BufferSize = objData->m_size;
	OUT PVOID Buffer = objData->m_buffer;

	PEPROCESS TargetProcess;
	ntStatus = ((PsLookupProcessByProcessId_)FLAG_PsLookupProcessByProcessId)((HANDLE)objData->m_pid, &TargetProcess);
	if (NT_SUCCESS(ntStatus))
	{
		((KeAttachProcess_)FLAG_KeAttachProcess)(TargetProcess);
		//在目标进程申请内存
		ULONG64 newAddr = 0;
		if (!NT_SUCCESS(((ZwProtectVirtualMemory_)FLAG_ZwProtectVirtualMemory)(-1, (PVOID*)&newAddr, 0, &objData->m_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)))
			ntStatus = STATUS_FATAL_APP_EXIT;
		else
			ntStatus = STATUS_SUCCESS;
		objData->m_buffer = newAddr;
		((KeDetachProcess_)FLAG_KeDetachProcess)();
		((ObfDereferenceObject_)FLAG_ObfDereferenceObject)(TargetProcess);
	}
	return STATUS_SUCCESS;
}



NTSTATUS FASTCALL myIrpDispath(IN PDEVICE_OBJECT DeviceObject, IN OUT PIRP Irp)
{
	//DbgPrint("myIrpDispath ENTRY!");
	NTSTATUS NtStatus = STATUS_SUCCESS;

	PIO_STACK_LOCATION pIoStackIrp = NULL;
	pIoStackIrp = /*((IoGetCurrentIrpStackLocation_)FLAG_IoGetCurrentIrpStackLocation)*/Irp->Tail.Overlay.CurrentStackLocation;
	if (pIoStackIrp && pIoStackIrp->MajorFunction == IRP_MJ_DEVICE_CONTROL)
	{
		ULONG loControlCodes = pIoStackIrp->Parameters.DeviceIoControl.IoControlCode;//I/O控制代码
		if (loControlCodes >= IRQ_CODE_MEM_BASE)
		{
			Irp->IoStatus.Information = 0;
			Irp->IoStatus.Status = STATUS_SUCCESS;
			MyStruct* myData = (MyStruct*)FLAG_MYSTRUCT;
			switch (loControlCodes)
			{
			case IRQ_CODE_MEM_BASE:
			{
				ULONG64 FunAddr = myData->_DoMemBaseAddr;
				ULONG FunSize = myData->_DoMemBaseSize;
				INT FunKey = myData->_DoMemBaseKey;
				((EncryptDecryptBuf_)FLAG_EncryptDecryptBuf)(FunAddr, FunSize, FunKey);
#if DEBUG_TEST
				NtStatus = DoMemBase((pMemStruct)Irp->AssociatedIrp.SystemBuffer);
#else
				NtStatus = ((DoMemFun_)FunAddr)((pMemStruct)Irp->AssociatedIrp.SystemBuffer);
#endif
				myData->_DoMemBaseKey = (FunKey + 1) % 90 + 1;
				((EncryptDecryptBuf_)FLAG_EncryptDecryptBuf)(FunAddr, FunSize, myData->_DoMemBaseKey);
			}
			break;
			case IRQ_CODE_MEM_READ:
			{
				ULONG64 FunAddr = myData->_DoMemReadAddr;
				ULONG FunSize = myData->_DoMemReadSize;
				INT FunKey = myData->_DoMemReadKey;
				((EncryptDecryptBuf_)FLAG_EncryptDecryptBuf)(FunAddr, FunSize, FunKey);
#if DEBUG_TEST
				NtStatus = DoMemRead((pMemStruct)Irp->AssociatedIrp.SystemBuffer);
#else
				NtStatus = ((DoMemFun_)FunAddr)((pMemStruct)Irp->AssociatedIrp.SystemBuffer);
#endif
				myData->_DoMemReadKey = (FunKey + 1) % 91 + 3;
				((EncryptDecryptBuf_)FLAG_EncryptDecryptBuf)(FunAddr, FunSize, myData->_DoMemReadKey);
			}
			break;
			case IRQ_CODE_MEM_WRITE:
			{
				ULONG64 FunAddr = myData->_DoMemWriteAddr;
				ULONG FunSize = myData->_DoMemWriteSize;
				INT FunKey = myData->_DoMemWriteKey;
				((EncryptDecryptBuf_)FLAG_EncryptDecryptBuf)(FunAddr, FunSize, FunKey);
#if DEBUG_TEST
				NtStatus = DoMemWrite((pMemStruct)Irp->AssociatedIrp.SystemBuffer);
#else
				NtStatus = ((DoMemFun_)FunAddr)((pMemStruct)Irp->AssociatedIrp.SystemBuffer);
#endif
				myData->_DoMemWriteKey = (FunKey + 1) % 92 + 5;
				((EncryptDecryptBuf_)FLAG_EncryptDecryptBuf)(FunAddr, FunSize, myData->_DoMemWriteKey);
			}
			break;
			case IRQ_CODE_MEM_ALLOC:
			{
				ULONG64 FunAddr = myData->_DoMemAllocAddr;
				ULONG FunSize = myData->_DoMemAllocSize;
				INT FunKey = myData->_DoMemAllocKey;
				((EncryptDecryptBuf_)FLAG_EncryptDecryptBuf)(FunAddr, FunSize, FunKey);
#if DEBUG_TEST
				NtStatus = DoMemAlloc((pMemStruct)Irp->AssociatedIrp.SystemBuffer);
#else
				NtStatus = ((DoMemFun_)FunAddr)((pMemStruct)Irp->AssociatedIrp.SystemBuffer);
#endif
				myData->_DoMemAllocKey = (FunKey + 1) % 93 + 7;
				((EncryptDecryptBuf_)FLAG_EncryptDecryptBuf)(FunAddr, FunSize, myData->_DoMemAllocKey);
			}
			break;
			case IRQ_CODE_MEM_PROTECT:
			{
				ULONG64 FunAddr = myData->_DoMemProtectAddr;
				ULONG FunSize = myData->_DoMemProtectSize;
				INT FunKey = myData->_DoMemProtectKey;
				((EncryptDecryptBuf_)FLAG_EncryptDecryptBuf)(FunAddr, FunSize, FunKey);
#if DEBUG_TEST
				NtStatus = DoMemProtect((pMemStruct)Irp->AssociatedIrp.SystemBuffer);
#else
				NtStatus = ((DoMemFun_)FunAddr)((pMemStruct)Irp->AssociatedIrp.SystemBuffer);
#endif
				myData->_DoMemProtectKey = (FunKey + 1) % 94 + 9;
				((EncryptDecryptBuf_)FLAG_EncryptDecryptBuf)(FunAddr, FunSize, myData->_DoMemProtectKey);
			}
			break;
			}
			((IofCompleteRequest_)FLAG_IofCompleteRequest)(Irp, IO_NO_INCREMENT);
			return NtStatus;
		}
	}
	
	((IofCompleteRequest_)FLAG_IofCompleteRequest)(Irp, IO_NO_INCREMENT);
	return NtStatus;
/*
	PDRIVER_DISPATCH oldEnter = (ULONG64)g_luOldAddr + 0xF;
	NTSTATUS status  =  oldEnter(DeviceObject, Irp);
	return status;*/
}


