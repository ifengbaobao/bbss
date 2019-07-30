#ifndef SERVERSETWIDGET_H
#define SERVERSETWIDGET_H

#include <QWidget>
#include"servercontrol.h"

namespace Ui {
class ServerSetWidget;
}

class ServerSetWidget : public QWidget
{
    Q_OBJECT
private:
    ServerControl * sc;

    QVector<QHostAddress> broadcasts;

public:
    explicit ServerSetWidget(QWidget *parent = 0);
    ~ServerSetWidget();

private:
    Ui::ServerSetWidget *ui;
};

#endif // SERVERSETWIDGET_H
