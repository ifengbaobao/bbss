#ifndef SHOWWIDGET_H
#define SHOWWIDGET_H

#include <QWidget>
#include"socketrecv.h"
#include"mybitmap.h"
#include<QByteArray>
#include<QMenu>
#include<QMouseEvent>

class ShowWidget : public QWidget
{
    Q_OBJECT

#define SHOW_FIT_WINDOW 1
#define SHOW_NOBORDER 0

private:
    QMenu* menu;
    //缩放
    QAction* showmodeFit;
    //不缩放。
    QAction* showmodeNoborder;

    //显示模式。
    int showmode;

private:
    SocketRecv * sock;
    MyBitmap * bitmap;
public:
    explicit ShowWidget(QWidget *parent = 0);
    ~ShowWidget();
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);

private:
    void createMenu();

signals:

public slots:
    void slotRecvData(QByteArray data);
};

#endif // SHOWWIDGET_H
