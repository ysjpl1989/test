#include "mythread.h"
#include <QThread>
mythread::mythread(QObject *parent) : QObject(parent)
{
    isrun = true;
}

void mythread::mytimerout()
{
    while (isrun)
    {
        QThread::sleep(1);
        emit mysignal();
    }
}

void mythread::setrun(bool a)
{
    isrun = a;
}
