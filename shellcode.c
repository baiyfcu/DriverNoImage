#include <ntddk.h>
#include <ntifs.h>

extern PVOID OrgiDispatch;

extern PVOID DriverDisPath;

INT CalcShellcodeSize(UCHAR* adr)
{
	UCHAR *orgi = adr;
	while (*adr != 0xC3)
	{
		adr++;
	}
	return adr - orgi;
}

BOOLEAN replacedata(ULONG64 *Original, ULONG64 Now)
{
	while (*Original != Now)
	{
		Original = Original + 8;
	}
	if (Original)
	{
		*Original = Now;
	}
}

INT CalcWdkoptimizationCodeSize(UCHAR *adr) {
	UCHAR *orgi = adr;
	// C3 CC CC CC CC
	//ffff880`0307c3db c3              ret
	//	fffff880`0307c3dc cc              int     3
	//	fffff880`0307c3dd cc              int     3
	//fffff880`0307c3de cc              int     3
	//fffff880`0307c3df cc              int     3

	while (adr[0] != 0XC3 && adr[1] != 0xCC && adr[2] != 0xCC && adr[3] != 0xCC && adr[4] != 0xCC)
	{
		adr++;
	}
	return (adr - orgi) + 3;

}



BOOLEAN replacedata(UCHAR *Original, ULONG64 Orig, ULONG64 Now)
{
	UCHAR *UL = &Orig;
	INT Count = 0x1000;
	INT NowCount = 0;
	while (NowCount <= Count)
	{
		NowCount++;
		if (Original[0] == UL[0] && Original[1] == UL[1] && Original[2] == UL[2] && Original[3] == UL[3] && Original[4] == UL[4])
		{
			break;
		}
		Original++;
	}
	if (Original)
	{
		*(ULONG64*)Original = Now;
		return TRUE;
	}
	return FALSE;
}


NTKERNELAPI
NTSTATUS
ObReferenceObjectByName(
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

typedef NTSTATUS (*MmCopyVirtualMemoryx)(IN ULONG FromProcess, IN CONST VOID *FromAddress, IN ULONG ToProcess, OUT PVOID ToAddress, IN SIZE_T BufferSize, IN KPROCESSOR_MODE PreviousMode, OUT PSIZE_T NumberOfBytesCopied);

PDRIVER_DISPATCH gfn_OrigReadCompleteRoutine;
PDRIVER_OBJECT g_FilterDriverObject = NULL;
void startTask()
{
	NTSTATUS Status;
	UNICODE_STRING DestinationString;
	RtlInitUnicodeString(&DestinationString, L"\\Driver\\ComputerZ");

	
	Status = ObReferenceObjectByName(&DestinationString,
		OBJ_CASE_INSENSITIVE, NULL, NULL,
		*IoDriverObjectType, KernelMode, NULL, (PVOID)&g_FilterDriverObject);

	//result = IoGetDeviceObjectPointer( &DestinationString, FILE_ALL_ACCESS, &FileObject, &pDriver);
	DbgPrint("hookLudashiFsk pDriver:%p result:%lx IoDeviceObjectType:%p\n", g_FilterDriverObject, Status, IoDriverObjectType);

	if (NT_SUCCESS(Status))
	{
		UNICODE_STRING routineName;
		RtlInitUnicodeString(&routineName, L"MmCopyVirtualMemory");
		MmCopyVirtualMemoryx MmCopyVirtualMemoryRoutine = (MmCopyVirtualMemoryx)MmGetSystemRoutineAddress(&routineName);

		//计算原函数并且填充
		PVOID Orgi = ExAllocatePoolWithTag(NonPagedPool, PAGE_SIZE, 0);
		SIZE_T codesize = CalcShellcodeSize(&OrgiDispatch);
		memcpy(Orgi, &OrgiDispatch, codesize); //计算大小且复制Orgishellcode
		gfn_OrigReadCompleteRoutine = g_FilterDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL];
		gfn_OrigReadCompleteRoutine = (ULONG64)gfn_OrigReadCompleteRoutine + 0x18;
		replacedata(Orgi, 0x98888800c9e8f000, gfn_OrigReadCompleteRoutine);

		//////////////////////////////////////////////////////////////////////////
		///填充功能函数
		PVOID read = ExAllocatePoolWithTag(NonPagedPool, 0x1000, 0);
		codesize = CalcShellcodeSize(&write);
		memcpy(read, &Write, codesize);
		replacedata(read, 0x788777009e8f000, &PsLookupProcessByProcessId);
		replacedata(read, 0x688777009e8f000, &PsLookupProcessByProcessId);
		replacedata(read, 0x588777009e8f000, &MmCopyVirtualMemoryRoutine);
		replacedata(read, 0x488777009e8f000, &ObfDereferenceObject);
		replacedata(read, 0x388777009e8f000, &ObfDereferenceObject);
		replacedata(read, 0x288777009e8f000, &ObfDereferenceObject);

		//计算ShellCodeFunc 填充 Read Write 修正
		PVOID driverdis = ExAllocatePoolWithTag(NonPagedPool, 0x1000, 0);
		codesize = CalcShellcodeSize(&DriverDisPath);
		memcpy(driverdis, &DriverDisPath, codesize);

		replacedata(driverdis, 0x78888800c9e8f000, read);//read
		replacedata(driverdis, 0x68888800c9e8f000, write);//write
		replacedata(driverdis, 0x88888800c9e8f000, Orgi);//orig dis

		DbgPrint("shellcode:%p\n", driverdis);

		//Hook Com..z
		memcpy(jmp_code + 6, &driverdis, 8);;
		PDRIVER_DISPATCH Adr = g_FilterDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL];
		KernelMemCopy(Adr, jmp_code, 14, 0x18);

		ObDereferenceObject(g_FilterDriverObject);     //清除引用计数
	}
}

void stopTask()
{
	mem_protect_close();
	memcpy((PVOID)lpHookInfo->fnOrigAddress, lpHookInfo->origCode, HOOK_INST_LEN);
	mem_protect_open();
}