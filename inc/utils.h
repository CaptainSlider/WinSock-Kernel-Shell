#pragma once

#include "WinShell.h"

NTSTATUS IoCompletionRoutine(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp,
	PVOID Context
);
NTSTATUS IoCompletionRoutinew(
	PDEVICE_OBJECT,
	PIRP,
	PVOID Context
);
namespace WinShell::utils {

	//common
	NTSTATUS InitWsk(PWSK_REGISTRATION wskRegistretion, PWSK_PROVIDER_NPI wskProviderNpi);
	void DeregisterWsk(PWSK_REGISTRATION wskRegistretion);
	SOCKADDR ConvertStringToAdress(WSK_PROVIDER_NPI wskNpi, endpoint StringAdress);
	void CloseSocket(PWSK_SOCKET sock);

	//Listener Socket
	NTSTATUS AcceptConnection(PWSK_SOCKET sock);
	NTSTATUS BindListenerSocket(PWSK_SOCKET sock);
	NTSTATUS CreateListeningSocket(WSK_PROVIDER_NPI wskProviderNpi, PWSK_SOCKET* sock);

	//Stream Socket
	NTSTATUS CreateStreamSocket(PWSK_PROVIDER_NPI wskProviderNpi, PWSK_SOCKET* sock);
	NTSTATUS BindStreamSocket(PWSK_SOCKET sock);
	NTSTATUS AcceptConnectionStream(PWSK_SOCKET sock);

	//Connection Socket
	NTSTATUS BindConnectionSocket(PWSK_SOCKET sock);
	NTSTATUS ConnectConnectionSocket(PWSK_SOCKET sock, PSOCKADDR ConnectionAdress);
	NTSTATUS CreateConnectionSocket(PWSK_PROVIDER_NPI wskProviderNpi, PWSK_SOCKET* sock);
	NTSTATUS SendDataConnectionSocket(PWSK_SOCKET sock, PVOID Data, ULONG DataSize);
	NTSTATUS GetDataConnectionSocket(PWSK_SOCKET sock, PVOID Data, ULONG DataSize);
	NTSTATUS DisconnectConnectionSocket(PWSK_SOCKET sock);

	//Datagrame Socket
	NTSTATUS CreateDatagramSocket(PWSK_PROVIDER_NPI wskProviderNpi, PWSK_SOCKET* sock);
	NTSTATUS BindDatagramSocket(PWSK_SOCKET sock);
	NTSTATUS SendDataDatagramSocket(PWSK_SOCKET sock, PVOID Data, ULONG DataSize, PSOCKADDR ConnectionAddress);
	NTSTATUS GetDataDatagramSocket(PWSK_SOCKET sock, PVOID Data, ULONG DataSize);

}

