#include "../inc/sockets.h"

DRIVER_UNLOAD DriverUnload;
HANDLE Thread;
void ThreadOne(
	PVOID
) {
	WinShell::sockets::ConnectionSocket socket;
	WinShell::endpoint endpoint(L"192.168.0.31", L"1337");
	char msg[] = "Hello World!!!!";
	socket.Connect(endpoint);
	auto status = socket.SendData(msg,sizeof(msg));
	PsTerminateSystemThread(status);
}
extern "C"
NTSTATUS DriverEntry(_In_ DRIVER_OBJECT * DriverObject, _In_ PUNICODE_STRING RegistryPath){
	__debugbreak();
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