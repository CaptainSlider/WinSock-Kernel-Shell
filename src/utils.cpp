#include "../inc/utils.h"

NTSTATUS IoCompletionRoutine(
	PDEVICE_OBJECT,
	PIRP,
	PVOID Context
){
	KeSetEvent((PKEVENT)Context, IO_NO_INCREMENT, FALSE);

	return STATUS_MORE_PROCESSING_REQUIRED;
}
NTSTATUS IoCompletionRoutinew(
	PDEVICE_OBJECT,
	PIRP,
	PVOID Context
) {
	KeSetEvent((PKEVENT)Context, IO_NO_INCREMENT, FALSE);

	return STATUS_MORE_PROCESSING_REQUIRED;
}
namespace WinShell::utils {

	//////////////
	//Coommon

	SOCKADDR ConvertStringToAdress(WSK_PROVIDER_NPI wskNpi ,endpoint StringAdress) {
		KEVENT event;
		PIRP Irp = IoAllocateIrp(1, FALSE);
	
		KeInitializeEvent(&event, NotificationEvent, FALSE);

		IoSetCompletionRoutine(Irp, IoCompletionRoutine, &event, TRUE, TRUE, TRUE);

		SOCKADDR adress = { 0 };
		PADDRINFOEXW Result;
		auto status = wskNpi.Dispatch->WskGetAddressInfo(wskNpi.Client, &StringAdress._IpAdress, &StringAdress._Port, NS_ALL, nullptr, nullptr, &Result, nullptr, nullptr, Irp);
		if (status == STATUS_PENDING) {
			KeWaitForSingleObject(&event, Executive, KernelMode, false, NULL);

			status = Irp->IoStatus.Status;
			if (!NT_SUCCESS(status)) {
				IoFreeIrp(Irp);
				return adress;
			}
		}

		adress = *Result[0].ai_addr;

		IoFreeIrp(Irp);
		return adress;
	}
	NTSTATUS InitWsk(PWSK_REGISTRATION wskRegistretion, PWSK_PROVIDER_NPI wskProviderNpi) {
		WSK_CLIENT_DISPATCH wskClientDispatch = { MAKE_WSK_VERSION(1, 0), 0, NULL };
		WSK_CLIENT_NPI clientNpi;
		clientNpi.ClientContext = nullptr;
		clientNpi.Dispatch = &wskClientDispatch;

		auto status = WskRegister(&clientNpi, wskRegistretion);
		if (!NT_SUCCESS(status)) {
			std::exception e("Failed to InitWsk::WskRegsiter()");
			throw e;
			KdPrint(("Failed to InitWsk::WskRegsiter() 0x%08X", status));
			return status;
		}

		status = WskCaptureProviderNPI(wskRegistretion, WSK_INFINITE_WAIT, wskProviderNpi);
		if (!NT_SUCCESS(status)) {
			std::exception e("Failed to InitWsk::WskCaptureProviderNPI");
			throw e;
			KdPrint(("Failed to InitWsk::WskCaptureProviderNPI 0x%08X", status));
			WskDeregister(wskRegistretion);
			return status;
		}

		return status;
	}

	void DeregisterWsk(PWSK_REGISTRATION wskRegistretion) {
		WskReleaseProviderNPI(wskRegistretion);
		WskDeregister(wskRegistretion);
	}


	void CloseSocket(PWSK_SOCKET sock) {
		KEVENT event;
		PIRP Irp = IoAllocateIrp(1, FALSE);

		KeInitializeEvent(&event, NotificationEvent, FALSE);

		SOCKADDR LocalAdress = { 0 };
		LocalAdress.sa_family = AF_INET;

		IoSetCompletionRoutine(Irp, IoCompletionRoutine, &event, TRUE, TRUE, TRUE);

		auto Dispatch = (PWSK_PROVIDER_BASIC_DISPATCH)sock->Dispatch;
		auto status = Dispatch->WskCloseSocket(sock, Irp);
		if (status == STATUS_PENDING) {
			KeWaitForSingleObject(&event, Executive, KernelMode, false, NULL);

			status = Irp->IoStatus.Status;
			if (!NT_SUCCESS(status)) {
				IoFreeIrp(Irp);
				return;
			}
		}

		IoFreeIrp(Irp);
	}

