#include "inlineHook.h"


#define kmalloc(_s) ExAllocatePoolWithTag(NonPagedPool, _s, 'SYSW')  

#define kfree(_p) ExFreePool(_p)  



KIRQL WPOFFx64()
{
	KIRQL irql = KeRaiseIrqlToDpcLevel();
	UINT64 cr0 = __readcr0();
	cr0 &= 0xfffffffffffeffff;
	__writecr0(cr0);
	_disable();
	return irql;

}

void WPONx64(KIRQL irql)
{
	UINT64 cr0 = __readcr0();
	cr0 |= 0x10000;
	_enable();
	__writecr0(cr0);
	KeLowerIrql(irql);
}

ULONG GetPatchSize(PUCHAR Address)
{
	return 15;
}

  
void HookKernelApi(IN void* ApiAddress, IN void* Proxy_ApiAddress, OUT void* *Original_ApiAddress, OUT ULONG *PatchSize)
{

	KIRQL irql;
	UINT64 tmpv;
	PVOID head_n_byte, ori_func;
	UCHAR jmp_code[] = "\xFF\x25\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
	UCHAR jmp_code_orifunc[] = "\xFF\x25\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";

	//How many bytes shoule be patch  
	*PatchSize = GetPatchSize((PUCHAR)ApiAddress);

	//step 1: Read current data  
	head_n_byte = kmalloc(*PatchSize);

	irql = WPOFFx64();
	memcpy(head_n_byte, ApiAddress, *PatchSize);
	WPONx64(irql);

	//step 2: Create ori function  
	ori_func = ExAllocatePoolWithTag(NonPagedPool, PAGE_SIZE, 0);
	//ori_func = kmalloc(*PatchSize + 14);    //原始机器码+跳转机器码  
	RtlFillMemory(ori_func, *PatchSize + 14, 0x90);
	tmpv = (ULONG64)ApiAddress + *PatchSize;    //跳转到没被打补丁的那个字节  
	memcpy(jmp_code_orifunc + 6, &tmpv, 8);
	memcpy((PUCHAR)ori_func, head_n_byte, *PatchSize);
	kfree(head_n_byte);
	memcpy((PUCHAR)ori_func + *PatchSize, jmp_code_orifunc, 14);
	*Original_ApiAddress = ori_func;
	//step 3: fill jmp code  
	tmpv = (UINT64)Proxy_ApiAddress;
	memcpy(jmp_code + 6, &tmpv, 8);

	//step 4: Fill NOP and hook  
	irql = WPOFFx64();
	RtlFillMemory(ApiAddress, *PatchSize, 0x90);
	memcpy(ApiAddress, jmp_code, 14);
	WPONx64(irql);

	return;
}


void UnhookKernelApi(IN void* ApiAddress, IN void* OriCode, IN ULONG PatchSize)
{
	KIRQL irql;
	irql = WPOFFx64();
	memcpy(ApiAddress, OriCode, PatchSize);
	WPONx64(irql);
	//后续还要释放申请的内存等
	//kfree(OriCode);
	ExFreePoolWithTag(OriCode, 0);
}