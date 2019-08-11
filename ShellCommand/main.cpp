#include <QCoreApplication>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <QDir>

using namespace std;

int main(int argc, char *argv[])
{
    while(1) {
        string ln;
        QDir work(QDir::currentPath());
        QString tip = QString("[%1]$").arg(QDir::toNativeSeparators(work.path()));
        cout << tip.toStdString();
        getline(cin, ln);
        QString line(ln.c_str());
        line = line.trimmed();
        if(line.isEmpty()) {
            continue;
        }
        if(line.compare("exit") == 0) {
            cout << "exit program now." << endl;
            break;
        }
        if(line.startsWith("cd ")) {
            if(!work.cd(line.mid(3))) {
               cout << "bad path." << endl;
            }else{
                QDir::setCurrent(work.path());
            }
            continue;
        }
        system(line.toStdString().c_str());
    }
    return 0;
}