	//Connection Socket
	/////////////////
	NTSTATUS CreateConnectionSocket(PWSK_PROVIDER_NPI wskProviderNpi, PWSK_SOCKET* sock) {
		KEVENT event;
		PIRP Irp = IoAllocateIrp(1, FALSE);
		
		KeInitializeEvent(&event, NotificationEvent, FALSE);

		IoSetCompletionRoutine(Irp, IoCompletionRoutine, &event, TRUE, TRUE, TRUE);

		auto status = wskProviderNpi->Dispatch->WskSocket(wskProviderNpi->Client, AF_INET, SOCK_STREAM, IPPROTO_TCP, WSK_FLAG_CONNECTION_SOCKET, nullptr, nullptr, nullptr, nullptr, nullptr, Irp);
		
		if (!NT_SUCCESS(status)) {
			IoFreeIrp(Irp);
			return status;
		}

		if (status == STATUS_PENDING) {
			KeWaitForSingleObject(&event, Executive, KernelMode, false, NULL);

			status = Irp->IoStatus.Status;
			if (!NT_SUCCESS(status)) {
				IoFreeIrp(Irp);
				return status;
			}
		}
		*sock = (PWSK_SOCKET)Irp->IoStatus.Information;
		IoFreeIrp(Irp);
		return status;
	}

	NTSTATUS BindConnectionSocket(PWSK_SOCKET sock) {
		KEVENT event;
		PIRP Irp = IoAllocateIrp(1, FALSE);

		KeInitializeEvent(&event, NotificationEvent, FALSE);

		SOCKADDR LocalAdress = { 0 };
		LocalAdress.sa_family = AF_INET;

		IoSetCompletionRoutine(Irp, IoCompletionRoutine, &event, TRUE, TRUE, TRUE);

		auto Dispatch = (PWSK_PROVIDER_CONNECTION_DISPATCH)sock->Dispatch;
		auto status = Dispatch->WskBind(sock, &LocalAdress, 0, Irp);
		if (status == STATUS_PENDING) {
			KeWaitForSingleObject(&event, Executive, KernelMode, false, NULL);

			status = Irp->IoStatus.Status;
			if (!NT_SUCCESS(status)) {
				IoFreeIrp(Irp);
				return status;
			}
		}

		IoFreeIrp(Irp);
		return status;
	}

	NTSTATUS DisconnectConnectionSocket(PWSK_SOCKET sock) {
		KEVENT event;
		PIRP Irp = IoAllocateIrp(1, FALSE);

		KeInitializeEvent(&event, NotificationEvent, FALSE);

		SOCKADDR LocalAdress = { 0 };
		LocalAdress.sa_family = AF_INET;

		IoSetCompletionRoutine(Irp, IoCompletionRoutine, &event, TRUE, TRUE, TRUE);

		auto Dispatch = (PWSK_PROVIDER_CONNECTION_DISPATCH)sock->Dispatch;
		auto status = Dispatch->WskDisconnect(sock, NULL, 0, Irp);
		if (status == STATUS_PENDING) {
			KeWaitForSingleObject(&event, Executive, KernelMode, false, NULL);

			status = Irp->IoStatus.Status;
			if (!NT_SUCCESS(status)) {
				IoFreeIrp(Irp);
				return status;
			}
		}

		IoFreeIrp(Irp);
		return status;
	}

	NTSTATUS ConnectConnectionSocket(PWSK_SOCKET sock, PSOCKADDR ConnectionAdress) {
		KEVENT event;
		PIRP Irp = IoAllocateIrp(1, FALSE);

		KeInitializeEvent(&event, NotificationEvent, FALSE);

		IoSetCompletionRoutine(Irp, IoCompletionRoutine, &event, TRUE, TRUE, TRUE);

		auto Dispatch = (PWSK_PROVIDER_CONNECTION_DISPATCH)sock->Dispatch;
		auto status = Dispatch->WskConnect(sock, ConnectionAdress, 0, Irp);
		if (status == STATUS_PENDING) {
			KeWaitForSingleObject(&event, Executive, KernelMode, false, NULL);

			status = Irp->IoStatus.Status;
			if (!NT_SUCCESS(status)) {
				IoFreeIrp(Irp);
				return status;
			}

		}

		IoFreeIrp(Irp);
		return status;
	}

