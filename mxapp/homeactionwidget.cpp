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
#include "homeactionwidget.h"
#include "basewidget.h"
#include "mxmaindialog.h"
#include "mxapplication.h"

#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>

HomeActionWidget::HomeActionWidget(QWidget *parent, MxApplication *obj, int width, int height):BaseWidget(parent,obj)
{
    m_width = width;
    if(m_width <=0){
        m_width = DEFAULT_SCREEN_WIDTH;
    }
    m_height = height;
    if(m_height <= 0){
        m_height = DEFAULT_SCREEN_HEIGHT;
    }
	
	qDebug() << m_width << m_height << " of HomeActionWidget \n" << endl;


    this->setFixedSize(m_width, m_height);
    this->setAutoFillBackground(true);
    this->setObjectName("transparentWidget");
    logo_myir_label = new QLabel();
    cover_myir_label = new QLabel();

    this->initUI();
    this->initConnection();
//    this->setCurrentLanguage(QString("zh_CN"));

}

void HomeActionWidget::initUI()
{
    logo_myir_label->setPixmap(QPixmap(":/res/images/myir/logo_myir.png"));
    cover_myir_label->setPixmap(QPixmap(":/res/images/myir/cover_myir.png"));

    QVBoxLayout *layout1 = new QVBoxLayout();
//    layout1->addStretch();
    layout1->addWidget(cover_myir_label);
//    layout1->addStretch();
//    layout1->setSpacing(15);
//    layout1->setContentsMargins(0, 20, 0, 0);

    QHBoxLayout *layout2 = new QHBoxLayout();
    layout2->addWidget(logo_myir_label, 0, Qt::AlignHCenter);
    layout2->addLayout(layout1);
    layout2->setSpacing(20);
    layout2->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *main_layout = new QHBoxLayout();
    main_layout->addLayout(layout2);
//    main_layout->addStretch();
    main_layout->setSpacing(0);
    main_layout->setMargin(0);
    main_layout->setContentsMargins(40, 30, 44, 20);

    setLayout(main_layout);

}

void HomeActionWidget::initConnection()
{

}

void HomeActionWidget::display()
{

}

void HomeActionWidget::setCurrentLanguage(QString &lang)
{
    qDebug() << "HomeActionWidget setCurrentLanguage :" << lang << endl;
}
