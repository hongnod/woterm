#include "qwoshellwidget_win32.h"
#include "qwosetting.h"
#include "qwoprocess.h"

#include <QCloseEvent>
#include <QIODevice>
#include <QDebug>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

QWoWin32ShellWidget::QWoWin32ShellWidget(QWidget *parent)
    : QWidget (parent)
{
    init();
}

void QWoWin32ShellWidget::closeEvent(QCloseEvent *event)
{
    emit aboutToClose(event);
    if(event->isAccepted()) {
        return;
    }
    QWidget::closeEvent(event);
}

BOOL CALLBACK EnumThreadWndProc(HWND hwnd, LPARAM lParam)
{
    char title[256];
    ::GetWindowTextA(hwnd, title, 256);
    DWORD pid;
    ::GetWindowThreadProcessId(hwnd, &pid);
    if(pid == lParam || 1) {
        qDebug() << "title" << title << "pid:" << pid;
    }
    return TRUE;
}

void QWoWin32ShellWidget::init2()
{
    QString path = QWoSetting::shellProgramPath();
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.wShowWindow = SW_SHOW;
    si.dwFlags = STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;
    ZeroMemory(&pi, sizeof(pi));
    std::wstring wstr = path.toStdWString();
    if(!CreateProcessW(nullptr, (LPWSTR)wstr.c_str(), nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi)) {
        return;
    }    
    //DWORD id = GetCurrentThreadId();
    DWORD id = GetThreadId(pi.hThread);
    WaitForSingleObject(pi.hProcess, 3000);
    EnumThreadWindows(id, EnumThreadWndProc, (LPARAM)this);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void QWoWin32ShellWidget::init()
{
    QString path = QWoSetting::shellProgramPath();
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.wShowWindow = SW_SHOW;
    si.dwFlags = STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;
    ZeroMemory(&pi, sizeof(pi));
    std::wstring wstr = path.toStdWString();
    if(!CreateProcessW(nullptr, (LPWSTR)wstr.c_str(), nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi)) {
        return;
    }
    DWORD id = GetProcessId(pi.hProcess);
    qDebug() << "pid:" << id;
    EnumWindows(EnumThreadWndProc, (LPARAM)id);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
