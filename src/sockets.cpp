#include "../inc/sockets.h"

namespace WinShell::sockets {

	//////// Listener Socket
	_ListenerSocket::_ListenerSocket() {
		KdPrint(("_ListenerSocket::_ListenerSocket(x)"));
		auto status = utils::InitWsk(&_wskReg, &_providNpi);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed _ListenerSocket::_ListenerSocket(x)::InitWsk (0x%08x)", status));
			return;
		}
		status = utils::CreateListeningSocket(_providNpi, &_sock);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed _ListenerSocket::_ListenerSocket(x)::CreateListeningSocket (0x%08x)", status));
			return;
		}
		status = utils::BindListenerSocket(_sock);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed _ListenerSocket::_ListenerSocket(x)::BindSocket (0x%08x)", status));
			return;
		}
	}

	NTSTATUS _ListenerSocket::Accept() {
		KdPrint(("_ListenerSocket::Accept"));
		return utils::AcceptConnection(_sock);
	}

	_ListenerSocket::~_ListenerSocket() {
		KdPrint(("_ListenerSocket::~_ListenerSocket()"));
		utils::DeregisterWsk(&_wskReg);
	}
	
	////////Connection Socket

	_ConnectionSocket::_ConnectionSocket() {
		KdPrint(("_ConnectionSocket::_ConnectionSocket(x)"));
		auto status = utils::InitWsk(&_wskReg, &_providNpi);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed _ConnectionSocket::_ConnectionSocket(x)::InitWsk (0x%08x)", status));
			return;
		}
		status = utils::CreateConnectionSocket(&_providNpi, &_sock);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed _ConnectionSocket::_ConnectionSocket(x)::CreateListeningSocket (0x%08x)", status));
			return;
		}
		status = utils::BindConnectionSocket(_sock);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed _ConnectionSocket::_ConnectionSocket(x)::BindSocket (0x%08x)", status));
			return;
		}
	}

	NTSTATUS _ConnectionSocket::Connect(endpoint ConnectionAdress) {
		SOCKADDR adress = utils::ConvertStringToAdress(_providNpi, ConnectionAdress);
		KdPrint(("_ConnectionSocket::Connect()"));
		return utils::ConnectConnectionSocket(_sock, &adress);
	}

	NTSTATUS _ConnectionSocket::SendData(PVOID Data, ULONG DataSize) {
		return utils::SendDataConnectionSocket(_sock, Data, DataSize);
	}

	_ConnectionSocket::~_ConnectionSocket() {
		KdPrint(("_ConnectionSocket::~_ConnectionSocket()"));
		utils::DeregisterWsk(&_wskReg);
	}

	///DatagramSocket

	_DatagramSocket::_DatagramSocket() {
		KdPrint(("_DatagramSocket::_DatagramSocket(x)"));
		auto status = utils::InitWsk(&_wskReg, &_providNpi);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed _DatagramSocket::_DatagramSocket(x)::InitWsk (0x%08x)", status));
			return;
		}
		status = utils::CreateDatagramSocket(&_providNpi, &_sock);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed __DatagramSocket::_DatagramSocket(x)::CreateListeningSocket (0x%08x)", status));
			return;
		}
		status = utils::BindDatagramSocket(_sock);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed _DatagramSocket::_DatagramSocket(x)::BindSocket (0x%08x)", status));
			return;
		}
	}


	NTSTATUS _DatagramSocket::SendData(PVOID Data, endpoint ConnectionAdress){
		SOCKADDR adress = utils::ConvertStringToAdress(_providNpi, ConnectionAdress);
		return utils::SendDataDatagramSocket(_sock, Data, sizeof(Data), &adress);
	}

	NTSTATUS _DatagramSocket::GetData(PVOID Data) {
		return utils::GetDataDatagramSocket(_sock, Data, sizeof(Data));
	}
	
	_DatagramSocket::~_DatagramSocket() {
		KdPrint(("_DatagramSocket::~_DatagramSocket()"));
		utils::DeregisterWsk(&_wskReg);
	}

	void _DatagramSocket::Unregister() {
		utils::DeregisterWsk(&_wskReg);
	}
}
