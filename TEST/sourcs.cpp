#include "../inc/sockets.h"

DRIVER_UNLOAD DriverUnload;
extern "C"
NTSTATUS DriverEntry(_In_ DRIVER_OBJECT * DriverObject, _In_ PUNICODE_STRING RegistryPath){
	__debugbreak();


	WinShell::sockets::DatagramSocket socket(L"192.168.0.31", L"1337");
	char msg[20] = "Hello World";
	socket.SendData(msg, L"192.168.0.31", L"1337");



	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);
	DriverObject->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}

void DriverUnload(PDRIVER_OBJECT) {
}