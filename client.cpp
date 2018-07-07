#include "client.h"
#include "ui_client.h"
#include <QFileInfo>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>

client::client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
    setWindowTitle("客户端");
    tcpsocket.setParent(this);
    isStart = true;
   connect(&tcpsocket, &QTcpSocket::readyRead,
           [=]()
         {
            QByteArray buff = tcpsocket.readAll();
            if (isStart)
            {
                isStart = false;
                filename = QString(buff).section("##", 0, 0);
                filesize = QString(buff).section("##", 1, 1).toInt();
                recvsize = 0;
                QString str = QString("正在接收%1(%2) ").arg(filename).arg(filesize);
                ui->textEdit->append(str);

                file.setFileName(filename);
                bool isok = file.open(QIODevice::WriteOnly);
                if (!isok)
                {
                    qDebug() << "open error";
                    tcpsocket.disconnectFromHost();
                    tcpsocket.close();
                }
            }
            else
            {
                qint64 len = file.write(buff);
                recvsize += len;

                if (recvsize == filesize)
                {
                    file.close();
                    tcpsocket.disconnectFromHost();
                    tcpsocket.close();
                    QMessageBox::information(this,"ok", "接收完成");
                }
            }

         }
           );
}

client::~client()
{
    delete ui;
}

void client::on_buttonconnect_clicked()
{
   QString ip = ui->lineeditip->text();
   qint16 port = ui->lineeditport->text().toInt();
   tcpsocket.connectToHost(QHostAddress(ip), port);
}

//关闭客户端
void client::on_buttoncolse_clicked()
{
    close();
}
