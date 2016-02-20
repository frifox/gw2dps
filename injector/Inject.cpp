#ifndef INJECT_CPP
#define INJECT_CPP

#include "Inject.h"

BOOL inject_dll(DWORD pID, const char * DLL_NAME);
DWORD GetTargetThreadIDFromProcName(const char * ProcName);

typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process;

BOOL IsWow64(DWORD pID)
{
    HANDLE Proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (!Proc)
    {
        return true;
    }

    BOOL bIsWow64 = FALSE;

    //IsWow64Process is not available on all supported versions of Windows.
    //Use GetModuleHandle to get a handle to the DLL that contains the function
    //and GetProcAddress to get a pointer to the function if available.
    fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
        GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

    if (NULL != fnIsWow64Process)
    {
        if (!fnIsWow64Process(Proc, &bIsWow64))
        {
            return true;
        }
    }
    else return true;

    CloseHandle(Proc);
    return bIsWow64;
}

bool inject64() {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);

    // we need a 64-bit proxy process to be able to call 64-bit windows APIs and inject a 64-bit dll.
    if (!CreateProcessA(NULL, "proxy64.exe", NULL, NULL, false, 0, NULL, NULL, &si, &pi)) {
        return false;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}

int inject()
{
    // Retrieve process ID 
    DWORD pID = GetTargetThreadIDFromProcName("Guild Wars 2");

    // Get the dll's full path name 
    wchar_t wchar_buf[MAX_PATH] = { 0 };
    GetFullPathName(L"gw2dps.dll", MAX_PATH, wchar_buf, NULL);
    char char_buf[MAX_PATH + 1] = { 0 };
    wcstombs(char_buf, wchar_buf, wcslen(wchar_buf));

    if (IsWow64(pID)) {
        // Inject our main dll 
        if (!inject_dll(pID, char_buf))
        {
            return 0;
        }
    } else {
        if (!inject64()) return 0;
    }
    return 1;
}

BOOL inject_dll(DWORD pID, const char * DLL_NAME)
{
    HANDLE Proc;
    char buf[50] = { 0 };
    LPVOID RemoteString, LoadLibAddy;

    if (!pID)
        return false;

    Proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (!Proc)
    {
        printf(buf, "OpenProcess() failed: %d", GetLastError());
        printf(buf);
        return false;
    }

    LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");

    // Allocate space in the process for our DLL 
    RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(DLL_NAME), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    // Write the string name of our DLL in the memory allocated 
    WriteProcessMemory(Proc, (LPVOID)RemoteString, DLL_NAME, strlen(DLL_NAME), NULL);

    // Load our DLL 
    CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, NULL, NULL);

    CloseHandle(Proc);
    return true;
}

DWORD GetTargetThreadIDFromProcName(const char * ProcName)
{
    HWND windowHandle = FindWindowA(NULL, ProcName);
    DWORD* processID = new DWORD;
    GetWindowThreadProcessId(windowHandle, processID);

    return *processID;
}

#endif
