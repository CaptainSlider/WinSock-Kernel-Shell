#pragma once

#include <ntddk.h>
#include <wsk.h>

NTSTATUS IoCompletionRoutine(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp,
	PVOID Context
);

namespace utils {

	NTSTATUS InitWsk(PWSK_REGISTRATION wskRegistretion, PWSK_PROVIDER_NPI wskProviderNpi);
	void DeregisterWsk(PWSK_REGISTRATION wskRegistretion);

	//Listener Socket
	NTSTATUS AcceptConnection(PWSK_SOCKET sock);
	NTSTATUS BindListenerSocket(PWSK_SOCKET sock, PSOCKADDR LocalAdress);
	NTSTATUS CreateListeningSocket(WSK_PROVIDER_NPI wskProviderNpi, PWSK_SOCKET* sock);

	//Connection Socket
	NTSTATUS BindConnectionSocket(PWSK_SOCKET sock, PSOCKADDR LocalAdress);
	NTSTATUS ConnectConnectionSocket(PWSK_SOCKET sock, PSOCKADDR ConnectionAdress);
	NTSTATUS CreateConnectionSocket(PWSK_PROVIDER_NPI wskProviderNpi, PWSK_SOCKET* sock);

	//Datagrame Socket
	NTSTATUS CreateDatagramSocket(PWSK_PROVIDER_NPI wskProviderNpi, PWSK_SOCKET* sock);
	NTSTATUS BindDatagramSocket(PWSK_SOCKET sock, PSOCKADDR Adress);
	NTSTATUS SendDataDtagramSocket(PWSK_SOCKET sock, PVOID Data, ULONG DataSize, PSOCKADDR ConnectionAddress);

	SOCKADDR ConvertStringToAdress(WSK_PROVIDER_NPI wskNpi, WCHAR* stringAdress, WCHAR* stringPort);

}

