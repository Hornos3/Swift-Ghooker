#include "Inject.h"

WCHAR exeAddress[MAX_PATH + 1] = {0};

void dllInject(const WCHAR* inputEXE){
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    si.cb = sizeof(STARTUPINFO);
    WCHAR dirPath[MAX_PATH + 1] = {0};
    wcscpy_s(dirPath, MAX_PATH, DLL_DIRECTORY);
    char dllPath[MAX_PATH + 1] = DLL_ADDRESS;
    wcscpy_s(exeAddress, MAX_PATH + 1, inputEXE);

    if(DetourCreateProcessWithDllEx(exeAddress, NULL, NULL, NULL,
                                    TRUE, CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED,
                                    NULL, dirPath, &si, &pi, dllPath, NULL)){
        ResumeThread(pi.hThread);
        WaitForSingleObject(pi.hProcess, INFINITE);
    }else
        printf_s("%d", GetLastError());
}
