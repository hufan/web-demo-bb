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
#ifndef MXMAINDIALOG_H
#define MXMAINDIALOG_H

#include "constant.h"
#include "mxapplication.h"
#include "systemactionwidget.h"
#include "systemcontentwidget.h"

#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QDialog>
#include <QPixmap>
#include <QPaintEvent>
#include <QShowEvent>
#include <QPainter>

class MxMainDialog : public QDialog
{
    Q_OBJECT

public:
    int                 m_width;
    int                 m_height;
    int                 m_default_action_width;
    int                 m_default_action_height;
    int                 m_default_content_width;
    int                 m_default_content_height;


    MxMainDialog(MxApplication *app = 0, QWidget *parent = 0, int w = DEFAULT_SCREEN_HEIGHT, int h = DEFAULT_SCREEN_WIDTH);
    ~MxMainDialog();
    void setApplication(MxApplication *app);
    void setParentWindow(QWidget *parent);
    void setCurrentLanguage(QString &lang);
    void display();
    void initConnect();

public slots:
    void OnSystemDialogClosed();
    void MainDialogShown();

protected:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);

private:
    MxApplication       *m_app;
    QPixmap             *main_skin_pixmap;


    SystemActionWidget  *m_actionwidget;
    SystemContentWidget *m_contentwidget;
    QWidget             *m_parent;

};

#endif // MXMAINDIALOG_H
