#include "widget.h"
#include "ui_widget.h"
#include <QStringList>
#include <QString>
#include <QDir>
#include <QStandardPaths> //路径
#include <QFileInfo>//文件名
#include <QByteArray>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
   // this->setWindowFlags(Qt::FramelessWindowHint);//隐藏标题栏
    setWindowTitle("music");
    flag = true;
    i = 0;
    player = new QMediaPlayer;//创建多媒体对像
    mlist = new QMediaPlaylist; //播放列表
    list = "/home/red_fihs/playlist.txt";//播放列表存储位置
    player->setPlaylist(mlist);
    mlist->setPlaybackMode(QMediaPlaylist::Loop);
    //查看是否有播放列表
    checklist();
    connect(player, &QMediaPlayer::durationChanged, this, &Widget::updateprogress);
    connect(player,&QMediaPlayer::positionChanged,this,&Widget::updatePosition);//更新进度条
    connect(ui->horizontalSlider,&QSlider::sliderMoved,player,&QMediaPlayer::setPosition);//控制播放进度
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
    //更新标签
    updatelabel();
}

//下一首
void Widget::on_next_clicked()
{
   mlist->next();
    i = mlist->currentIndex();
   flag = true;
   on_play_clicked();
}

//上一首
void Widget::on_back_clicked()
{
    mlist->previous();
    i = mlist->currentIndex();
    flag = true;
    on_play_clicked();
}

//正在播放标签
void Widget::updatelabel()
{
    ui->label_3->setText(listname.at(i));
}

//添加播放列表
void Widget::addplaylist()
{
      QString  initdir = QDir::homePath();//当前路径
      QStringList playlist = QFileDialog::getOpenFileNames(this, tr("选择文件"), initdir, tr("*.mp3","*.wmv"));
      //保存列表
      savelist(playlist);
      checklist();
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
            file.write("#");
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
                file.write("#");
             }
          }
          file.close();
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
    listname.clear();
    for (int i = 0; i < mylist.size(); i++)
    {
        QString filename = mylist.at(i);
        QFileInfo fi = QFileInfo(filename);
        mlist->addMedia(QMediaContent(QUrl::fromLocalFile(filename)));
        listname << fi.fileName();// 去掉路径文件列表
    }
   //更新显示
    ui->listWidget->clear();
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
               QStringList mylist  = mlst.split("#");
               myaddlist(mylist);//保存
        }
    }
}

void Widget::updateprogress(qint64 duration)
{
    ui->horizontalSlider->setRange(0,duration);
    ui->horizontalSlider->setEnabled(duration);
    ui->horizontalSlider->setPageStep(duration);
}

//转换时间
static QString Time(qint64 t)
{
    qint64 second = t/1000;
    qint64 minter = second/60;
    return QString("%1:%2").arg(minter).arg(second%60);
}

//更新进度条
void Widget::updatePosition(qint64 position)
{
    ui->horizontalSlider->setValue(position);
    ui->label->setText(Time(position)+"/"+Time(player->duration()));
}
