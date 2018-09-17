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
#include "constant.h"
#include "systemactionwidget.h"
#include "basewidget.h"
#include "mxapplication.h"

#include <QDebug>
#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>

SystemActionWidget::SystemActionWidget(QWidget *parent, MxApplication *obj, int width, int height):BaseWidget(parent,obj)
{
    m_width = width;
    if(m_width <=0){
        m_width = DEFAULT_SCREEN_WIDTH;
    }
    m_height = height;
    if(m_height <= 0){
        m_height = DEFAULT_SCREEN_HEIGHT;
    }

    this->setFixedSize(m_width, m_height);
    this->setAutoFillBackground(true);
    this->setObjectName("transparentWidget");

    logo_myir_label = new QLabel();
    title_myir_label = new QLabel();
    m_closeButton = new SystemButton();

    this->initUI();
}

void SystemActionWidget::setParentWindow(QWidget *parent)
{
    m_parent = parent;
}

void SystemActionWidget::initUI()
{
    logo_myir_label->setScaledContents(true);//自动缩放,显示图像大小自动调整为Qlabel大小
    QPixmap label_pixmap(":/res/images/myir/logo_myir.png");
    logo_myir_label->setPixmap(label_pixmap);
//    img_label->setFixedSize(label_pixmap.size());

    if(m_height > 60){
        logo_myir_label->setFixedSize(QSize(200,60));
        title_myir_label->setObjectName("bigWhiteLabel");
    }
    else if(m_height >= 40)
    {
        logo_myir_label->setFixedSize(QSize(150,40));
        title_myir_label->setObjectName("whiteLabel");
    }
    else if(m_height < 40)
    {
        logo_myir_label->setFixedSize(QSize(100,20));
        title_myir_label->setObjectName("smallWhiteLabel");

    }
    title_myir_label->setWordWrap(true);//QLabel自动换行
    title_myir_label->setAlignment(Qt::AlignCenter);
//    title_myir_label->setText(tr("QT5 Demo System"));

    title_myir_label->setMinimumWidth(200);
    title_myir_label->setMaximumWidth(m_width);
    title_myir_label->setContentsMargins(0,0,0,0);

    title_myir_label->setText(tr("LED Test"));
    m_closeButton->loadPixmap(":/res/images/sys/close_button.png");
    m_closeButton->setFocusPolicy(Qt::NoFocus);

    QVBoxLayout *layout1 = new QVBoxLayout();
    layout1->addWidget(logo_myir_label);
    layout1->addStretch();
    layout1->setSpacing(0);
    layout1->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *layout2 = new QVBoxLayout();
    layout2->addWidget(m_closeButton);
    layout2->addStretch();
    layout2->setSpacing(0);
    layout2->setContentsMargins(0, 5, 5, 0);

    QHBoxLayout *layout3 = new QHBoxLayout();
    layout3->addLayout(layout1);
    layout3->addWidget(title_myir_label);
    layout3->addLayout(layout2);
    layout3->setStretchFactor(layout1, 1);
    layout3->setStretchFactor(title_myir_label,8);
    layout3->setStretchFactor(layout2,1);
    layout3->setSpacing(0);
    layout3->setContentsMargins(2, 2, 2, 2);
    setLayout(layout3);
}

void SystemActionWidget::initConnection()
{
    connect(m_closeButton, SIGNAL(clicked()), m_parent, SLOT(OnSystemDialogClosed()));
}

void SystemActionWidget::display()
{

}

void SystemActionWidget::setCurrentLanguage(QString &lang)
{
    qDebug() << "BoxActionWidget setCurrentLanguage :" << lang << endl;
}

