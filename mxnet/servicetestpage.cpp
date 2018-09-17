/******************************************************************************
* Copyright (C), 2016-2017, Sunny.Guo
* FileName:
* Author: Sunny.Guo
* Version: 1.0
* Date:
* Description:
*
* History:
*  <author>  	<time>   	<version >   	<desc>
*  Sunny.Guo   	19/01/2017      1.0     	create this moudle
*
* Licensed under GPLv2 or later, see file LICENSE in this source tree.
*******************************************************************************/
#include "servicetestpage.h"
#include <QMessageBox>
#include <QProcess>
#include <QTextBrowser>
#include <QDebug>
ServiceTestPage::ServiceTestPage(arrayElement  ale,QWidget *parent) : QWidget(parent)
{

    m_width = parent->width()-120;
    m_height = parent->height();
    this->setFixedSize(m_width,m_height);

    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(Qt::white));
    this->setPalette(palette);

    m_pingGrouBox = new QGroupBox(this);
    m_pingGrouBox->setTitle(tr("Ping Test"));
    m_pingGrouBox->setGeometry(QRect(0, 0, m_width, m_height/2-20));

    m_pingWidget = new QWidget(m_pingGrouBox);
    m_pingWidget->setObjectName(QStringLiteral("layoutWidget2"));
    m_pingWidget->setGeometry(QRect(10, 30, 530, 24));

    m_pingHBoxLayout = new QHBoxLayout(m_pingWidget);
    m_pingHBoxLayout->setSpacing(6);
    m_pingHBoxLayout->setContentsMargins(11, 11, 11, 11);
    m_pingHBoxLayout->setObjectName(QStringLiteral("horizontalLayout_3"));
    m_pingHBoxLayout->setContentsMargins(0, 0, 0, 0);

    m_pingLable = new QLabel(m_pingWidget);
    m_pingLable->setObjectName(QStringLiteral("label_6"));
    m_pingLable->setText(tr("URL/IP :"));
    m_pingHBoxLayout->addWidget(m_pingLable);

    m_pingLineEdit = new QLineEdit(m_pingWidget);
    m_pingLineEdit->setObjectName(QStringLiteral("pingLineEdit"));
    m_pingLineEdit->setFocusPolicy(Qt::ClickFocus);
    m_pingHBoxLayout->addWidget(m_pingLineEdit);

    m_pingPushButton = new QPushButton(m_pingWidget);
    m_pingPushButton->setObjectName("netButton");
    m_pingPushButton->setText(tr("Ping"));
    m_pingHBoxLayout->addWidget(m_pingPushButton);

    m_ping_out = new QLabel(m_pingGrouBox);
    m_ping_out->adjustSize();
    m_ping_out->setGeometry(0,60,650,100);
    m_ping_out->setWordWrap(true);
    m_ping_out->setAlignment(Qt::AlignTop);


    services_item = ale;

    ping = new QProcess(this);

    connect(ping, SIGNAL(finished(int)), this, SLOT(readResult()));
    connect(m_pingPushButton, SIGNAL(clicked()), this, SLOT(pingTest()));
}


void ServiceTestPage::pingTest()
{
    if(!m_pingLineEdit->text().isEmpty()){
        remoteIP = m_pingLineEdit->text();
        m_ping_out->clear();
        ping->start("ping -s 1 -c 1 -w 2 " + remoteIP);
    }
}
void ServiceTestPage::readResult(){
    QByteArray out = ping->readAllStandardOutput();
    m_ping_out->setText(QString::fromLocal8Bit(out));


    if (out.indexOf("ttl") != -1)
    {

        QMessageBox::information(NULL,"",tr("ping %1 is online").arg(remoteIP));
    }
    else
    {
        QMessageBox::information(NULL,"",tr("ping %1 is offline").arg(remoteIP));
    }
}
