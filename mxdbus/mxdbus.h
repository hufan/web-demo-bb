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
#ifndef MXDBUS_H
#define MXDBUS_H

#include <QObject>
#include <QWidget>
#include <QMutex>

#include "mxde_interface.h"

#define DBUS_SERVICE_NAME   "com.myirtech.mxde"
#define DBUS_SERVICE_PATH   "/com/myirtech/mxde"

class MxDbus : public QObject
{
    Q_OBJECT
public:
    static MxDbus *getMxDubs(QObject *object);
    bool    isValid();
//led
    QString getLedList();
    int   setLedBrightness(QString &led, int brightness);
//serial
    int openSerialPort(const QString &dev_name,QString &tty_configure);
    int closeSerialPort(int uart_fd);
    int setSerialPort(const QString &parameter);
    QString getSerialList();
    int SerialWrite(int uart_fd, const QString &data, int size);
//485
    QString getRs485List();
//can
    QString getCanList();
    int openCanPort(const QString &can_name);
    int closeCanPort(const QString &can_name,int can_fd);
    int setCanPort(const QString &can_name,int bitrate,int status,const QString &loop,QString &can_configure);
    int CanWrite(int can_fd,const QString &data,int len);
//info
    QString getSystemInfo();
    QString getCpuInfo();
    QString getMemoryInfo();
    QString getStorageInfo();
signals:

public slots:

protected:
    void startService();
    void stopService();

private:
    explicit MxDbus(QObject *parent = 0);
    ~MxDbus();

//    QReadWriteLock internalMutex;
//    static QMutex  m_mutex;
//    static QAtomicPointer<MxDbus> m_dbus;
    static MxDbus   *m_dbus;
    QObject  *m_object;
    com::myirtech::mxde::MxdeInterface  *mxde_session_iface;
    com::myirtech::mxde::MxdeInterface  *mxde_system_iface;
    bool isvalid;
};

#endif // MXDBUS_H