	NTSTATUS SendDataConnectionSocket(PWSK_SOCKET sock, PVOID Data, ULONG DataSize) {
		KEVENT event;
		PIRP Irp = IoAllocateIrp(1, FALSE);
		WSK_BUF buffer;
		buffer.Mdl = IoAllocateMdl(Data, DataSize, FALSE, FALSE, NULL);
		MmBuildMdlForNonPagedPool(buffer.Mdl);
		buffer.Offset = 0;
		buffer.Length = DataSize;
		KeInitializeEvent(&event, NotificationEvent, FALSE);

		IoSetCompletionRoutine(Irp, IoCompletionRoutine, &event, TRUE, TRUE, TRUE);

		auto Dispatch = (PWSK_PROVIDER_CONNECTION_DISPATCH)sock->Dispatch;
		auto status = Dispatch->WskSend(sock,&buffer,0,Irp);
		if (!NT_SUCCESS(status)) {
			IoFreeMdl(buffer.Mdl);
			KdPrint(("Failed status (0x08X"));
			IoFreeIrp(Irp);
			return status;
		}

		if (status == STATUS_PENDING) {
			KeWaitForSingleObject(&event, Executive, KernelMode, false, NULL);

			status = Irp->IoStatus.Status;
			if (!NT_SUCCESS(status)) {
				KdPrint(("Failed status (0x%08X)", status));
				IoFreeIrp(Irp);
				return status;
			}
		}

		auto GetDataSize = Irp->IoStatus.Information;
		if ((USHORT)GetDataSize < 0) {
			IoFreeMdl(buffer.Mdl);
			status = STATUS_INVALID_PARAMETER;
		}

		IoFreeIrp(Irp);
		return status;
	}

	NTSTATUS GetDataConnectionSocket(PWSK_SOCKET sock, PVOID Data, ULONG DataSize) {
		KEVENT event;
		PIRP Irp = IoAllocateIrp(1, FALSE);
		WSK_BUF buffer;
		buffer.Mdl = IoAllocateMdl(Data, DataSize, FALSE, FALSE, NULL);
		MmBuildMdlForNonPagedPool(buffer.Mdl);
		buffer.Offset = 0;
		buffer.Length = DataSize;
		KeInitializeEvent(&event, NotificationEvent, FALSE);

		IoSetCompletionRoutine(Irp, IoCompletionRoutine, &event, TRUE, TRUE, TRUE);

		auto Dispatch = (PWSK_PROVIDER_CONNECTION_DISPATCH)sock->Dispatch;
		auto status = Dispatch->WskReceive(sock, &buffer, 0, Irp);

		if (status == STATUS_PENDING) {
			KeWaitForSingleObject(&event, Executive, KernelMode, false, NULL);

			status = Irp->IoStatus.Status;
			if (!NT_SUCCESS(status)) {
				KdPrint(("Failed status (0x%08X)", status));
				IoFreeIrp(Irp);
				return status;
			}
		}
		if (!NT_SUCCESS(status)) {
			IoFreeMdl(buffer.Mdl);
			KdPrint(("Failed status (0x08X"));
			IoFreeIrp(Irp);
			return status;
		}
		KdPrint(("status = 0x%08X", status));
		auto GetDataSize = Irp->IoStatus.Information;
		if ((USHORT)GetDataSize < 0) {
			IoFreeMdl(buffer.Mdl);
			status = STATUS_INVALID_PARAMETER;
		}

		IoFreeIrp(Irp);
		return status;
	}

