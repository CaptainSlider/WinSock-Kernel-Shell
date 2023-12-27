#include "../inc/sockets.h"

DRIVER_UNLOAD DriverUnload;
HANDLE Thread;
void ThreadOne(
	PVOID
) {
	__debugbreak();
	WinShell::sockets::tcp::Socket socket;
	WinShell::endpoint endpoint(L"192.168.43.128", L"1337");
	socket.Connect(endpoint);
	char msg[] = "Hello";
	socket.SendData(msg, sizeof(msg));
	char buffer[50] = { 0 };
	auto status = socket.ReceivData(buffer, sizeof(buffer));
	KdPrint(("status = 0x%08X", status));
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