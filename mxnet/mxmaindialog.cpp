
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
#include "mxmaindialog.h"
#include "systemactionwidget.h"
#include "systemcontentwidget.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDialog>
#include <QTimer>
#include <iostream>

MxMainDialog::MxMainDialog(MxApplication *app, QWidget *parent, int w, int h)
    : QDialog(parent)
{
//    m_mxde = new MxDE(this);
    this->setApplication(app);
    m_width = w;
    m_height = h;
    m_default_action_width = m_width;
    m_default_content_width = m_width;
    m_default_action_height = OTHER_ACTION_HEIGHT(m_height);
    m_default_content_height = OTHER_CONTENT_HEIGHT(m_height);
    this->resize(m_width,m_height);
    this->setAutoFillBackground(true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint);
//    this->setWindowOpacity(1);
    this->setStyleSheet("QDialog{border: 1px solid white;border-radius:1px;background-color: #ffffff;}");
    m_actionwidget = NULL;
    m_contentwidget = NULL;

}

MxMainDialog::~MxMainDialog()
{
    if(m_actionwidget != NULL){
        delete m_actionwidget;
        m_actionwidget = NULL;
    }
    if(m_contentwidget != NULL){
        delete m_contentwidget;
        m_contentwidget = NULL;
    }
    if(main_skin_pixmap != NULL){
        delete main_skin_pixmap;
        main_skin_pixmap = NULL;
    }
}

void MxMainDialog::setApplication(MxApplication *app)
{
    m_app = app;
}

//void MxMainDialog::setDbusProxy(MxDE *mxde)
//{
//    m_mxde = mxde;
//}

void MxMainDialog::setParentWindow(QWidget *parent)
{
    m_parent = parent;
}

void MxMainDialog::display(){
    if(m_actionwidget == NULL){
        m_actionwidget = new SystemActionWidget(this,m_app,m_default_action_width,m_default_action_height);
        m_actionwidget->setParentWindow(this);
        m_actionwidget->initConnection();

        m_actionwidget->setGeometry(QRect(0,0,m_default_action_width,m_default_action_height));
        m_actionwidget->setObjectName("transparentWidget");
//        QPalette palette_back;
//        palette_back.setBrush(QPalette::Background, QBrush(QPixmap(":/res/images/myir/mxde_background1.png")));
//        m_actionwidget->setPalette(palette_back);
//        m_actionwidget->setPalette(palette_back);
    }

    if(m_contentwidget == NULL)
    {
        m_contentwidget = new SystemContentWidget(this, m_app,m_default_content_width, m_default_content_height);
        m_contentwidget->setGeometry(QRect(0,m_default_action_height,m_default_content_width,m_default_content_height));
//        m_contentwidget->setDbusProxy(m_mxde);
//        m_contentwidget->loadLeds();
        m_contentwidget->initUI();
        m_contentwidget->setApplication(m_app);
        m_contentwidget->setParentWindow(this);
//        connect(this, SIGNAL(LedBrightnessChanged(const QString)), m_contentwidget, SLOT(onLedBrightnessChanged(const QString)));
    }

    if (this->isHidden()) {
        int windowWidth, windowHeight = 0;
            windowWidth = QApplication::desktop()->screenGeometry(0).width();
            windowHeight = QApplication::desktop()->screenGeometry(0).height();
            this->move((windowWidth - this->width()) / 2,(windowHeight - this->height()) / 2);
        this->show();
        this->raise();
    //        QTimer::singleShot(100, this, SLOT(startDbusDaemon()));
    }

    else {
        this->hide();
    }
}
void MxMainDialog::OnSystemDialogClosed()
{

    std::cout << "MainDialogHide" << std::endl;
    std::cerr << "MainDialogHide" << std::endl;
    this->close();
}

//void MxMainDialog::onLedBrightnessChanged(const QString &message)
//{
//    emit this->LedBrightnessChanged(message);
//}

void MxMainDialog::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev);
//    QTimer::singleShot(50, this, SLOT(MainDialogShown()));
	this->MainDialogShown();
}

void MxMainDialog::MainDialogShown()
{
    std::cout << "MainDialogShown" << std::endl;
    std::cerr << "MainDialogShown" << std::endl;
}

void MxMainDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    main_skin_pixmap = new QPixmap(":/res/images/myir/mxde_background1.png");

    painter.drawPixmap(0,0,this->width(), this->height(), *main_skin_pixmap);
}
