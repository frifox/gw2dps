
#include <windows.h>
#include <iostream>

BOOL inject_dll(DWORD pID, const char * DLL_NAME);
DWORD GetTargetThreadIDFromProcName(const char * ProcName);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    // Retrieve process ID 
    DWORD pID = GetTargetThreadIDFromProcName("Guild Wars 2");
    size_t converted;

    // Get the dll's full path name 
    wchar_t wchar_buf[MAX_PATH] = { 0 };
    GetFullPathName(L"gw2dps_x64.dll", MAX_PATH, wchar_buf, NULL);
    char char_buf[MAX_PATH + 1] = { 0 };
    wcstombs_s(&converted, char_buf, wchar_buf, wcslen(wchar_buf));

    // Inject our main dll 
    if (!inject_dll(pID, char_buf))
    {
        return 0;
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

    LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");

    // Allocate space in the process for our DLL 
    RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(DLL_NAME), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    // Write the string name of our DLL in the memory allocated 
    WriteProcessMemory(Proc, (LPVOID)RemoteString, DLL_NAME, strlen(DLL_NAME), NULL);

    // Load our DLL
    HANDLE thread = CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, NULL, NULL);

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

