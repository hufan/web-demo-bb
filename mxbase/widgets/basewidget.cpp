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
#include "basewidget.h"
#include "mxapplication.h"

#include <QObject>
#include <QWidget>

BaseWidget::BaseWidget(QWidget *parent, MxApplication *obj) : QWidget(parent)
{
    m_obj = obj;
}

BaseWidget::~BaseWidget()
{

}
