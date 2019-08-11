#include <QCoreApplication>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <QDir>

using namespace std;


QString getline()
{
    QString line;
    while(1) {
        int c;
        int n = fread(0, &c, 1);
        if(c == '\r' || c == '\n') {
            return line;
        }
        printf("%c", c);
        line.append(QChar(c));
    }
}

int main(int argc, char *argv[])
{
    QFile in;
    QFile err;
    QFile out;
    if(!in.open(stdin, QIODevice::ReadOnly)) {
        return -1;
    }
    if(!out.open(stdout, QIODevice::WriteOnly)) {
        return -1;
    }
    if(!err.open(stderr, QIODevice::WriteOnly)) {
        return -1;
    }
    while(1) {
        QDir work(QDir::currentPath());
        QString tip = QString("[%1]$").arg(QDir::toNativeSeparators(work.path()));
        printf(tip.toStdString().c_str());
        QString line = getline();
        line = line.trimmed();
        if(line.isEmpty()) {
            continue;
        }
        if(line.compare("exit") == 0) {
            printf("exit program now.\r\n");
            break;
        }
        if(line.startsWith("cd ")) {
            if(!work.cd(line.mid(3))) {
               printf("bad path.\r\b");
            }else{
                QDir::setCurrent(work.path());
            }
            continue;
        }
        printf(line.toStdString().c_str());
        system(line.toStdString().c_str());
    }
    return 0;
}
