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
#ifndef SERVICETAB_H
#define SERVICETAB_H

#include <QObject>
#include <QWidget>
#include <QStackedWidget>
#include <QListWidget>
#include <QLabel>
#include <QSplitter>
#include "serviceitem.h"
#include "servicetestpage.h"
#include "servicedetailpage.h"
#include "serviceconfigpage.h"

class ServiceTab : public QWidget
{
    Q_OBJECT
public:
    explicit ServiceTab(arrayElement ael,QWidget *parent = 0);
    void     setData();
    void initUI();

signals:

public slots:
    void setCurrentPage(int);
    void updateContentsPage(arrayElement ael);
private:

    QSplitter   *splitter;
    QListWidget     *contentsWidget;
    QStackedWidget  *stackedWidget;

    ServiceConfigPage *m_config_page;
    ServiceDetailPage *m_detail_page;
    ServiceTestPage  *m_test_page;

    QString current_tip;
    QStringList type_list;
    arrayElement services_item;
};

#endif // SERVICETAB_H
