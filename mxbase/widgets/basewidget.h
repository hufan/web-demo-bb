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
#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QString>

#include "mxapplication.h"

class BaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseWidget(QWidget *parent = 0,MxApplication *obj = 0);
    ~BaseWidget();

    virtual void setCurrentLanguage(QString &lang){}
    virtual void initUI(){}
    virtual void initConnection(){}
    virtual void display(){}

signals:

public slots:


private:
    MxApplication  *m_obj;
};

#endif // BASEWIDGET_H
