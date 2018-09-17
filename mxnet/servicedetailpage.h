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
#ifndef SERVICEDETAILPAGE_H
#define SERVICEDETAILPAGE_H

#include <QObject>
#include <QWidget>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "serviceitem.h"

class ServiceDetailPage : public QWidget
{
    Q_OBJECT
public:
    explicit ServiceDetailPage(arrayElement ael,QWidget *parent = 0);
    void initUI();
    void showServiceDetails();
    void updateData(arrayElement ael);

    int                 m_width;
    int                 m_height;
signals:

public slots:

private:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QGroupBox *info_group_box;
    QLabel *label_details_right;
    QLabel *label_details_left;

    arrayElement  services_item;
};

#endif // SERVICEDETAILPAGE_H
