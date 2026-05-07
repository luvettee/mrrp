#include <Windows.h>
#include "MinHook.hpp"
#include <stdio.h>

// Global pointer to the original CreateProcessW function
decltype(&CreateProcessW) oCreateProcessW = nullptr;

// Hooked CreateProcessW function
BOOL WINAPI hkCreateProcessW(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
    // Check if the application being launched is csgo.exe
    bool isCsgo = false;
    if (lpApplicationName && wcsstr(lpApplicationName, L"csgo.exe")) isCsgo = true;
    if (lpCommandLine && wcsstr(lpCommandLine, L"csgo.exe")) isCsgo = true;

    if (!isCsgo)
    {
        return oCreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
    }

    // Call the original CreateProcessW
    BOOL result = oCreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
    
    // If process creation was successful, allocate memory
    if (result)
    {
#if defined(MILLIONWARE)
        VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x30930000), 0x41D000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x1860000), 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x1870000), 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x1880000), 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#elif defined(PLAGUE)
        VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x3A300000), 0x01000000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#else // PANDORA_DESYNC (default)
        VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x395D0000), 0x8CC000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#endif
    }

    return result;
}

// Function to set up the hooks
DWORD WINAPI SetupHooks(LPVOID lpParam)
{
    // Initialize MinHook
    if (MH_Initialize() != MH_OK) {
        MessageBoxA(0, "Failed to initialize MinHook", "girlyboys", MB_OK | MB_ICONERROR);
        return 0;
    }

    // Create hook for CreateProcessW
    if (MH_CreateHook(&CreateProcessW, &hkCreateProcessW, reinterpret_cast<LPVOID*>(&oCreateProcessW)) != MH_OK)
    {
        MessageBoxA(0, "Failed to create hook", "girlyboys", MB_OK | MB_ICONERROR);
        return 0;
    }

    // Enable all hooks
    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
    {
        MessageBoxA(0, "Failed to initialize hook", "girlyboys", MB_OK | MB_ICONERROR);
        return 0;
    }

    // Signal that we are ready
#if defined(MILLIONWARE)
    MessageBoxA(0, "Ready for Millionware", "girlyboys", MB_OK);
#elif defined(PLAGUE)
    MessageBoxA(0, "Ready for Plague", "girlyboys", MB_OK);
#else
    MessageBoxA(0, "Ready for Pandora (Desync)", "girlyboys", MB_OK);
#endif

    return 0;
}

// DLL entry point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        // Create a new thread to set up hooks to avoid blocking DllMain
        CreateThread(nullptr, 0, SetupHooks, nullptr, 0, nullptr);
    }
    return TRUE;
}
