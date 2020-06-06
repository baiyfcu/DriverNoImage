#include <ntdef.h>
#include <ntifs.h>

#include "inlineHook.h"
#include "patchFun.h"
#include "replaceData.h"

extern NTSTATUS FASTCALL _myIrpDispath(IN PDEVICE_OBJECT DeviceObject, IN OUT PIRP Irp);

NTKERNELAPI NTSTATUS ObReferenceObjectByName(
	IN PUNICODE_STRING ObjectName,
	IN ULONG Attributes,
	IN PACCESS_STATE PassedAccessState OPTIONAL,
	IN ACCESS_MASK DesiredAccess OPTIONAL,
	IN POBJECT_TYPE ObjectType,
	IN KPROCESSOR_MODE AccessMode,
	IN OUT PVOID ParseContext OPTIONAL,
	OUT PVOID *Object
);
extern POBJECT_TYPE* IoDriverObjectType;

//////////////////////////////////////////////////////////////////////////







void* g_luOldAddr = NULL;
void* g_originalAddr = NULL;
ULONG g_patchSize = NULL;
PVOID g_myIrpAddrNew;




void HookLuDashi()
{
	
	NTSTATUS result;
	UNICODE_STRING DestinationString;
	//\Driver\driverMain
	//加了最后一位随机 a-z
	wchar_t linkPath[] = { 92,68,114,105,118,101,114,92,100,117,109,112,95,100,117,109,112,108,100,115,0,0 };
	PDRIVER_OBJECT pDriver = NULL;
	for (int i = 0; i < 26; i++)
	{
		linkPath[20] = 97 + i;
		RtlInitUnicodeString(&DestinationString, linkPath/*L"\\Driver\\ComputerZ"*/);

		
		result = ObReferenceObjectByName(&DestinationString,
			OBJ_CASE_INSENSITIVE, NULL, NULL,
			*IoDriverObjectType, KernelMode, NULL, (PVOID)&pDriver);
		if (NT_SUCCESS(result))
			break;
		pDriver = NULL;
	}
	

	//"pDriver:%p result:%lx IoDeviceObjectType:%p\n"
	char printStr[] = { 112,68,114,105,118,101,114,58,37,112,32,114,101,115,117,108,116,58,37,108,120,32,73,111,68,101,118,105,99,101,79,98,106,101,99,116,84,121,112,101,58,37,112,92,110,0 };
	DbgPrint(printStr, pDriver, result, IoDriverObjectType);

	if (NT_SUCCESS(result))
	{
		KIRQL oldIrql;
		g_luOldAddr = pDriver->MajorFunction[IRP_MJ_DEVICE_CONTROL];

		KIRQL irql = WPOFFx64();
		__int32 hookFlag = *(__int32*)g_luOldAddr;
		WPONx64(irql);

		if (hookFlag != 0x25FF)
		{
			InitFunTable();
			//HOOK开始
			g_myIrpAddrNew = CopyMyFun(_myIrpDispath, NULL);
			HookKernelApi(g_luOldAddr, g_myIrpAddrNew, &g_originalAddr, &g_patchSize);
		}
		else
		{
			//"Driver had run...\n"
			char printStr[] = { 68,114,105,118,101,114,32,104,97,100,32,114,117,110,46,46,46,92,110,0 };
			DbgPrint(printStr);
		}
			
		
		ObfDereferenceObject(pDriver);
	}




/*
	PDEVICE_OBJECT DeviceObject = NULL;
	PIRP Irp = NULL;
	myIrpDispath(DeviceObject, Irp);
	DoMemBase(NULL);
	DoMemRead(NULL);
	DoMemWrite(NULL);
	DoMemAlloc(NULL);
	DoMemProtect(NULL);
	EncryptDecryptBuf(NULL, NULL, NULL);*/
	
}

void UnHookLuDashi()
{
	if (g_originalAddr)
	{
		UnhookKernelApi(g_luOldAddr, g_originalAddr, g_patchSize);
		ExFreePoolWithTag(g_myIrpAddrNew, 0);
	}
}


NTSTATUS FASTCALL myIrpDispathTest(IN PDEVICE_OBJECT DeviceObject, IN OUT PIRP Irp)
{
	//DbgPrint("myIrpDispath ENTRY!");
	NTSTATUS NtStatus = STATUS_SUCCESS;

	PIO_STACK_LOCATION pIoStackIrp = NULL;
	pIoStackIrp = /*((IoGetCurrentIrpStackLocation_)FLAG_IoGetCurrentIrpStackLocation)*/Irp->Tail.Overlay.CurrentStackLocation;
	if (pIoStackIrp && pIoStackIrp->MajorFunction == IRP_MJ_CREATE)
	{
		ULONG loControlCodes = pIoStackIrp->Parameters.DeviceIoControl.IoControlCode;//I/O控制代码
		DbgPrint("myIrpDispath ENTRY!");
		if (loControlCodes == -1)
		{
			Irp->IoStatus.Information = 0;
			Irp->IoStatus.Status = STATUS_SUCCESS;
			MyStruct* myData = (MyStruct*)FLAG_MYSTRUCT;
			IofCompleteRequest(Irp, IO_NO_INCREMENT);
			return NtStatus;
		}
	}

/*
	((IofCompleteRequest_)FLAG_IofCompleteRequest)(Irp, IO_NO_INCREMENT);
	return NtStatus;*/
	
	PDRIVER_DISPATCH oldEnter = (ULONG64)g_luOldAddr + 0xF;
	NTSTATUS status  =  oldEnter(DeviceObject, Irp);
	return status;
}

void HookNTFS()
{
	NTSTATUS result;
	UNICODE_STRING DestinationString;

	PDRIVER_OBJECT pDriver = NULL;

	RtlInitUnicodeString(&DestinationString, L"\\FileSystem\\Ntfs");


	result = ObReferenceObjectByName(&DestinationString,
		OBJ_CASE_INSENSITIVE, NULL, NULL,
		*IoDriverObjectType, KernelMode, NULL, (PVOID)&pDriver);


	DbgPrint("pDriver:%p result:%lx IoDeviceObjectType:%p\n", pDriver, result, IoDriverObjectType);

	if (NT_SUCCESS(result))
	{
		KIRQL oldIrql;
		g_luOldAddr = pDriver->MajorFunction[IRP_MJ_CREATE];

		KIRQL irql = WPOFFx64();
		__int32 hookFlag = *(__int32*)g_luOldAddr;
		WPONx64(irql);

		if (hookFlag != 0x25FF)
		{
			//HOOK开始
			HookKernelApi(g_luOldAddr, myIrpDispathTest, &g_originalAddr, &g_patchSize);
		}
		else
		{
			//"Driver had run...\n"
			char printStr[] = { 68,114,105,118,101,114,32,104,97,100,32,114,117,110,46,46,46,92,110,0 };
			DbgPrint(printStr);
		}


		ObfDereferenceObject(pDriver);
	}

}

void UnHookNTFS()
{
	if (g_originalAddr)
	{
		UnhookKernelApi(g_luOldAddr, g_originalAddr, g_patchSize);
	}
}