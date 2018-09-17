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
#ifndef MXDE_H
#define MXDE_H

#include <QObject>
#include <QWidget>
#include <QString>

class MxDE : public QObject
{
    Q_OBJECT
public:
    explicit MxDE(QObject *parent = 0);
    ~MxDE();
//led
    QString callGetLedList();
    int   callSetLedBrightness(QString &led, int brightness);
//serial
    int callOpenSerialPort(const QString &dev_name,QString &tty_configure);
    int callCloseSerialPort(int uart_fd);
    int callSetSerialPort(const QString &parameter);
    QString callGetSerialList();
    QString callgetRs485List();
    int callSerialWrite(int uart_fd, const QString &data, int size);
//can
    QString callgetCanList();
    int callopenCanPort(const QString &can_name);
    int callcloseCanPort(const QString &can_name,int can_fd);
    int callsetCanPort(const QString &can_name,int bitrate,int status,const QString &loop,QString &can_configure);
    int callCanWrite(int can_fd,const QString &data,int len);
    bool    isValid();
//info
    QString callgetSystemInfo();
    QString callgetCpuInfo();
    QString callgetMemoryInfo();
    QString callgetStorageInfo();
signals:

public slots:
//    void onLedBrightnessChanged(QString &message);

private:
    QObject *m_app;
    QObject *m_dbus;
    bool    isvalid;
};

#endif // MXDE_H
