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
#ifndef BOXACTIONWIDGET_H
#define BOXACTIONWIDGET_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QDebug>

#include "basewidget.h"
#include "systembutton.h"
#include "mxapplication.h"

class BoxActionWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit BoxActionWidget(QWidget *parent = 0,MxApplication *obj = 0, int width = 0, int height = 0);

    void setCurrentLanguage(QString &lang);
    void initUI();
    void initConnection();
    void display();

signals:
    void CloseBox();

private:
    QLabel *logo_myir_label;
    QLabel *title_myir_label;
    QWidget *m_parent;
    SystemButton *m_closeButton;

    int                      m_width;
    int                      m_height;
    MxApplication           *m_mxapp;
};

#endif // BOXACTIONWIDGET_H
