#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
   ui->buttonfile->setEnabled(false);
   ui->buttonsend->setEnabled(false);
    setWindowTitle("服务器");
    tcpserver = new QTcpServer(this);

    tcpserver->listen(QHostAddress::Any,8888);

    connect(tcpserver, &QTcpServer::newConnection,
            [=] ()
          {
                tcpsocket = tcpserver->nextPendingConnection();
                QString ip = tcpsocket->peerAddress().toString();
                quint16 port = tcpsocket->peerPort();

                QString str = QString("[%1:%2] 连接成功").arg(ip).arg(port);
                ui->textEdit->setText(str);
                ui->buttonfile->setEnabled(true);

          }
    );
    connect(&timer, &QTimer::timeout,
            [=] ()
            {
                timer.stop();
                readdate();
            }
            );
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_buttonfile_clicked()
{
   QString filepath = QFileDialog::getOpenFileName(this,"open","/home/red_fihs",tr("txt(*.txt *.docx)"));
   if (filepath.isEmpty() == false)
   {
       filename.clear();
       filesize = 0;
        QFileInfo info(filepath);
        filename = info.fileName();
        filesize = info.size();
        sendsize = 0;
        file.setFileName(filepath);
        file.open(QIODevice::ReadOnly);
        bool isok = file.open(QIODevice::ReadOnly);
        if (!isok)
        {
            qDebug() << "open fail";
        }
       ui->buttonfile->setEnabled(false);
       ui->buttonsend->setEnabled(true);

        ui->textEdit->append(filepath);
   }
   else
   {
       qDebug() << "Error";
   }
}
void Widget::readdate()
{
    qint64 len = 0;
    do
    {
        char buff[1024*4] = {0};
        len = file.read(buff,sizeof(buff));
        tcpsocket->write(buff,len);
        sendsize += len;
    }while(len > 0);

    if (sendsize == filesize)
    {
        ui->textEdit->append("OK");
        file.close();
        tcpsocket->disconnectFromHost();
        tcpsocket->close();
    }
    else
    {
        qDebug() << "error";
        ui->buttonfile->setEnabled(true);
        ui->buttonsend->setEnabled(false);
        file.close();
    }
}

void Widget::on_buttonsend_clicked()
{
    QString head = QString("%1##%2").arg(filename).arg(filesize);
    qint64 len = tcpsocket->write(head.toUtf8());
    if (len > 0)
    {
        timer.start(20);
    }
    else
    {
        qDebug() << "error";
        file.close();
        ui->buttonfile->setEnabled(true);
        ui->buttonsend->setEnabled(false);
    }
}
