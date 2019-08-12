#include <iostream>
#include<stdlib.h>
#include<stdio.h>

using namespace std;

int main()
{
    char *line = (char*)malloc(1024);
    char *path = (char*)malloc(2048);
    while(1) {
        int n = fread(line, 1, 1, stdin);
        fwrite(line, 1, n, stdout);
        fflush(stdout);
    }
    return 0;
}
