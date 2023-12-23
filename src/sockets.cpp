#include "../inc/sockets.h"

namespace WinShell::sockets {

	//////// Listener Socket
	_ListenerSocket::_ListenerSocket(PSOCKADDR LocalAdress) {
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
		status = utils::BindListenerSocket(_sock, LocalAdress);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed _ListenerSocket::_ListenerSocket(x)::BindSocket (0x%08x)", status));
			return;
		}
	}

	_ListenerSocket::_ListenerSocket() {
	}

	NTSTATUS _ListenerSocket::InitSock(PSOCKADDR LocalAdress) {
		KdPrint(("_ListenerSocket::InitSock()"));

		auto status = utils::InitWsk(&_wskReg, &_providNpi);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed _ListenerSocket::InitSock():InitWsk (0x%08x)", status));
			return status;
		}

		status = utils::CreateListeningSocket(_providNpi, &_sock);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed _ListenerSocket::InitSock()::CreateListeningSocket (0x%08x)", status));
			return status;
		}

	    status = utils::BindListenerSocket(_sock, LocalAdress);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed _ListenerSocket::InitSock()::BindSocket (0x%08x)", status));
		}

		return status;
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

	_ConnectionSocket::_ConnectionSocket(PSOCKADDR LocalAdress) {
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
		status = utils::BindConnectionSocket(_sock, LocalAdress);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed _ConnectionSocket::_ConnectionSocket(x)::BindSocket (0x%08x)", status));
			return;
		}
	}

	_ConnectionSocket::_ConnectionSocket() {
		return;
	}

	NTSTATUS _ConnectionSocket::InitSock(PSOCKADDR LocalAdress) {
		KdPrint(("_ConnectionSocket::InitSock()"));

		auto status = utils::InitWsk(&_wskReg, &_providNpi);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed _ConnectionSocket::InitSock():InitWsk (0x%08x)", status));
			return status;
		}

		status = utils::CreateConnectionSocket(&_providNpi, &_sock);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed _ConnectionSocket::InitSock()::CreateListeningSocket (0x%08x)", status));
			return status;
		}

		status = utils::BindConnectionSocket(_sock, LocalAdress);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed ConnectionSocket::InitSock()::BindSocket (0x%08x)", status));
		}

		return status;
	}

	NTSTATUS _ConnectionSocket::Connect(PSOCKADDR ConnectionAdress) {
		KdPrint(("_ConnectionSocket::Connect()"));
		return utils::ConnectConnectionSocket(_sock, ConnectionAdress);
	}

	_ConnectionSocket::~_ConnectionSocket() {
		KdPrint(("_ConnectionSocket::~_ConnectionSocket()"));
		utils::DeregisterWsk(&_wskReg);
	}

	///DatagramSocket

	_DatagramSocket::_DatagramSocket() {
		return;
	}

	_DatagramSocket::_DatagramSocket(WCHAR* LocalAdress, WCHAR* Port) {
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
		SOCKADDR adress = utils::ConvertStringToAdress(_providNpi, LocalAdress, Port);
		status = utils::BindDatagramSocket(_sock, &adress);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed _DatagramSocket::_DatagramSocket(x)::BindSocket (0x%08x)", status));
			return;
		}
	}

	NTSTATUS _DatagramSocket::InitSock(WCHAR* LocalAdress, WCHAR* Port) {
		KdPrint(("_DatagramSocket::_DatagramSocket(x)"));
		auto status = utils::InitWsk(&_wskReg, &_providNpi);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed _DatagramSocket::_DatagramSocket(x)::InitWsk (0x%08x)", status));
			return status;
		}
		status = utils::CreateDatagramSocket(&_providNpi, &_sock);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed __DatagramSocket::_DatagramSocket(x)::CreateListeningSocket (0x%08x)", status));
			return status;
		}
		SOCKADDR adress = utils::ConvertStringToAdress(_providNpi, LocalAdress, Port);
		status = utils::BindDatagramSocket(_sock, &adress);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed _DatagramSocket::_DatagramSocket(x)::BindSocket (0x%08x)", status));
			return status;
		}

		return status;
	}

	NTSTATUS _DatagramSocket::SendData(PVOID Data, WCHAR* LocalAdress, WCHAR* Port){
		SOCKADDR ConnectionAdress = utils::ConvertStringToAdress(_providNpi, LocalAdress, Port);
		return utils::SendDataDtagramSocket(_sock, Data, sizeof(Data), &ConnectionAdress);
	}

	_DatagramSocket::~_DatagramSocket() {
		KdPrint(("_DatagramSocket::~_DatagramSocket()"));
		utils::DeregisterWsk(&_wskReg);
	}
}
