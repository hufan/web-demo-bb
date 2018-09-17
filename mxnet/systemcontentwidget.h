
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
#ifndef SYSTEMCONTENTWIDGET_H
#define SYSTEMCONTENTWIDGET_H
#include "basewidget.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QList>
#include <QTextEdit>
#include <QTabWidget>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusMetaType>
#include <QtDBus/QDBusReply>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtNetwork/QHostAddress>
//#include "mxde.h"
//#include "mxledindicator.h"
#include "mxapplication.h"
#include "serviceitem.h"
#include "servicetab.h"
# define DBUS_PATH "/"
# define DBUS_CON_SERVICE "net.connman"
# define DBUS_VPN_SERVICE "net.connman.vpn"
# define DBUS_CON_MANAGER "net.connman.Manager"
# define DBUS_VPN_MANAGER "net.connman.vpn.Manager"

class SystemContentWidget : public BaseWidget
{
    Q_OBJECT
public:
    SystemContentWidget(QWidget *parent = 0,MxApplication *obj = 0, int width = 0, int height = 0);
    ~SystemContentWidget();
    void setApplication(MxApplication *app);
    void setParentWindow(QWidget *parent);
    void initUI();
    void initConnection();
    void display();

    bool getArray(QList<arrayElement>& r_list, const QDBusMessage& r_msg );
    void getServices();
    bool extractMapData(QMap<QString,QVariant>& r_map, const QVariant& r_var);
    QString getDeviceName(const QDBusObjectPath& objpath);
    void setServiceTab();
    void deleteServiceTab(const QDBusObjectPath& objpath);
signals:
    void servicesChange(arrayElement ael);

public slots:

    void dbsServicesChanged(QList<QVariant>, QList<QDBusObjectPath>, QDBusMessage);
    void dbsServicePropertyChanged(QString, QDBusVariant, QDBusMessage);
private:

    QWidget                 *m_parent;
    QTabWidget              *m_tabWidget;

    MxApplication           *m_app;
    int                      m_width;
    int                      m_height;

    QDBusInterface          *con_manager;
   // QMap<QString,ServiceItem *>     services_map;
    QMap<QString,ServiceTab *>      tab_map;
    QList<arrayElement>   services_list;
};

#endif // SYSTEMCONTENTWIDGET_H
