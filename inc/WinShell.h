#pragma once

#include "..//try_catch/cpp_support.hpp"
#include <ntddk.h>
#include <wsk.h>
#include "..//try_catch//memory_manager.hpp"
#include "..//try_catch//globals.hpp"

namespace WinShell {
	typedef struct _endpoint
	{
		_endpoint(const WCHAR* IpAdress,const WCHAR* Port);
		UNICODE_STRING _IpAdress;
		UNICODE_STRING _Port;
	}* Pendpoint, endpoint;
}