#include <iostream>
#include<stdlib.h>
#include<stdio.h>

using namespace std;

int readline(char *line, int max)
{
    int idx = 0;
    while(1) {
        int c = fgetc(stdin);
        if(c == EOF) {
            return EOF;
        }
        if(idx == max-1) {
            line[idx] = '\0';
            return idx;
        }
        if(c == '\r' || c == '\n') {
            line[idx] = '\0';
            if(idx <= 0) {
                continue;
            }
            return idx;
        }
        fputc(c, stdout);
        line[idx++] = char(c);
    }
}

void trim(char *src)
{
    char *begin = src;
    char *end   = src;

    while ( *end++ ) {
        ; // Statement is null
    }

    if ( begin == end ) return;

    while ( *begin == ' ' || *begin == '\t' )
        ++begin;
    while ( (*end) == '\0' || *end == ' ' || *end == '\t')
        --end;

    if ( begin > end ) {
        *src = '\0';
        return;
    }
    while ( begin != end ) {
        *src++ = *begin++;
    }

    *src++ = *end;
    *src = '\0';
    return;
}

int main()
{
    setlocale(LC_ALL,"zh_CN.UTF-8");
    setvbuf(stdin, nullptr, _IONBF, 0);
    setvbuf(stdout, nullptr, _IONBF, 0);
    setvbuf(stderr, nullptr, _IONBF, 0);
    const int maxlen = 1024;
    char *line = (char*)malloc(maxlen);
    char *path = (char*)malloc(maxlen);
    while(1) {
        int n = readline(line, maxlen);
        if(n == EOF) {
            return -1;
        }
        trim(line);
        if(strcmp(line, "exit") == 0) {
            fwrite("exit now", 1, 1, stdout);
            break;
        }
        system(line);
        fflush(stdout);
    }
    fflush(stdout);
    return 0;
}
