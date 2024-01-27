#include "../inc/sockets.h"

DRIVER_UNLOAD DriverUnload;
HANDLE Thread;
void ThreadOne(
	PVOID
) {
	__crt_init();
	NTSTATUS status = STATUS_SUCCESS;
	try
	{
		globals::mem_manager = new hh::tlsf_allocator();
		WinShell::sockets::tcp::Socket socket;
		WinShell::endpoint endpoint(L"192.168.0.31", L"1337");
		socket.Connect(endpoint);
		char msg[] = "Hello";
		socket.SendData(msg, sizeof(msg));
		char buffer[50] = { 0 };
		status = socket.ReceivData(buffer, sizeof(buffer));
		KdPrint(("%s\n", buffer));
	}
	catch (WinShell::exception & e)
	{
		globals::mem_manager->~memory_manager();
		auto* pTemp = globals::mem_manager;
		globals::mem_manager = nullptr;
		operator delete(pTemp);
		__crt_deinit();
		KdPrint(("Failed: %s     (status = 0x%08X) \n", e.e.what(), e.status));
		PsTerminateSystemThread(e.status);
	}
	if (globals::mem_manager != nullptr)
	{
		globals::mem_manager->~memory_manager();
		auto* pTemp = globals::mem_manager;
		globals::mem_manager = nullptr;
		operator delete(pTemp);
	}
	__crt_deinit();
	PsTerminateSystemThread(status);
}
extern "C"
NTSTATUS DriverEntry(_In_ DRIVER_OBJECT * DriverObject, _In_ PUNICODE_STRING RegistryPath){

	OBJECT_ATTRIBUTES threadAttributes;
	InitializeObjectAttributes(&threadAttributes, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);
	PsCreateSystemThread(&Thread, THREAD_ALL_ACCESS, &threadAttributes, NULL, NULL, ThreadOne, NULL);
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);
	DriverObject->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}

void DriverUnload(PDRIVER_OBJECT) {
	ZwClose(Thread);
}