	//DatagramSocket
	/////////////////////
	NTSTATUS CreateDatagramSocket(PWSK_PROVIDER_NPI wskProviderNpi, PWSK_SOCKET* sock) {
		KEVENT event;
		PIRP Irp = IoAllocateIrp(1, FALSE);

		KeInitializeEvent(&event, NotificationEvent, FALSE);

		IoSetCompletionRoutine(Irp, IoCompletionRoutine, &event, TRUE, TRUE, TRUE);

		auto status = wskProviderNpi->Dispatch->WskSocket(wskProviderNpi->Client, AF_INET, SOCK_DGRAM, IPPROTO_UDP, WSK_FLAG_DATAGRAM_SOCKET, nullptr, nullptr, nullptr, nullptr, nullptr, Irp);

		if (status == STATUS_PENDING) {
			KeWaitForSingleObject(&event, Executive, KernelMode, false, NULL);

			status = Irp->IoStatus.Status;
			if (!NT_SUCCESS(status)) {
				IoFreeIrp(Irp);
				return status;
			}

			
		}

		*sock = (PWSK_SOCKET)Irp->IoStatus.Information;

		IoFreeIrp(Irp);
		return status;
	}

	NTSTATUS BindDatagramSocket(PWSK_SOCKET sock) {
		KEVENT event;
		PIRP Irp = IoAllocateIrp(1, FALSE);

		SOCKADDR_IN adress = { 0 };
		adress.sin_family = AF_INET;
		adress.sin_port = 1337;

		KeInitializeEvent(&event, NotificationEvent, FALSE);

		IoSetCompletionRoutine(Irp, IoCompletionRoutine, &event, TRUE, TRUE, TRUE);

		auto Dispatch = (PWSK_PROVIDER_DATAGRAM_DISPATCH)sock->Dispatch;
		auto status = Dispatch->WskBind(sock, (PSOCKADDR)&adress, 0, Irp);
		if (status == STATUS_PENDING) {
			KeWaitForSingleObject(&event, Executive, KernelMode, false, 0);

			status = Irp->IoStatus.Status;
			if (!NT_SUCCESS(status)) {
				IoFreeIrp(Irp);
				return status;
			}
		}
		
		IoFreeIrp(Irp);
		return status;
	}

	NTSTATUS SendDataDatagramSocket(PWSK_SOCKET sock, PVOID Data, ULONG DataSize, PSOCKADDR ConnectionAddress) {
		KEVENT event;
		PIRP Irp = IoAllocateIrp(1, FALSE);
		WSK_BUF buffer;
		buffer.Mdl = IoAllocateMdl(Data, DataSize, FALSE, FALSE, NULL);
		MmBuildMdlForNonPagedPool(buffer.Mdl);
		buffer.Offset = 0;
		buffer.Length = DataSize;
		KeInitializeEvent(&event, NotificationEvent, FALSE);

		IoSetCompletionRoutine(Irp, IoCompletionRoutine, &event, TRUE, TRUE, TRUE);

		auto Dispatch = (PWSK_PROVIDER_DATAGRAM_DISPATCH)sock->Dispatch;
		auto status = Dispatch->WskSendTo(sock, &buffer, 0,ConnectionAddress, 0, NULL, Irp);

		if (!NT_SUCCESS(status)) {
			IoFreeMdl(buffer.Mdl);
			KdPrint(("Failed status (0x08X"));
			IoFreeIrp(Irp);
			return status;
		}

		if (status == STATUS_PENDING) {
			KeWaitForSingleObject(&event, Executive, KernelMode, false, NULL);

			status = Irp->IoStatus.Status;
			if (!NT_SUCCESS(status)) {
				KdPrint(("Failed status (0x%08X)", status));
				IoFreeIrp(Irp);
				return status;
			}
		}

		auto GetDataSize = Irp->IoStatus.Information;
		if ((USHORT)GetDataSize < 0) {
			IoFreeMdl(buffer.Mdl);
			status = STATUS_INVALID_PARAMETER;
		}

		IoFreeIrp(Irp);
		return status;
	}

