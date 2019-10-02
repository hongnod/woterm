#include <stdio.h>

#if defined (Q_OS_WIN)
#include <Windows.h>
#endif

typedef void (*FunIpcCallBack)(int hdl, char *argv[], int argc);
typedef int (*FunIpcConnect)(const char* name, FunIpcCallBack cb);
typedef int (*FunIpcCall)(int hdl, char *argv[], int argc);
typedef int (*FunIpcClose)(int hdl);
typedef int (*FunIpcInit)(void);
typedef int (*FunIpcExit)(void);

static FunIpcInit IpcInit;
static FunIpcConnect IpcConnect;
static FunIpcCall IpcCall;
static FunIpcClose IpcClose;
static FunIpcExit IpcExit;

void MyIpcCallBack(int hdl, char *funArgv[], int argc) {
    char buf[512] = {0};
    strcat_s(buf, 512, funArgv[0]);
    for(int i = 1; i < argc; i++) {
        strcat_s(buf, 512, ",");
        strcat_s(buf, 512, funArgv[i]);
    }
    static int idx = 0;
    idx++;
    sprintf(funArgv[argc-1], "%d", idx);
    IpcCall(hdl, funArgv, argc);
    printf("\r\n%s", buf);
}

int main()
{
    printf("Hello World!\n");
    HMODULE hdl = LoadLibraryA("woipc.dll");
    if(hdl == NULL) {
        printf("Can Not Find WoIpc.dll");
        return 0;
    }
    IpcInit = (FunIpcInit)GetProcAddress(hdl, "IpcInit");
    IpcConnect = (FunIpcConnect)GetProcAddress(hdl, "IpcConnect");
    IpcCall = (FunIpcCall)GetProcAddress(hdl, "IpcCall");
    IpcClose = (FunIpcClose)GetProcAddress(hdl, "IpcClose");
    IpcExit = (FunIpcClose)GetProcAddress(hdl, "IpcExit");
    IpcInit();
    int id = IpcConnect("IpcServer", MyIpcCallBack);
    for(int i = 0; i < 0; i++) {
        char name[10];
        sprintf(name, "%d", i);
        char *argv[] = {"sendMessage", "a", "c", name};
        int argc = sizeof(argv) / sizeof(argv[0]);
        Sleep(10);
        IpcCall(id, argv, argc);
    }
    printf("press c to continue.");
    while(getchar() != 'c') {
        printf("press c to continue.");
    }
    //IpcClose(id);
    IpcExit();
    FreeLibrary(hdl);
    printf("exit now.");
    return 0;
}
