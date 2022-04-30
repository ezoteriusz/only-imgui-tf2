#include "hooks.h"
CHooks hooks;
unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }

bool CHooks::initialize()
{
	const auto ResetTarget = reinterpret_cast<void*>(get_virtual(DirectX, 16));
	const auto PresentTarget = reinterpret_cast<void*>(get_virtual(DirectX, 17));
	Hooks::directx::wnd_procOrginal = reinterpret_cast<decltype(Hooks::directx::wnd_procOrginal)>(SetWindowLongA(FindWindowA("Valve001", nullptr), GWL_WNDPROC, reinterpret_cast<LONG>(Hooks::directx::wnd_proc)));

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize MH_Initialize.");
	if (MH_CreateHook(ResetTarget, &Hooks::directx::reset, reinterpret_cast<void**>(&Hooks::directx::resetOriginal)) != MH_OK)
		throw std::runtime_error("failed to initialize create_move. (outdated index?)");
	if (MH_CreateHook(PresentTarget, &Hooks::directx::present, reinterpret_cast<void**>(&Hooks::directx::presentOriginal)) != MH_OK)
		throw std::runtime_error("failed to initialize directx. (outdated index?)");
	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks.");

	return true;
}



long __stdcall Hooks::directx::wnd_proc(HWND window, UINT msg, WPARAM wparm, LPARAM lparm) noexcept
{
	[[maybe_unused]] static const auto once = [](const HWND& window) noexcept
	{
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(window);

		return true;
	}(window);

	long ImGui_ImplWin32_WndProcHandler(HWND window, UINT msg, WPARAM wparm, LPARAM lparm);
	ImGui_ImplWin32_WndProcHandler(window, msg, wparm, lparm);
	//interfaces->inputSystem->EnableInput(!menu::is_open);

	return CallWindowProcA(Hooks::directx::wnd_procOrginal, window, msg, wparm, lparm);
}

long __stdcall Hooks::directx::reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	return Hooks::directx::resetOriginal(device, params);
}

long __stdcall Hooks::directx::present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND window_override, const RGNDATA* dirty_region) noexcept
{
	if (!ImGui::GetCurrentContext()) return Hooks::directx::presentOriginal(device, src, dest, window_override, dirty_region);

	static const bool imgui_init{ ImGui_ImplDX9_Init(device) };
	static const bool menu_init{ menu::initialize() };

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (ImGui::IsKeyPressed(VK_INSERT, false))
	{
		menu::is_open = !menu::is_open;
	}

	menu::render();

	ImGui::EndFrame();
	ImGui::Render();

	if (device->BeginScene() == D3D_OK)
	{
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}
	return Hooks::directx::presentOriginal(device, src, dest, window_override, dirty_region);
}


void CHooks::detach()
{
	MH_Uninitialize();
	MH_RemoveHook(MH_ALL_HOOKS);
	if (Hooks::directx::wnd_procOrginal)
	{
		SetWindowLongA(FindWindowA("Valve001", nullptr), GWL_WNDPROC, reinterpret_cast<LONG>(Hooks::directx::wnd_procOrginal));
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();

	if (ImGui::GetCurrentContext())
	{
		ImGui::DestroyContext();
	}
	//interfaces->inputSystem->EnableInput(true);
}