//客户端
#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QFile>
#include <QDebug>
#include <QTcpSocket>

namespace Ui {
class client;
}

class client : public QWidget
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = 0);
    ~client();

private slots:
    void on_buttonconnect_clicked();

    void on_buttoncolse_clicked();

private:
    Ui::client *ui;
    QTcpSocket tcpsocket; //传输套接字
    QFile file;
    QString filename;
    qint64 filesize;
    qint64 recvsize;//己接收文件大小

    bool isStart;
};

#endif // CLIENT_H
