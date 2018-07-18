#include "widget.h"
#include "ui_widget.h"
#include <QStringList>
#include <QString>
#include <QDir>
#include <QStandardPaths> //路径
#include <QFileInfo>//文件名
#include <QByteArray>
#include <QDebug>

//判断文件列表是否存在
//bool isFileExist(QString fullFileName);


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
   // this->setWindowFlags(Qt::FramelessWindowHint);//隐藏标题栏
    setWindowTitle("music");
    flag = true;
    player = new QMediaPlayer;//创建多媒体对像
    mlist = new QMediaPlaylist; //播放列表
    list = "/home/red_fihs/playlist.txt";//播放列表存储位置
    player->setPlaylist(mlist);
    mlist->setPlaybackMode(QMediaPlaylist::Loop);
    //查看是否有播放列表
    checklist();

    connect(ui->addlist, &QPushButton::clicked, this, &Widget::addplaylist);
}

Widget::~Widget()
{
    delete ui;
}

//播放暂停
void Widget::on_play_clicked()
{
    if (flag)
    {
        flag = false;
        player->play();
    }
    else
    {
        player->pause();
        flag = true;
    }
}


//下一首
void Widget::on_next_clicked()
{
   mlist->next();
   flag = true;
   on_play_clicked();
}


//上一首
void Widget::on_back_clicked()
{
    mlist->previous();
    flag = true;
    on_play_clicked();
}

//添加播放列表
void Widget::addplaylist()
{
      QString  initdir = QDir::homePath();//当前路径
      QStringList playlist = QFileDialog::getOpenFileNames(this, tr("选择文件"), initdir, tr("*.mp3","*.wmv"));
      myaddlist( playlist);
      //保存列表
      savelist(playlist);
}

//保存列表
void Widget::savelist(QStringList playlist)
{
    QFile file(list);
    if (isFileExist(list))//播放列表存在，追加方式打开
    {
        file.open(QIODevice::WriteOnly|QIODevice::Append);
        for (int i = 0; i < playlist.size(); i++)
        {
            QString name = playlist.at(i);
            QByteArray ba = name.toLocal8Bit();
            file.write(ba.data());
            file.write("\n");
        }
    }

    else
        {
            file.open(QIODevice::WriteOnly);
            for (int i = 0; i < playlist.size(); i++)
            {
                QString name = playlist.at(i);
                QByteArray ba = name.toLocal8Bit();
                file.write(ba.data());
                file.write("\n");
             }
          file.close();
          }
}

//判断文件是否存在
bool Widget::isFileExist(QString fullFileName)
{
    QFileInfo fileInfo(fullFileName);
    return fileInfo.isFile()?true:false;
}

//添加列表核心代码
void Widget::myaddlist(QStringList mylist)
{
    QStringList listname;
    for (int i = 0; i < mylist.size(); i++)
    {
        QString filename = mylist.at(i);
        QFileInfo fi = QFileInfo(filename);
        mlist->addMedia(QMediaContent(QUrl::fromLocalFile(filename)));
        listname << fi.fileName();// 去掉路径文件列表
    }
   //更新显示
    ui->listWidget->setViewMode(QListView::ListMode);
    ui->listWidget->addItems(listname);
}

//检查是否有播放列表文件
void Widget::checklist()
{
    if (isFileExist(list))
    {
        QFile mm(list);
        if (mm.open(QIODevice::ReadOnly|QIODevice::Text))
        {
               QByteArray str = mm.readAll();
               QString mlst(str);
               QStringList mylist  = mlst.split("\n");
               myaddlist(mylist);//保存
        }
    }
}
