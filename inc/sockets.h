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

	}* PListenerSocket, ListenerSocket;


	typedef class _ConnectionSocket {
	public:
		_ConnectionSocket();

		NTSTATUS Connect(endpoint ConnectionAdress);
		NTSTATUS SendData(PVOID Data, ULONG DataSize);

		~_ConnectionSocket();

	private:
		PWSK_SOCKET _sock;
		WSK_PROVIDER_NPI _providNpi;
		WSK_REGISTRATION _wskReg;


	}* PConnectionSocket, ConnectionSocket;

	typedef class _DatagramSocket {
	public:
		_DatagramSocket();

		NTSTATUS SendData(PVOID Data, endpoint ConnectionAdress);
		NTSTATUS GetData(PVOID Data);
		void Unregister();

		~_DatagramSocket();
		
	private:
		PWSK_SOCKET _sock;
		WSK_PROVIDER_NPI _providNpi;
		WSK_REGISTRATION _wskReg;

	}* PDatagramSocket, DatagramSocket;
}