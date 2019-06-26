#include <stdio.h>
#include <Windows.h>

int main()
{
    printf("Hello World!\n");
    HMODULE hdl = LoadLibrary("woipc.dll");
    return 0;
}
