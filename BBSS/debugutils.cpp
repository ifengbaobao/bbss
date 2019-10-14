#include "debugutils.h"
#include<QDebug>
#include<QDir>
#include<QFile>
#include <QCoreApplication>

DebugUtils::DebugUtils()
{

}

static void MsgHandler(QtMsgType type, const QMessageLogContext & , const QString &msg)
{
    QString cat = "default";
    if(type == QtDebugMsg)
    {
        cat = "debug";
    }
    if(type == QtWarningMsg)
    {
        cat = "warn";
    }
    else if(type == QtCriticalMsg)
    {
        cat = "error";
    }

    QString filename = qApp->applicationName() + ".log";
    QFile file(filename);
    file.open(QIODevice::Append | QIODevice::WriteOnly);
    QString str = QString("%1 : %2\r\n").arg(cat, msg);
    file.write(str.toUtf8());
    file.close();
}

void DebugUtils::installDebug()
{
    qInstallMessageHandler(MsgHandler);
    qDebug()<<"debug begin.";
}
