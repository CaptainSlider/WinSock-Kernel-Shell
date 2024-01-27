#include "../inc/sockets.h"

namespace WinShell::sockets {

	//////// Listener Socket
	_ListenerSocket::_ListenerSocket() {
		KdPrint(("_ListenerSocket::_ListenerSocket(x)"));
		auto status = utils::InitWsk(&_wskReg, &_providNpi);
		if (!NT_SUCCESS(status)) {
			exception e;
			e.e = std::exception("Failed _ListenerSocket::_ListenerSocket(x)::InitWsk");
			e.status = status;
			throw e;
		}
		status = utils::CreateListeningSocket(_providNpi, &_sock);
		if (!NT_SUCCESS(status)) {
			exception e;
			e.e = std::exception("Failed _ListenerSocket::_ListenerSocket(x)::CreateListeningSocket");
			e.status = status;
			throw e;
		}
		status = utils::BindListenerSocket(_sock);
		if (!NT_SUCCESS(status)) {
			exception e;
			e.e = std::exception("Failed _ListenerSocket::_ListenerSocket(x)::BindSocket");
			e.status = status;
			throw e;
		}
	}

	NTSTATUS _ListenerSocket::Accept() {
		KdPrint(("_ListenerSocket::Accept"));
		auto status = utils::AcceptConnection(_sock);
		if (!NT_SUCCESS(status)) {
			exception e;
			e.e = std::exception("Failed _ListenerSocket::Accept()");
			e.status = status;
			throw e;
		}
		return status;
	}

	_ListenerSocket::~_ListenerSocket() {
		KdPrint(("_ListenerSocket::~_ListenerSocket()"));
		utils::CloseSocket(_sock);
		utils::DeregisterWsk(&_wskReg);
	}
}

	////////Connection Socket
namespace WinShell::sockets::tcp {
	_ConnectionSocket::_ConnectionSocket() {
		KdPrint(("_ConnectionSocket::_ConnectionSocket(x)"));
		auto status = utils::InitWsk(&_wskReg, &_providNpi);
		if (!NT_SUCCESS(status)) {
			exception e;
			e.e = std::exception("Failed _ConnectionSocket::_ConnectionSocket(x)::InitWsk");
			e.status = status;
			throw e;
		}
		status = utils::CreateConnectionSocket(&_providNpi, &_sock);
		if (!NT_SUCCESS(status)) {
			exception e;
			e.e = std::exception("Failed _ConnectionSocket::_ConnectionSocket(x)::CreateConnectionSocket");
			e.status = status;
			throw e;
		}
		status = utils::BindConnectionSocket(_sock);
		if (!NT_SUCCESS(status)) {
			exception e;
			e.e = std::exception("Failed _ConnectionSocket::_ConnectionSocket(x)::BindSocket");
			e.status = status;
			throw e;
		}
	}

	NTSTATUS _ConnectionSocket::Connect(endpoint ConnectionAdress) {
		SOCKADDR adress = utils::ConvertStringToAdress(_providNpi, ConnectionAdress);
		auto status = utils::ConnectConnectionSocket(_sock, &adress);
		if (!NT_SUCCESS(status)) {
			exception e;
			e.e = std::exception("_ConnectionSocket::Connect");
			e.status = status;
			throw e;
		}
		return status;
	}

	NTSTATUS _ConnectionSocket::SendData(PVOID Data, ULONG DataSize) {
		auto status = utils::SendDataConnectionSocket(_sock, Data, DataSize);;
		if (!NT_SUCCESS(status)) {
			exception e;
			e.e = std::exception("_ConnectionSocket::SendData");
			e.status = status;
			throw e;
		}
		return status;
	}

	NTSTATUS _ConnectionSocket::ReceivData(PVOID Data, ULONG DataSize) {
		auto status = utils::GetDataConnectionSocket(_sock, Data, DataSize);
		if (!NT_SUCCESS(status)) {
			exception e;
			e.e = std::exception("_ConnectionSocket::SendData");
			e.status = status;
			throw e;
		}
		return status;
	}

	NTSTATUS _ConnectionSocket::Disconnect() {
		auto status = utils::DisconnectConnectionSocket(_sock);
		if (!NT_SUCCESS(status)) {
			exception e;
			e.e = std::exception("_ConnectionSocket::SendData");
			e.status = status;
			throw e;
		}
		return status;
	}

	_ConnectionSocket::~_ConnectionSocket() {
		KdPrint(("_ConnectionSocket::~_ConnectionSocket()"));
		utils::CloseSocket(_sock);
		utils::DeregisterWsk(&_wskReg);
	}
}
	

	///DatagramSocket
namespace WinShell::sockets::udp{
	_DatagramSocket::_DatagramSocket() {
		KdPrint(("_DatagramSocket::_DatagramSocket(x)"));

		auto status = utils::InitWsk(&_wskReg, &_providNpi);
		if (!NT_SUCCESS(status)) {
			exception e;
			e.e = std::exception("Failed Failed _DatagramSocket::_DatagramSocket(x)::InitWsk");
			e.status = status;
			throw e;
		}

		status = utils::CreateDatagramSocket(&_providNpi, &_sock);
		if (!NT_SUCCESS(status)) {
			exception e;
			e.e = std::exception("Failed __DatagramSocket::_DatagramSocket(x)::CreateListeningSocket");
			e.status = status;
			throw e;
		}

		status = utils::BindDatagramSocket(_sock);
		if (!NT_SUCCESS(status)) {
			exception e;
			e.e = std::exception("Failed _DatagramSocket::_DatagramSocket(x)::BindSocket");
			e.status = status;
			throw e;
		}
	}


	NTSTATUS _DatagramSocket::SendData(PVOID Data, endpoint ConnectionAdress){
		SOCKADDR adress = utils::ConvertStringToAdress(_providNpi, ConnectionAdress);
		auto status =  utils::SendDataDatagramSocket(_sock, Data, sizeof(Data), &adress);
		if (!NT_SUCCESS(status)) {
			exception e;
			e.e = std::exception("utils::SendDataDatagramSocket");
			e.status = status;
			throw e;
		}
		return status;
	}

	NTSTATUS _DatagramSocket::ReceivData(PVOID Data, ULONG DataSize) {
		auto status = utils::GetDataDatagramSocket(_sock, Data, DataSize);
		if (!NT_SUCCESS(status)) {
			exception e;
			e.e = std::exception("utils::SendDataDatagramSocket");
			e.status = status;
			throw e;
		}
		return status;
	}
	
	_DatagramSocket::~_DatagramSocket() {
		KdPrint(("_DatagramSocket::~_DatagramSocket()"));
		utils::CloseSocket(_sock);
		utils::DeregisterWsk(&_wskReg);
	}

}
