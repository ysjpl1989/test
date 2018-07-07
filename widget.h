//一个简单的计时器
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>
#include "mythread.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void dealclose();
    void dealsignal();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

signals:
    void starthread();
    //void stopthread();

private:
    Ui::Widget *ui;
    mythread* myT;
    QThread* thread;
};

#endif // WIDGET_H
