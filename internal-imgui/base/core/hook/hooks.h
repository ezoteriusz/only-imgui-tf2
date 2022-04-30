#pragma once
#include <Windows.h>
#include <cstdlib>
#include <cstdint>
#include <TlHelp32.h>

#include "../../../ext/minhook/minhook.h"
#include "../../../ext/imgui/imgui.h"
#include "../../../ext/imgui/imgui_impl_dx9.h"
#include "../../../ext/imgui/imgui_impl_win32.h"

#include "../utils/utils.h"
#include "../menu/menu.h"

namespace Hooks {
	namespace directx {
		long __stdcall wnd_proc(HWND window, UINT msg, WPARAM wparm, LPARAM lparm) noexcept;
		long __stdcall reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept;
		long __stdcall present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND window_override, const RGNDATA* dirty_region) noexcept;
		inline WNDPROC wnd_procOrginal;
		inline std::add_pointer_t<long __stdcall(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*)> resetOriginal;
		inline std::add_pointer_t<long __stdcall(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*)> presentOriginal;
		inline void(__thiscall* lock_cursor)(void*);
	}
}


class CHooks
{
public:
	bool initialize();
	void detach();
};

extern CHooks hooks;
