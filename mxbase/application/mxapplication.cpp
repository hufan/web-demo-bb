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
#include "mxapplication.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QtSingleApplication>

MxApplication::MxApplication(int &argc, char **argv, bool GUIenabled)
    :QtSingleApplication(argc,argv,GUIenabled)
{

}

void MxApplication::onLedBrightnessChanged(const QString &message)
{
	emit	this->sigLedBrightnessChanged(message);
}

void MxApplication::setMainWindow(QWidget *w)
{
    m_mainWindow = w;
}

QWidget * MxApplication::getMainWindow()
{
    return m_mainWindow;
}

int MxApplication::getMainWindowWidth()
{
    if(m_mainWindow!= NULL){
        return m_mainWindow->width();
    }
    else
    {
        return QApplication::desktop()->screenGeometry(0).width();
    }
}

int MxApplication::getMainWindowHeight()
{
    if(m_mainWindow!= NULL){
        return m_mainWindow->height();
    }
    else
    {
        return QApplication::desktop()->screenGeometry(0).height();
    }
}
