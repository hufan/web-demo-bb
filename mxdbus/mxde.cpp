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
#include <QObject>
#include "mxde.h"
#include "mxdbus.h"

MxDE::MxDE(QObject *parent) : QObject(parent)
{
    m_app  = parent;
    m_dbus = MxDbus::getMxDubs(m_app);
}

MxDE::~MxDE()
{
    delete m_app;
    m_app = NULL;
    delete m_dbus;
    m_dbus = NULL;
}

bool MxDE::isValid(){
    if(m_dbus){
        MxDbus *bus = (MxDbus*)m_dbus;
        return bus->isValid();
    }
    else
    {
        return false;
    }
}
//led
QString MxDE::callGetLedList()
{
    MxDbus *bus = (MxDbus *)m_dbus;
    return bus->getLedList();
}

int MxDE::callSetLedBrightness(QString &led, int brightness)
{
    MxDbus *bus = (MxDbus *)m_dbus;
    return bus->setLedBrightness(led, brightness);
}
//serial
int MxDE::callOpenSerialPort(const QString &dev_name,QString &tty_configure)
{
    MxDbus *bus = (MxDbus *)m_dbus;
    return bus->openSerialPort(dev_name,tty_configure);
}
int MxDE::callCloseSerialPort(int uart_fd)
{
    MxDbus *bus = (MxDbus *)m_dbus;
    return bus->closeSerialPort(uart_fd);
}
int  MxDE::callSetSerialPort(const QString &parameter)
{
    MxDbus *bus = (MxDbus *)m_dbus;
    return bus->setSerialPort(parameter);
}
QString MxDE::callGetSerialList()
{
    MxDbus *bus = (MxDbus *)m_dbus;
    return bus->getSerialList();
}
int MxDE::callSerialWrite(int uart_fd, const QString &data, int size)
{
    MxDbus *bus = (MxDbus *)m_dbus;
    return bus->SerialWrite(uart_fd,data,size);
}
//rs485
QString MxDE::callgetRs485List()
{
    MxDbus *bus = (MxDbus *)m_dbus;
    return bus->getRs485List();
}
//can
QString MxDE::callgetCanList()
{
    MxDbus *bus = (MxDbus *)m_dbus;
    return bus->getCanList();
}
int MxDE::callopenCanPort(const QString &can_name)
{
    MxDbus *bus = (MxDbus *)m_dbus;
    return bus->openCanPort(can_name);
}
int MxDE::callcloseCanPort(const QString &can_name,int can_fd)
{
    MxDbus *bus = (MxDbus *)m_dbus;
    return bus->closeCanPort(can_name,can_fd);
}
int MxDE::callsetCanPort(const QString &can_name,int bitrate,int status,const QString &loop,QString &can_configure)
{
    MxDbus *bus = (MxDbus *)m_dbus;
    return bus->setCanPort(can_name,bitrate,status,loop,can_configure);
}
int MxDE::callCanWrite(int can_fd,const QString &data,int len)
{
    MxDbus *bus = (MxDbus *)m_dbus;
    return bus->CanWrite(can_fd,data,len);
}
//info
QString MxDE::callgetSystemInfo()
{
    MxDbus *bus = (MxDbus *)m_dbus;
    return bus->getSystemInfo();
}
QString MxDE::callgetCpuInfo()
{
    MxDbus *bus = (MxDbus *)m_dbus;
    return bus->getCpuInfo();
}
QString MxDE::callgetMemoryInfo()
{
    MxDbus *bus = (MxDbus *)m_dbus;
    return bus->getMemoryInfo();
}
QString MxDE::callgetStorageInfo()
{
    MxDbus *bus = (MxDbus *)m_dbus;
    return bus->getStorageInfo();
}
