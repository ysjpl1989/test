//自定义线程类
#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>

class mythread : public QObject
{
    Q_OBJECT
public:
    explicit mythread(QObject *parent = nullptr);
    void mytimerout();//线程处理函数
    void setrun(bool a = false);//跳出循环

signals:
    void mysignal();//发送给主线程信号

private:
    bool isrun;
};

#endif // MYTHREAD_H