	NTSTATUS GetDataDatagramSocket(PWSK_SOCKET sock, PVOID Data, ULONG DataSize) {		
		KEVENT event;
		NTSTATUS status;
		PIRP Irp = IoAllocateIrp(1, FALSE);
		WSK_BUF buffer;
		buffer.Mdl = IoAllocateMdl(Data, DataSize, FALSE, FALSE, NULL);
		MmBuildMdlForNonPagedPool(buffer.Mdl);
		buffer.Offset = 0;
		buffer.Length = DataSize;
		KeInitializeEvent(&event, NotificationEvent, FALSE);
		IoSetCompletionRoutine(Irp, IoCompletionRoutine, &event, TRUE, TRUE, TRUE);
		auto Dispatch = (PWSK_PROVIDER_DATAGRAM_DISPATCH)sock->Dispatch;
		ULONG Return;
		SOCKADDR_IN adress = { 0 };
		adress.sin_family = AF_INET;
		adress.sin_port = 1337;
		status = Dispatch->WskReceiveFrom(sock, &buffer, 0, (PSOCKADDR)&adress, NULL, NULL, &Return, Irp);
		if (status == STATUS_PENDING) {
			KeWaitForSingleObject(&event, Executive, KernelMode, false, 0);

			status = Irp->IoStatus.Status;

			if (!NT_SUCCESS(status)) {
				KdPrint(("Failed status (0x%08X)", status));
				IoFreeIrp(Irp);
				return status;
			}

		}

		auto GetDataSize = Irp->IoStatus.Information;
		if ((USHORT)GetDataSize < 0) {
			status = STATUS_INVALID_PARAMETER;
		}

		IoFreeIrp(Irp);
		return status;
	}
	//Listener Socket
	//////
	NTSTATUS CreateListeningSocket(WSK_PROVIDER_NPI wskProviderNpi, PWSK_SOCKET* sock) {
		KEVENT event;
		PIRP Irp = IoAllocateIrp(1, FALSE);

		KeInitializeEvent(&event, NotificationEvent, FALSE);

		IoSetCompletionRoutine(Irp, IoCompletionRoutine, &event, TRUE, TRUE, TRUE);

		auto status = wskProviderNpi.Dispatch->WskSocket(wskProviderNpi.Client, AF_INET, SOCK_STREAM, IPPROTO_TCP, WSK_FLAG_LISTEN_SOCKET, nullptr, nullptr, nullptr, nullptr, nullptr, Irp);

		if (status == STATUS_PENDING) {
			KeWaitForSingleObject(&event, Executive, KernelMode, false, NULL);

			status = Irp->IoStatus.Status;
			if (!NT_SUCCESS(status)) {
				IoFreeIrp(Irp);
				return status;
			}

		}

		*sock = (PWSK_SOCKET)Irp->IoStatus.Information;

		IoFreeIrp(Irp);
		return status;
	}

	NTSTATUS BindListenerSocket(PWSK_SOCKET sock) {
		KEVENT event;
		PIRP Irp = IoAllocateIrp(1, FALSE);
		SOCKADDR LocalAdress{ 0 };
		LocalAdress.sa_family = AF_INET;

		KeInitializeEvent(&event, NotificationEvent, FALSE);

		IoSetCompletionRoutine(Irp, IoCompletionRoutine, &event, TRUE, TRUE, TRUE);

		auto Dispatch = (PWSK_PROVIDER_LISTEN_DISPATCH)(sock)->Dispatch;
		auto status = Dispatch->WskBind(sock, &LocalAdress, 0, Irp);
		if (status == STATUS_PENDING) {
			KeWaitForSingleObject(&event, Executive, KernelMode, false, NULL);

			status = Irp->IoStatus.Status;
			if (!NT_SUCCESS(status)) {
				IoFreeIrp(Irp);
				return status;
			}

		}

		IoFreeIrp(Irp);
		return status;
	}
	
	NTSTATUS AcceptConnection(PWSK_SOCKET sock) {
		KEVENT event;
		PIRP Irp = IoAllocateIrp(1, FALSE);

		KeInitializeEvent(&event, NotificationEvent, FALSE);

		IoSetCompletionRoutine(Irp, IoCompletionRoutine, &event, TRUE, TRUE, TRUE);

		auto Dispatch = (PWSK_PROVIDER_LISTEN_DISPATCH)sock->Dispatch;
		auto status = Dispatch->WskAccept(sock, 0, 0, nullptr, nullptr, nullptr, Irp);
		if (status == STATUS_PENDING) {
			KeWaitForSingleObject(&event, Executive, KernelMode, false, NULL);

			status = Irp->IoStatus.Status;
			if (!NT_SUCCESS(status)) {
				IoFreeIrp(Irp);
				return status;
			}
		}

		IoFreeIrp(Irp);
		return status;
	}
}
