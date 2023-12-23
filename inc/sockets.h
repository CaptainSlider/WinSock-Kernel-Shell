#include "utils.h"

namespace WinShell::sockets {

	typedef class _ListenerSocket {
	public:
		_ListenerSocket(PSOCKADDR LocalAdress);
		_ListenerSocket();

		NTSTATUS InitSock(PSOCKADDR LocalAdress);

		NTSTATUS Accept();

		~_ListenerSocket();

	private:
		PWSK_SOCKET _sock;
		WSK_PROVIDER_NPI _providNpi;
		WSK_REGISTRATION _wskReg;

	}* PListenerSocket, ListenerSocket;


	typedef class _ConnectionSocket {
	public:
		_ConnectionSocket(PSOCKADDR LocalAdress);
		_ConnectionSocket();

		NTSTATUS InitSock(PSOCKADDR LocalAdress);

		NTSTATUS Connect(PSOCKADDR ConnectionAdress);

		~_ConnectionSocket();

	private:
		PWSK_SOCKET _sock;
		WSK_PROVIDER_NPI _providNpi;
		WSK_REGISTRATION _wskReg;


	}* PConnectionSocket, ConnectionSocket;

	typedef class _DatagramSocket {
	public:
		_DatagramSocket(WCHAR* LocalAdress,WCHAR* Port);
		_DatagramSocket();

		NTSTATUS InitSock(WCHAR* LocalAdress, WCHAR* Port);

		NTSTATUS SendData(PVOID Data, WCHAR* LocalAdress, WCHAR* Port);

		~_DatagramSocket();
		
	private:
		PWSK_SOCKET _sock;
		WSK_PROVIDER_NPI _providNpi;
		WSK_REGISTRATION _wskReg;

	}* PDatagramSocket, DatagramSocket;
}