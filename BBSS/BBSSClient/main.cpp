#include "showwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ShowWidget w;
    //w.show();
    w.showMaximized();

    return a.exec();
}
