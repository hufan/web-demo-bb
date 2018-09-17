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
#ifndef SERVICECONFIGPAGE_H
#define SERVICECONFIGPAGE_H

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

class ServiceConfigPage : public QWidget
{
    Q_OBJECT
public:
    //ServiceItem *service
    explicit ServiceConfigPage(arrayElement ael, QWidget *parent = 0);
    void initUI();
    void updateData(arrayElement ael);
    void create_config_page();
    void create_detail_page();
    void showServiceDetails();
    int                 m_width;
    int                 m_height;
signals:

public slots:
    void ipv4Method(int idx);
    void updateConfiguration();
private:
    QGroupBox *m_settingGroup;


    QWidget *m_methodlayoutWidget;
    QGridLayout *m_methodgridLayout;
    QGridLayout *m_mainlayout;
    QLabel *m_methodLable;
    QComboBox *m_methodComboBox;

    QWidget *m_infolayoutWidget;
    QFormLayout *m_infoformLayout;
    QLabel *m_ipLable;
    QLabel *m_netmaskLable;
    QLabel *m_gatewayLable;
    QLineEdit *m_ipLineEdit;
    QLineEdit *m_netmaskLineEdit;
    QLineEdit *m_gatewayLineEdit;

    QPushButton *m_cancleButton;
    QPushButton *m_okButton;

    arrayElement   services_item;
    QMap<QString,QVariant> objmap;
    QMap<QString,QVariant> ipv4map;
    QDBusObjectPath objpath;
    QStringList sl_ipv4_method;
    QStringList sl_ipv4_method_minuscules;

    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QGroupBox *info_group_box;
    QLabel *label_details_right;
    QLabel *label_details_left;
};

#endif // SERVICECONFIGPAGE_H
