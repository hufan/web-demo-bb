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
#ifndef SHADOWWIDGET_H
#define SHADOWWIDGET_H

#include <QWidget>
#include <QColor>

class ShadowWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
public:
    explicit ShadowWidget(QWidget *parent = 0);
    void setOpacity(qreal opacity);
    qreal opacity()
    {
        return widget_opacity;
    }
    void setColor(QColor color)
    {
        widget_color = color;
    }

protected:
    void paintEvent(QPaintEvent *);

private:
    qreal widget_opacity;
    QColor  widget_color;
};

#endif // SHADOWWIDGET_H
