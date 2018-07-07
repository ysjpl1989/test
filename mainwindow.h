//车辆管理系统，配合mysql使用
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void connectDB();//连接数据库
    void initData(); //初始化

private slots:
    void on_actioncar_triggered();

    void on_actioncalc_triggered();

    void on_changjiacomboBox_currentIndexChanged(const QString &arg1);

    void on_pinpaicomboBox_2_currentIndexChanged(const QString &arg1);

    void on_shuliangspinBox_valueChanged(int arg1);

    void on_pushButton_2_pressed();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
