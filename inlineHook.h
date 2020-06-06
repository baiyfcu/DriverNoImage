#include <ntddk.h>

KIRQL WPOFFx64();
void WPONx64(KIRQL irql);


//传入：待HOOK函数地址，代理函数地址，接收原始函数地址的指针，接收补丁长度的指针；返回：原来头N字节的数据
void HookKernelApi(IN void* ApiAddress, IN void* Proxy_ApiAddress, OUT void* *Original_ApiAddress, OUT ULONG *PatchSize);

//传入：被HOOK函数地址，原始数据，补丁长度  
void UnhookKernelApi(IN void* ApiAddress, IN void* OriCode, IN ULONG PatchSize);