#include"serversetwidget.h"
#include <QApplication>
#include"debugutils.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DebugUtils::installDebug();
    ServerSetWidget w;
    w.show();
    return a.exec();
}
