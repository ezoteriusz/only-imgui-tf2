#include "interfaces.h"
CInterfaces interfaces;

void* CInterfaces::getInterface(const char* mod, const char* iface) {
	using fn_capture_iface_t = void* (*)(const char*, int*);
	auto fn_addr = (fn_capture_iface_t)GetProcAddress(GetModuleHandleA(mod), "CreateInterface");

	auto iface_addr = fn_addr(iface, nullptr);

	printf("interface %s located 0x%p\n", iface, fn_addr(iface, nullptr));
	return iface_addr;
}

bool CInterfaces::initialize()
{

	return true;
}