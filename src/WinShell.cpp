#include "..//inc/WinShell.h"
#include "..//inc//utils.h"
#pragma comment(lib, "Netio.lib") 

namespace WinShell {
	_endpoint::_endpoint(WCHAR* IpAdress, WCHAR* Port) {
		RtlInitUnicodeString(&_IpAdress, IpAdress);
		RtlInitUnicodeString(&_Port, Port);
	}
}