#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("计时器");
    myT = new mythread;//分配线程空间
    thread = new QThread(this);//启动一个线程
    myT->moveToThread(thread);//添加线程模块
    connect(myT, &mythread::mysignal,this, &Widget::dealsignal);
    connect(this,&Widget::starthread, myT,  &mythread::mytimerout);
    connect(this, &Widget::destroyed, this, &Widget::dealclose);
}

Widget::~Widget()
{
    delete ui;
}

void ::Widget::dealclose()
{
    on_pushButton_2_clicked();
}

//界面计时显示
void Widget::dealsignal()
{
    static int i = 0;
    i++;
    ui->lcdNumber->display(i);
}

//开始按钮
void Widget::on_pushButton_clicked()
{
    if (!thread->isRunning())
    {
        thread->start();
        myT->setrun(true);
        emit starthread();
    }
}

//停止按钮
void Widget::on_pushButton_2_clicked()
{
    if (thread->isRunning())
    {
         myT->setrun();
        thread->quit();//退出
        thread->wait();//等待回收资源
    }
}
