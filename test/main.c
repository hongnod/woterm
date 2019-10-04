#include <stdio.h>

#ifdef WINDOWN
#include <Windows.h>
#define STRCAT_S(dst, size, src)  strcat_s(dst, size, src)
#else
#define STRCAT_S(dst, size, src)  strcat(dst, src)
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
    int i = 0;
    char buf[512] = {0};
    STRCAT_S(buf, 512, funArgv[0]);
    for(i = 1; i < argc; i++) {
        STRCAT_S(buf, 512, ",");
        STRCAT_S(buf, 512, funArgv[i]);
    }
    static int idx = 0;
    idx++;
    sprintf(funArgv[argc-1], "%d", idx);
    IpcCall(hdl, funArgv, argc);
    printf("\r\n%s", buf);
}

#ifdef WINDOW
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
#else

int main()
{
    return  0;
}

#endif
