#include "utils.h"

namespace WinShell::sockets {

	typedef class _ListenerSocket {
	public:
		_ListenerSocket();

		NTSTATUS Accept();

		~_ListenerSocket();

	private:
		PWSK_SOCKET _sock;
		WSK_PROVIDER_NPI _providNpi;
		WSK_REGISTRATION _wskReg;

	}*PListenerSocket, ListenerSocket;

}

namespace WinShell::sockets::tcp {
	typedef class _ConnectionSocket {
	public:
		_ConnectionSocket();

		NTSTATUS Connect(endpoint ConnectionAdress);
		NTSTATUS SendData(PVOID Data, ULONG DataSize);
		NTSTATUS ReceivData(PVOID Data, ULONG DataSize);
		NTSTATUS Disconnect();

		~_ConnectionSocket();

	private:
		PWSK_SOCKET _sock;
		WSK_PROVIDER_NPI _providNpi;
		WSK_REGISTRATION _wskReg;


	}*PSocket, Socket;
}

namespace WinShell::sockets::udp{

	typedef class _DatagramSocket {
	public:
		_DatagramSocket();

		NTSTATUS SendData(PVOID Data, endpoint ConnectionAdress);
		NTSTATUS ReceivData(PVOID Data, ULONG DataSize);

		~_DatagramSocket();
		
	private:
		PWSK_SOCKET _sock;
		WSK_PROVIDER_NPI _providNpi;
		WSK_REGISTRATION _wskReg;

	}* PSocket, Socket;
}