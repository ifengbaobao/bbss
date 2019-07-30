#include "showwidget.h"
#include "myqbitmapserialize.h"
#include<QPainter>
#include<QImage>
#include"config.h"
#include<QAction>
#include<QMessageBox>
#include<QString>

ShowWidget::ShowWidget(QWidget *parent) : QWidget(parent)
{
    createMenu();
    sock = new SocketRecv();
    emit sock->bindPort(PORT);
    connect(sock,&SocketRecv::recvData,this,&ShowWidget::slotRecvData);
    bitmap = NULL;
}

void ShowWidget::createMenu()
{
    menu = new QMenu();
    showmodeFit = menu->addAction(" 缩放");
    showmodeNoborder = menu->addAction("不缩放");

    connect(showmodeFit,&QAction::triggered,[&](){
        showmode = SHOW_FIT_WINDOW;
        qDebug()<<"缩放";
        update();
    });
    connect(showmodeNoborder,&QAction::triggered,[&](){
        showmode = SHOW_NOBORDER;
        qDebug()<<"不缩放";
        update();
    });

    connect(menu->addAction("关于我们"),&QAction::triggered,[](){
        QMessageBox msg;
        msg.setWindowTitle("关于我们");
        msg.setText("开发者：宝宝\nQQ(微信)：383537473\n\n时间太紧张，还有些不完善。\n后续有时间再调试！ ^-^ ");
        msg.exec();
    });

    showmode = SHOW_NOBORDER;
}

ShowWidget::~ShowWidget()
{
    delete sock;
    if(bitmap != NULL)
    {
        delete bitmap;
    }
}

void ShowWidget::slotRecvData(QByteArray data)
{
    MyBitmap itemBitmap = MyQBitmapSerialize::deserialize(data);
    if(bitmap == NULL)
    {
        bitmap = new MyBitmap(itemBitmap.getSrcWidth(),itemBitmap.getSrcHeight(),itemBitmap.getBytesPixel());
    }
    bitmap->drawBitmap(itemBitmap);
    update();
}

void ShowWidget::paintEvent(QPaintEvent *e)
{
    if(bitmap == NULL)
    {
        return;
    }
    QByteArray byteArr;
    byteArr.append((char *)(bitmap->getHead()),sizeof(BITMAPFILEHEADER));
    byteArr.append((char *)(bitmap->getBitmapInfo()),sizeof(BITMAPINFOHEADER));
    byteArr.append((char *)(bitmap->getData()),bitmap->getDataSize());

    QPainter p(this);
    QImage image;
    image.loadFromData(byteArr);

    if(showmode == SHOW_FIT_WINDOW)
    {
       image = image.scaled(width(),height());
    }
    p.drawImage(0,0,image);
}

void ShowWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
    {
        menu->popup(QCursor::pos());
    }
}

