#include <Windows.h>
#include "MinHook.hpp"
#include <stdio.h>

// Global pointer to the original CreateProcessW function
decltype(&CreateProcessW) oCreateProcessW = nullptr;

// Hooked CreateProcessW function
BOOL WINAPI hkCreateProcessW(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
    // Check if the application being launched is csgo.exe
    if (!lpApplicationName || !wcsstr(lpApplicationName, L"csgo.exe"))
    {
        return oCreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
    }

    // Call the original CreateProcessW
    BOOL result = oCreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
    
    // If process creation was successful, allocate memory for Pandora (Desync)
    if (result)
    {
        // Address: 0x395D0000, Size: 0x8CC000
        if (!VirtualAllocEx(lpProcessInformation->hProcess, reinterpret_cast<void*>(0x395D0000), 0x8CC000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) {
            MessageBoxA(0, "Failed to allocate memory for Pandora (Desync)", "girlyboys", MB_OK | MB_ICONERROR);
        }
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
    MessageBoxA(0, "Ready for Pandora (Desync)", "girlyboys", MB_OK);

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
