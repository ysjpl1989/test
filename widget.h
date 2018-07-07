#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
//服务器端
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void readdate();

private slots:
    void on_buttonfile_clicked();
    
    void on_buttonsend_clicked();

private:
    Ui::Widget *ui;
    QTcpServer* tcpserver;
    QTcpSocket* tcpsocket;
    //文件信息
    QFile file;
    QString filename;
    qint64 filesize;
    qint64 sendsize;

    QTimer timer;
};

#endif // WIDGET_H
