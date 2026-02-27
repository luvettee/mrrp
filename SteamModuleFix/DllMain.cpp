#include <Windows.h>
#include "MinHook.hpp"
#include <stdio.h>

decltype(&CreateProcessW) oCreateProcessW = nullptr;

BOOL WINAPI hkCreateProcessW( LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation )
{
    if (!lpApplicationName || !wcsstr(lpApplicationName, L"csgo.exe"))
    {
        return oCreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,  bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
    }

    BOOL result = oCreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
    if (result)
    {
        #ifdef ONETAP_LEGACY
            if (!VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x39210000), 0x10000000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
                MessageBoxA(0, "Failed to allocate memory for OneTap (Legacy) (Part 1)", "SteamModule", MB_OK | MB_ICONERROR);
                return 0;
			}
            if (!VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x11160000), 0xA59000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
				MessageBoxA(0, "Failed to allocate memory for OneTap (Legacy) (Part 2)", "SteamModule", MB_OK | MB_ICONERROR);
                return 0;
			}
			if (!VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x410C0000), 0x10000000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
                MessageBoxA(0, "Failed to allocate memory for OneTap (Legacy) (Part 3)", "SteamModule", MB_OK | MB_ICONERROR);
				return 0;
			}
			if (!VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x102C0000), 0xA59000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
				MessageBoxA(0, "Failed to allocate memory for OneTap (Legacy) (Part 4)", "SteamModule", MB_OK | MB_ICONERROR);
				return 0;
			}

        #elif PANDORA_LEGACY
            if (!VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x36FA0000), 0x700000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
				MessageBoxA(0, "Failed to allocate memory for Pandora (Legacy)", "SteamModule", MB_OK | MB_ICONERROR);
            }

        #elif PANDORA_DESYNC
			if (!VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x40B50000), 0x74C000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
                MessageBoxA(0, "Failed to allocate memory for Pandora (Desync)", "SteamModule", MB_OK | MB_ICONERROR);
			}

        #elif CLARITY
            if (!VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x36080000), 0x237000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
                MessageBoxA(0, "Failed to allocate memory for Clarity.tk", "SteamModule", MB_OK | MB_ICONERROR);
            }

        #elif MILLIONWARE
            if (!VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x30930000), 0x41D000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
                MessageBoxA(0, "Failed to allocate memory for Millionware (Part 1)", "SteamModule", MB_OK | MB_ICONERROR);
                return 0;
            }
            if (!VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x1860000), 0x1000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
                MessageBoxA(0, "Failed to allocate memory for Millionware (Part 2)", "SteamModule", MB_OK | MB_ICONERROR);
                return 0;
            }
            if (!VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x1870000), 0x1000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
                MessageBoxA(0, "Failed to allocate memory for Millionware (Part 3)", "SteamModule", MB_OK | MB_ICONERROR);
                return 0;
            }
            if (!VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x1880000), 0x1000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
                MessageBoxA(0, "Failed to allocate memory for Millionware (Part 4)", "SteamModule", MB_OK | MB_ICONERROR);
                return 0;
            }
        #elif FATALITY_LEGACY
		    if (!VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x1420000), 0x326000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
                MessageBoxA(0, "Failed to allocate memory for Fatality (Legacy)", "SteamModule", MB_OK | MB_ICONERROR);
				return 0;
			}
        #elif AIMWARE_LEGACY
            if (!VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x1B250000), 0x1F000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
                MessageBoxA(0, "Failed to allocate memory for Aimware (Legacy) (Part 1)", "SteamModule", MB_OK | MB_ICONERROR);
                return 0;
            }
            if (!VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x1B270000), 0x1A000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
                MessageBoxA(0, "Failed to allocate memory for Aimware (Legacy) (Part 2)", "SteamModule", MB_OK | MB_ICONERROR);
                return 0;
            }
            if (!VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x2B920000), 0xE3000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
                MessageBoxA(0, "Failed to allocate memory for Aimware (Legacy) (Part 3)", "SteamModule", MB_OK | MB_ICONERROR);
                return 0;
            }
            if (!VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x6D450000), 0x39000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
                MessageBoxA(0, "Failed to allocate memory for Aimware (Legacy) (Part 4)", "SteamModule", MB_OK | MB_ICONERROR);
                return 0;
            }
            if (!VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x1B290000), 0x5000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
                MessageBoxA(0, "Failed to allocate memory for Aimware (Legacy) (Part 5)", "SteamModule", MB_OK | MB_ICONERROR);
                return 0;
            }
            if (!VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x1B240000), 0xA000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
                MessageBoxA(0, "Failed to allocate memory for Aimware (Legacy) (Part 6)", "SteamModule", MB_OK | MB_ICONERROR);
                return 0;
            }
        #endif
    }

    return result;
}

DWORD WINAPI SetupHooks(LPVOID lpParam)
{
    if (MH_Initialize() != MH_OK) {
        MessageBoxA(0, "Failed to initialize MinHook", "SteamModule", MB_OK | MB_ICONERROR);
        return 0;
    }

    if (MH_CreateHook(&CreateProcessW, &hkCreateProcessW, reinterpret_cast<LPVOID*>(&oCreateProcessW)) != MH_OK)
    {
        MessageBoxA(0, "Failed to create hook", "SteamModule", MB_OK | MB_ICONERROR);
		return 0;
    }

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
	{
		MessageBoxA(0, "Failed to initialize hook", "SteamModule", MB_OK | MB_ICONERROR);
		return 0;
	}

    #ifdef ONETAP_LEGACY
	    const char* moduleName = "OneTap (Legacy)";
    #elif PANDORA_LEGACY
        const char* moduleName = "Pandora (Legacy)";
	#elif PANDORA_DESYNC
        const char* moduleName = "Pandora (Desync)";
    #elif CLARITY
	    const char* moduleName = "Clarity.tk";
    #elif MILLIONWARE
	    const char* moduleName = "Millionware";
    #elif FATALITY_LEGACY
	const char* moduleName = "Fatality (Legacy)";
    #elif AIMWARE_LEGACY
	const char* moduleName = "Aimware (Legacy)";
    #endif

    char message[256];
    snprintf(message, sizeof(message), "Ready for %s", moduleName);

    MessageBoxA(0, message, "SteamModule", MB_OK);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, SetupHooks, nullptr, 0, nullptr);
    }
    return TRUE;
}