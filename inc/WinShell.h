#pragma once

#include <ntddk.h>
#include <wsk.h>

namespace WinShell {
	typedef struct _endpoint
	{
		_endpoint(WCHAR* IpAdress, WCHAR* Port);
		UNICODE_STRING _IpAdress;
		UNICODE_STRING _Port;
	}* Pendpoint, endpoint;
}