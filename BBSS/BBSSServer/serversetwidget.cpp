#include "serversetwidget.h"
#include "ui_serversetwidget.h"
#include<QString>
#include"config.h"

ServerSetWidget::ServerSetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerSetWidget)
{
    ui->setupUi(this);
    setFixedSize(220,160);
    setWindowTitle("BBSSS");

    sc = new ServerControl(this);
    broadcasts = ServerControl::getLocalhostBroadcastIps();

    //图片质量。
    connect(ui->slider_quality,&QSlider::valueChanged,[&](int val){
        ui->label_quality->setText(QString::number(val));
    });
    ui->slider_quality->setValue(50);

    //帧间隔。
    connect(ui->slider_interval,&QSlider::valueChanged,[&](int val){
        ui->label_interval->setText(QString::number(val));
    });
    ui->slider_interval->setValue(250);

    for(QVector<QHostAddress>::iterator it = broadcasts.begin();it != broadcasts.end();++it)
    {
        ui->combobox_broadcast->addItem(it->toString());
    }

    ui->lineedit_block_size->setValidator(new QIntValidator(50, 60000, this));
    ui->lineedit_block_size->setText(QString::number(256));


    connect(ui->btn_start,&QPushButton::clicked,[&](){
        int quality = ui->slider_quality->value();
        int interval = ui->slider_interval->value();
        int broadcastIndex = ui->combobox_broadcast->currentIndex();
        QHostAddress broadcast = broadcasts.at(broadcastIndex);
        int blockSize =ui->lineedit_block_size->text().toInt();
        if(blockSize<50)
        {
            blockSize = 50;
        }else if(blockSize > 60000)
        {
            blockSize = 60000;
        }
        ui->lineedit_block_size->setText(QString::number(blockSize));


        sc->setQuality(quality);
        sc->setInterval(interval);
        sc->setPortAndHostAddress(PORT,broadcast);
        sc->setSockBlockSize(blockSize);

        sc->start();

        ui->slider_quality->setEnabled(false);
        ui->slider_interval->setEnabled(false);
        ui->combobox_broadcast->setEnabled(false);
        ui->lineedit_block_size->setEnabled(false);
        ui->btn_start->setEnabled(false);
    });


    connect(ui->btn_stop,&QPushButton::clicked,[&](){
        sc->stop();
        ui->slider_quality->setEnabled(true);
        ui->slider_interval->setEnabled(true);
        ui->combobox_broadcast->setEnabled(true);
        ui->lineedit_block_size->setEnabled(true);
        ui->btn_start->setEnabled(true);
    });

}

ServerSetWidget::~ServerSetWidget()
{
    delete ui;
}
