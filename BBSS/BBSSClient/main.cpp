#include "showwidget.h"
#include <QApplication>
#include"debugutils.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DebugUtils::installDebug();

    ShowWidget w;
    //w.show();
    w.showMaximized();

    return a.exec();
}
