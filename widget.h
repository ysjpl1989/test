#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtMultimediaWidgets>
#include <QMediaPlaylist>
#include <QtMultimedia>
#include <QFileDialog>
#include <QFile>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void addplaylist();//添加播放列表

    void myaddlist(QStringList list);//添加列表核心代码

    void checklist();//检查是否存在播放列表

    void savelist(QStringList playlist);

    bool isFileExist(QString fullFileName);

private slots:
    void on_next_clicked();

    void on_play_clicked();

    void on_back_clicked();

private:
    Ui::Widget *ui;
   QMediaPlayer* player; //多媒体对象
   QMediaPlaylist* mlist;//播放列表
   bool flag; //播放暂停标记
   QString list;
   //QFile savelist;
};

#endif // WIDGET_H
