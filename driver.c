#include <ntdef.h>
#include <ntifs.h>
#include "patch.h"



VOID Unload(PDRIVER_OBJECT  pDriverObject)
{
	//UnHookLuDashi();
	//DbgPrint("DriverNoImage Unload.\n");
	char printStr[] = { 68,114,105,118,101,114,32,85,110,108,111,97,100,46,92,110,0 };
	DbgPrint(printStr);
	UnHookNTFS();
}



NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT  pDriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	//HideDriver(pDriverObject);
	//"Driver entry.\n"
	char printStr[] = { 68,114,105,118,101,114,32,101,110,116,114,121,46,92,110,0 };
	DbgPrint(printStr);
	NTSTATUS status = STATUS_SUCCESS;
	pDriverObject->DriverUnload = Unload;
	HookNTFS();
	return status;
/*
	HookLuDashi();
#if DEBUG_TEST
	return STATUS_SUCCESS;
#else
	return STATUS_NOT_FOUND;
#endif*/
	
}