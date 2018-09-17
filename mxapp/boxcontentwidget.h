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
#ifndef BOXCONTENTWIDGET_H
#define BOXCONTENTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QList>
#include <QListView>
#include <QModelIndex>
#include <QPoint>
#include <QMouseEvent>

#include "basewidget.h"
#include "mxdesktopfile.h"
#include "mxlistmodel.h"
#include "mxapplication.h"

class BoxContentWidget : public BaseWidget
{
    Q_OBJECT
public:
    BoxContentWidget(QWidget *parent = 0,MxApplication *obj = 0, int width = 0, int height = 0);

    void setParentWindow(QWidget *w);
    void setCurrentLanguage(QString &lang);
    void initUI();
    void initConnection();
    void display();

    void loadApplications();
    void loadApplicationWidgets();

signals:
    void sigClickSystemInfo();
    void demoFinished();
    void demoStarted();

public slots:
    void OnClickListView(const QModelIndex &index);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

private:
    QWidget                 *m_parent;
    MxApplication           *m_mxapp;
    QLabel                  *m_demo;
    QListView               *m_list_view;
    QList<MxDesktopFile *>   m_apps;
    MxListModel              m_appModel;

    int                      m_width;
    int                      m_height;

    QPoint mousePoint;
    bool mousePressed;              //鼠标是否按下


};

#endif // BOXCONTENTWIDGET_H
