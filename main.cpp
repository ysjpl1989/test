#include "widget.h"
#include <QApplication>
#include "client.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    client w2;
    w2.show();

    return a.exec();
}
