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
#ifndef SYSTEMACTIONWIDGET_H
#define SYSTEMACTIONWIDGET_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QDebug>

#include "basewidget.h"
#include "systembutton.h"
#include "mxapplication.h"

#include "systemcontentwidget.h"
class SystemActionWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit SystemActionWidget(QWidget *parent = 0,MxApplication *obj = 0, int width = 0, int height = 0);
    void setParentWindow(QWidget *parent);

    void setCurrentLanguage(QString &lang);
    void initUI();
    void initConnection();
    void display();

private:
    QLabel *logo_myir_label;
    QLabel *title_myir_label;

    SystemButton *m_closeButton;
    QWidget *m_parent;
    int                      m_width;
    int                      m_height;

};

#endif // SYSTEMACTIONWIDGET_H
