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
#include "shadowwidget.h"

#include "shadowwidget.h"
#include <QPainter>

ShadowWidget::ShadowWidget(QWidget *parent) :
    QWidget(parent)
{
    widget_opacity = 1;
}

void ShadowWidget::setOpacity(qreal opacity)
{
    widget_opacity = opacity;
    update();
}

void ShadowWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setOpacity(widget_opacity);
    painter.setBrush(widget_color);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());
}
