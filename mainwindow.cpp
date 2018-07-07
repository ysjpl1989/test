#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel> //下拉框模型

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_actioncar_triggered();
    //连接数据库
    connectDB();
    //初始化
    initData();
}

// 连接数据库
void MainWindow::connectDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("passwd");
    db.setDatabaseName("info");//要使用的数据库

    if (!db.open())
    {
        qDebug() << "ERRor";
        return;
    }
}

// 初始化UI
void MainWindow::initData()
{
    QSqlQueryModel* querymodel = new QSqlQueryModel(this);
    querymodel->setQuery("select distinct name from cars ");//sql 语句distinct 过滤重复的值
    ui->changjiacomboBox->setModel(querymodel);//设置厂家模型


    //ui->label_8->setText("0");
    ui->jinelineEdit_2->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 车辆管理
void MainWindow::on_actioncar_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->car);
    ui->label->setText("车辆管理");
}

// 统计信息
void MainWindow::on_actioncalc_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->calc);
    ui->label->setText("统计信息");
}

void MainWindow::on_changjiacomboBox_currentIndexChanged(const QString &arg1)
{
   if (arg1 == "请选择厂家")
   { //初始化界面
       ui->jinelineEdit_2->clear();
       ui->pinpaicomboBox_2->clear();
       ui->label_8->setText("0");
       ui->shuliangspinBox->setValue(0);
       ui->baojialineEdit->clear();
       ui->shuliangspinBox->setEnabled(false);
       ui->pushButton->setEnabled(false);
   }
   else
   {
       ui->pinpaicomboBox_2->clear();
        QSqlQuery query;
        QString sql = QString("select distinct pinpai from cars where name='%1'").arg(arg1);
        query.exec(sql);//调用mysql

        while (query.next())
        {
                QString pinpai = query.value("pinpai").toString();
                ui->pinpaicomboBox_2->addItem(pinpai);
         }
        ui->shuliangspinBox->setEnabled(true);
   }
}

void MainWindow::on_pinpaicomboBox_2_currentIndexChanged(const QString &arg1)
{
    QSqlQuery query;
    QString sql = QString("select jiage, number from cars where name='%1' and pinpai = '%2'").arg(ui->changjiacomboBox->currentText())
            .arg(arg1);
    query.exec(sql);

    while (query.next())
    {
        int jiage = query.value("jiage").toInt();
        QString number = query.value(1).toString();

       ui->baojialineEdit->setText(QString::number(jiage));
       ui->label_8->setText(number);
    }
}

void MainWindow::on_shuliangspinBox_valueChanged(int arg1)
{
    int sum = ui->baojialineEdit->text().toInt() * arg1;
    QSqlQuery query;
    QString str = QString("select number from cars where name = '%1' and pinpai='%2'").arg(ui->changjiacomboBox->currentText())
            .arg(ui->pinpaicomboBox_2->currentText());
    query.exec(str);
    int num = 0;
    while (query.next())
    {
         num = query.value(0).toInt() - arg1;
    }
         if (num < 0)
         {
             QMessageBox::warning(this, "error", "超过库存");
             ui->shuliangspinBox->setValue(query.value(0).toInt());
              //ui->shuliangspinBox
              return;
         }
         ui->jinelineEdit_2->setText(QString::number(sum));
         ui->label_8->setText(QString::number(num));
         ui->pushButton->setEnabled(true);

}

//  取消按键
void MainWindow::on_pushButton_2_pressed()
{
    //on_changjiacomboBox_currentIndexChanged("请选择厂家");
    ui->changjiacomboBox->setCurrentIndex(0);
}

//确定按键
void MainWindow::on_pushButton_clicked()
{
    QString str = QString("update cars set number= '%1' where name='%2' and pinpai='%3'")
            .arg(ui->label_8->text().toInt()).arg(ui->changjiacomboBox->currentText()).arg(ui->pinpaicomboBox_2->currentText());
    QSqlQuery query;
    query.exec(str);
    if (ui->textEdit->toPlainText() == NULL)
    {
        ui->textEdit->setText("********销售记录*********");
    }
    ui->textEdit->append(QString("%1	%2 卖出 :%3 辆").arg(ui->changjiacomboBox->currentText()).arg(ui->pinpaicomboBox_2->currentText())
                         .arg(ui->shuliangspinBox->text()));
    ui->pushButton->setEnabled(false);
}
