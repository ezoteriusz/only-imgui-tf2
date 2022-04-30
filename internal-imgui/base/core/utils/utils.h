#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

#include <d3dx9.h>
#include <d3d9.h>
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"d3d9.lib")

class CUtils
{
private:
	FILE* pFile;
	std::uint8_t* PatternScan(void* module, const char* signature);
public:
	void setupCmd();
	void detachCmd();
	void setupSignatures();
};
extern CUtils utils;
extern IDirect3DDevice9* DirectX;

