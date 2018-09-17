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
#ifndef HOMEACTIONWIDGET_H
#define HOMEACTIONWIDGET_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QDebug>

#include "basewidget.h"

class HomeActionWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit HomeActionWidget(QWidget *parent = 0,MxApplication *obj = 0, int width = 0, int height = 0);

    void setCurrentLanguage(QString &lang);
    void initUI();
    void initConnection();
    void display();

private:
    QLabel *logo_myir_label;
    QLabel *cover_myir_label;
    QWidget *m_parent;

    int                      m_width;
    int                      m_height;
};

#endif // HOMEACTIONWIDGET_H
