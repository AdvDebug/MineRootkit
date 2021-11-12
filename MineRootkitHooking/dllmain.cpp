#include "pch.h"
#include "Hooks.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        HookTerminateProcess();
        HookGetModuleHandle();
        HookOpenProcess();
        HookTerminateThread();
        HookOpenThread();
    }
    return true;
}