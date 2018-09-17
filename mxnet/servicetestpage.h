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
#ifndef SERVICETESTPAGE_H
#define SERVICETESTPAGE_H

#include <QObject>
#include <QWidget>

#include "serviceitem.h"
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
#include <QTextBrowser>
#include <QProcess>
class ServiceTestPage : public QWidget
{
    Q_OBJECT
public:
    explicit ServiceTestPage(arrayElement  ale, QWidget *parent = 0);
    int                 m_width;
    int                 m_height;

signals:

public slots:
    void pingTest();
    void readResult();
private:

    QGroupBox *m_pingGrouBox;
    QGridLayout *m_pingGridLayout;
    QWidget *m_pingWidget;
    QHBoxLayout *m_pingHBoxLayout;
    QLabel *m_pingLable;
    QLineEdit *m_pingLineEdit;
    QPushButton *m_pingPushButton;
    QTextBrowser *m_testBrowser;
    QLabel *m_ping_out;

    arrayElement  services_item;

    QProcess *ping;
    bool running;
    QString remoteIP;
};

#endif // SERVICETESTPAGE_H
