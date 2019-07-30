#include"serversetwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerSetWidget w;
    w.show();

    return a.exec();
}
