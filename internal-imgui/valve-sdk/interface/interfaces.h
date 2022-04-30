#pragma once
#include <Windows.h>
#include <cstdlib>
#include <iostream>
#include <TlHelp32.h>


class CInterfaces
{
private:
	void* getInterface(const char* mod, const char* iface);
public:
	bool initialize();
};
extern CInterfaces interfaces;

